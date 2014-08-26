#include <Engine/Model.hpp>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

engine::Model::Model(void)
{
	_tD3DObject = NULL;
	_pMVPMatrixBuffer = NULL;
	_pNormalMatrixBuffer = NULL;
	_pInputLayout = NULL;
	_MVPMatrix = (XMMATRIX *)_aligned_malloc(sizeof *_MVPMatrix, 16);
	_ModelMatrix = (XMMATRIX *)_aligned_malloc(sizeof *_ModelMatrix, 16);
	_NormalMatrix = (XMMATRIX *)_aligned_malloc(sizeof *_NormalMatrix, 16);
	_program = NULL;
	_pd3dDevice = NULL;
	_pContext = NULL;

	matIdentity();
}

engine::Model::~Model(void)
{
	UINT i;
	if(_tD3DObject != NULL && isMirror == FALSE)
	{
		for (i = 0; i < _tD3DObject->size(); i++)
			delete (*_tD3DObject)[i];
		delete _tD3DObject;
	}

	_aligned_free(_NormalMatrix);
	_aligned_free(_ModelMatrix);
	_aligned_free(_MVPMatrix);

	if (_pInputLayout)
		_pInputLayout->Release();
	if (_pNormalMatrixBuffer)
		_pNormalMatrixBuffer->Release();
	if (_pMVPMatrixBuffer)
		_pMVPMatrixBuffer->Release();
}

void engine::Model::initD3DObjectArray(void)
{
	UINT i;
	if(_tD3DObject != NULL && isMirror == FALSE)
	{
		for (i = 0; i < _tD3DObject->size(); i++)
			delete (*_tD3DObject)[i];
		delete _tD3DObject;
	}
	isMirror = FALSE;
	_tD3DObject = new std::vector<D3DObject *>;
}

void engine::Model::initD3DObjectMirror(Model *m)
{
	UINT i;
	if (_tD3DObject != NULL && isMirror == FALSE)
	{
		for (i = 0; i < _tD3DObject->size(); i++)
			delete (*_tD3DObject)[i];
		delete _tD3DObject;
	}
	isMirror = TRUE;
	_tD3DObject = m->_tD3DObject;
}

void engine::Model::config(ShaderProgram *program, ID3D11Device *pd3dDevice, ID3D11DeviceContext *pContext)
{
	HRESULT hr;
	D3D11_BUFFER_DESC bd;

	_program = program;
	_pd3dDevice = pd3dDevice;
	_pContext = pContext;

	if (_pMVPMatrixBuffer)
		_pMVPMatrixBuffer->Release();
	if (_pNormalMatrixBuffer)
		_pNormalMatrixBuffer->Release();

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;

	// MVPMatrix Buffer
	bd.ByteWidth = sizeof *_MVPMatrix;
	hr = _pd3dDevice->CreateBuffer(&bd, NULL, &_pMVPMatrixBuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create MVPMatrix Buffer", "Model", MB_OK);
		exit(1);
	}

	// NormalMatrix Buffer
	bd.ByteWidth = sizeof *_NormalMatrix;
	hr = _pd3dDevice->CreateBuffer(&bd, NULL, &_pNormalMatrixBuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create NormalMatrix Buffer", "Model", MB_OK);
		exit(1);
	}

	// Create and set the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "IN_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "IN_TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "IN_NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	hr = _pd3dDevice->CreateInputLayout(layout, ARRAYSIZE(layout),
		program->getEntryBufferPointer(), program->getEntryBytecodeLength(),
		&_pInputLayout);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create Input Layout", "D3DObject", MB_OK);
		exit(1);
	}
}

void engine::Model::createObject(const UINT &sizeVertexArray, const FLOAT *vertexArray,
	const UINT &sizeIndexArray, const UINT *indexArray,
	const TCHAR *pathTexture,
	const XMFLOAT4 &ambient, const XMFLOAT4 &diffuse, const XMFLOAT4 &specular, const FLOAT &shininess)
{
	D3DObject *newone = new D3DObject(_pd3dDevice);
	ID3D11Texture2D *ptex;
	ID3D11ShaderResourceView *pshr;
	ID3D11SamplerState *psam;

	loadTextureFromFile(pathTexture, &ptex, &pshr, &psam, _pd3dDevice, _pContext);

	newone->setTexture(ptex, pshr, psam);
	newone->setAmbient(ambient, _pContext);
	newone->setDiffuse(diffuse, _pContext);
	newone->setSpecular(specular, _pContext);
	newone->setShininess(shininess, _pContext);
	newone->load(sizeVertexArray, vertexArray,
		sizeIndexArray, indexArray,
		_pd3dDevice);
  
	_tD3DObject->push_back(newone);
}

static std::string getDir(const TCHAR *file)
{
	UINT size, i;
	std::string path;

	for (size = i = 0; file[i] != '\0'; i++)
		if (file[i] == '/')
			size = i + 1;

	path.insert(0, file, 0, size);

	return path;
}

void engine::Model::loadFromFile(const TCHAR *szFileName)
{
	Assimp::Importer Importer;
	UINT i, j;

	if (_tD3DObject != NULL && isMirror == FALSE)
	{
		_tD3DObject->clear();
	}

	const aiScene *pScene = Importer.ReadFile(szFileName, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);
	if (!pScene)
	{
		std::string mes = "Failed to load File: ";
		mes.append(szFileName);
		MessageBox(NULL, mes.c_str(), "D3DObject", MB_OK);
		exit(1);
	}

	std::vector<Vertex> vertices;
	std::vector<UINT> indices;
	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);
	for (i = 0; i<pScene->mNumMeshes; i++)
	{
		//vertices.resize(pScene->mMeshes[i]->mNumVertices);
		for (j = 0; j<pScene->mMeshes[i]->mNumVertices; j++)
		{
			const aiVector3D *pPos = &(pScene->mMeshes[i]->mVertices[j]);
			const aiVector3D *pNormal = pScene->mMeshes[i]->HasNormals() ? &(pScene->mMeshes[i]->mNormals[j]) : &Zero3D;
			const aiVector3D *pTexCoord = pScene->mMeshes[i]->HasTextureCoords(0) ? &(pScene->mMeshes[i]->mTextureCoords[0][j]) : &Zero3D;

			Vertex v = 
			{
				XMFLOAT3(pPos->x, pPos->y, pPos->z), 
				XMFLOAT2(pTexCoord->x, pTexCoord->y), 
				XMFLOAT3(pNormal->x, pNormal->y, pNormal->z)
			};

			vertices.push_back(v);
		}

		//vertices.resize(pScene->mMeshes[i]->mNumFaces * 3);
		for (j = 0; j < pScene->mMeshes[i]->mNumFaces; j++)
		{
			indices.push_back(pScene->mMeshes[i]->mFaces[j].mIndices[0]);
			indices.push_back(pScene->mMeshes[i]->mFaces[j].mIndices[1]);
			indices.push_back(pScene->mMeshes[i]->mFaces[j].mIndices[2]);
		}

		aiString path;
		std::string fullPath;
		if (pScene->mMaterials[pScene->mMeshes[i]->mMaterialIndex]->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
			fullPath = getDir(szFileName) + path.data;
		else
			fullPath = "resources/none.png";

		aiColor4D mat_ambient;
		aiColor4D mat_diffuse;
		aiColor4D mat_specular;
		FLOAT mat_shininess;
		FLOAT opacity;

		pScene->mMaterials[pScene->mMeshes[i]->mMaterialIndex]->Get(AI_MATKEY_COLOR_AMBIENT, mat_ambient);
		pScene->mMaterials[pScene->mMeshes[i]->mMaterialIndex]->Get(AI_MATKEY_COLOR_DIFFUSE, mat_diffuse);
		pScene->mMaterials[pScene->mMeshes[i]->mMaterialIndex]->Get(AI_MATKEY_COLOR_SPECULAR, mat_specular);
		pScene->mMaterials[pScene->mMeshes[i]->mMaterialIndex]->Get(AI_MATKEY_SHININESS, mat_shininess);
		pScene->mMaterials[pScene->mMeshes[i]->mMaterialIndex]->Get(AI_MATKEY_OPACITY, opacity);
		mat_ambient.a = opacity;
		mat_diffuse.a = opacity;
		mat_specular.a = opacity;

		createObject(vertices.size() * sizeof(Vertex), (FLOAT *)&vertices[0],
			indices.size() * sizeof(UINT), &indices[0],
			fullPath.c_str(),
			XMFLOAT4(mat_ambient.r, mat_ambient.g, mat_ambient.b, mat_ambient.a), XMFLOAT4(mat_diffuse.r, mat_diffuse.g, mat_diffuse.b, mat_diffuse.a), XMFLOAT4(mat_specular.r, mat_specular.g, mat_specular.b, mat_specular.a),
			mat_shininess);

		vertices.clear();
		indices.clear();
	}
}

void engine::Model::sortD3DObject(void)
{
	qsort(&(*_tD3DObject)[0], _tD3DObject->size(), sizeof (*_tD3DObject)[0], comparD3DObject);
}

void engine::Model::matIdentity(void)
{
	*_ModelMatrix = XMMatrixTranspose(XMMatrixIdentity());
}

void engine::Model::matTranslate(const FLOAT &x, const FLOAT &y, const FLOAT &z)
{
	*_ModelMatrix *= XMMatrixTranspose(XMMatrixTranslation(x, y, z));
}

void engine::Model::matRotate(const FLOAT &angle, const FLOAT &x, const FLOAT &y, const FLOAT &z)
{
	*_ModelMatrix *= XMMatrixTranspose(XMMatrixRotationAxis(XMVectorSet(x, y, z, 1.0f), angle * ((FLOAT)XM_PI / 180)));
}

void engine::Model::matScale(const FLOAT &x, const FLOAT &y, const FLOAT &z)
{
	*_ModelMatrix *= XMMatrixTranspose(XMMatrixScaling(x, y, z));
}

XMFLOAT3 engine::Model::getPosition(void) const
{
	XMFLOAT3 tmp;
	tmp.x = XMVectorGetZ(_ModelMatrix->r[0]);
	tmp.y = XMVectorGetZ(_ModelMatrix->r[1]);
	tmp.z = XMVectorGetZ(_ModelMatrix->r[2]);

	return tmp;
}

engine::D3DObject *engine::Model::getD3DObject(UINT num) const
{
	if(num>=_tD3DObject->size())
	{
		MessageBox(NULL, "Bad num Object!", "Error", MB_OK);
		exit(1);
	}
	return (*_tD3DObject)[num];
}
  
void engine::Model::display(GBuffer *g, Camera *cam)
{
	UINT i;

	if (_program == NULL)
	{
		MessageBox(NULL, "Need to config the Model before displaying", "Model", MB_OK);
		exit(1);
	}
	if (g == NULL)
	{
		MessageBox(NULL, "Bad GBuffer", "Model", MB_OK);
		exit(1);
	}
	if (cam == NULL)
	{
		MessageBox(NULL, "Bad Camera", "Model", MB_OK);
		exit(1);
	}

	*_MVPMatrix = cam->getVPMatrix() * *_ModelMatrix;
	*_NormalMatrix = XMMatrixTranspose(XMMatrixInverse(NULL, *_ModelMatrix));

	g->getContext()->UpdateSubresource(_pMVPMatrixBuffer, 0, NULL, _MVPMatrix, 0, 0);
	g->getContext()->UpdateSubresource(_pNormalMatrixBuffer, 0, NULL, _NormalMatrix, 0, 0);

	g->getContext()->VSSetShader(_program->getVertexShader(), NULL, 0);
	g->getContext()->GSSetShader(_program->getGeometryShader(), NULL, 0);
	g->getContext()->PSSetShader(_program->getPixelShader(), NULL, 0);
	g->getContext()->IASetInputLayout(_pInputLayout);
	g->getContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	g->getContext()->VSSetConstantBuffers(0, 1, &_pMVPMatrixBuffer);
	g->getContext()->VSSetConstantBuffers(1, 1, &_pNormalMatrixBuffer);
  
	for (i = 0; i<_tD3DObject->size(); i++)
		if ((*_tD3DObject)[i]->getTransparency() == 1.0f)
			(*_tD3DObject)[i]->display(g->getContext());

	g->executeDeferredContext();
}