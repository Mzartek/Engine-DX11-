#include <Engine/Model.hpp>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

engine::Model::Model(void)
{
	_tD3DObject = NULL;
	_pMatrixBuffer = NULL;
	_matrix = (struct matrix *)_aligned_malloc(sizeof *_matrix, 16);
	_program = NULL;

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

	_aligned_free(_matrix);

	if (_pMatrixBuffer)
		_pMatrixBuffer->Release();
}

void engine::Model::initObjectArray(void)
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

void engine::Model::initObjectMirror(Model *m)
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

HRESULT engine::Model::config(ShaderProgram *program, ID3D11Device *pd3dDevice)
{
	HRESULT hr;

	_program = program;

	// Create uniform
	D3D11_BUFFER_DESC bd;
	bd.ByteWidth = sizeof(*_matrix) + (((sizeof(*_matrix) % 16) == 0) ? 0 : (16 - (sizeof(*_matrix) % 16)));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	hr = pd3dDevice->CreateBuffer(&bd, NULL, &_pMatrixBuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create Constant Buffer", "Model", MB_OK);
		return hr;
	}

	return hr;
}

HRESULT engine::Model::createObject(const UINT &sizeVertexArray, const FLOAT *vertexArray,
	const UINT &sizeIndexArray, const UINT *indexArray,
	const TCHAR *pathTexture,
	const FLOAT *ambient, const FLOAT *diffuse, const FLOAT *specular, const FLOAT *shininess,
	ID3D11Device *pd3dDevice, ID3D11DeviceContext *pContext)
{
	HRESULT hr;
	D3DObject *newone = new D3DObject;
	ID3D11ShaderResourceView *pshr;
	ID3D11SamplerState *psam;

	hr = loadTextureFromFile(pathTexture, &pshr, &psam, pd3dDevice, pContext);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Fail to load Texture", "Model", MB_OK);
		return hr;
	}

	hr = newone->config(_program, pd3dDevice);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Fail to load ShaderProgram", "Model", MB_OK);
		return hr;
	}

	newone->setTexture(pshr, psam);
	newone->setAmbient(ambient[0], ambient[1], ambient[2], ambient[3]);
	newone->setDiffuse(diffuse[0], diffuse[1], diffuse[2], diffuse[3]);
	newone->setSpecular(specular[0], specular[1], specular[2], specular[3]);
	newone->setShininess(shininess[0]);
	hr = newone->load(sizeVertexArray, vertexArray,
		sizeIndexArray, indexArray,
		pd3dDevice);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Fail to load an Object", "Model", MB_OK);
		return hr;
	}
  
	_tD3DObject->push_back(newone);

	return S_OK;
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

HRESULT engine::Model::loadFromFile(const TCHAR *szFileName, 
	ID3D11Device *pd3dDevice, ID3D11DeviceContext *pContext)
{
	Assimp::Importer Importer;
	UINT i, j;

	if (_tD3DObject != NULL && isMirror == FALSE)
	{
		_tD3DObject->clear();
	}

	const aiScene *pScene = Importer.ReadFile(szFileName, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);
	if (!pScene)
		return E_FAIL;

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
				DirectX::XMFLOAT3(pPos->x, pPos->y, pPos->z), 
				DirectX::XMFLOAT2(pTexCoord->x, pTexCoord->y), 
				DirectX::XMFLOAT3(pNormal->x, pNormal->y, pNormal->z)
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

		if (FAILED(createObject(vertices.size() * sizeof(Vertex), (FLOAT *)&vertices[0],
			indices.size() * sizeof(UINT), &indices[0],
			fullPath.c_str(),
			(FLOAT *)&mat_ambient, (FLOAT *)&mat_diffuse, (FLOAT *)&mat_specular,
			&mat_shininess,
			pd3dDevice, pContext)))
			return E_FAIL;

		vertices.clear();
		indices.clear();
	}

	return S_OK;
}

void engine::Model::sortObject(void)
{
	qsort(&(*_tD3DObject)[0], _tD3DObject->size(), sizeof (*_tD3DObject)[0], comparD3DObject);
}

void engine::Model::matIdentity(void)
{
	_matrix->modelMatrix = XMMatrixTranspose(DirectX::XMMatrixIdentity());
}

void engine::Model::matTranslate(const FLOAT &x, const FLOAT &y, const FLOAT &z)
{
	_matrix->modelMatrix *= XMMatrixTranspose(DirectX::XMMatrixTranslation(x, y, z));
}

void engine::Model::matRotate(const FLOAT &angle, const BOOL &x, const BOOL &y, const BOOL &z)
{
	if (x)
		_matrix->modelMatrix *= XMMatrixTranspose(DirectX::XMMatrixRotationX(angle*((FLOAT)DirectX::XM_PI / 180)));
	if (y)
		_matrix->modelMatrix *= XMMatrixTranspose(DirectX::XMMatrixRotationY(angle*((FLOAT)DirectX::XM_PI / 180)));
	if (z)
		_matrix->modelMatrix *= XMMatrixTranspose(DirectX::XMMatrixRotationZ(angle*((FLOAT)DirectX::XM_PI / 180)));
}

void engine::Model::matScale(const FLOAT &x, const FLOAT &y, const FLOAT &z)
{
	_matrix->modelMatrix *= XMMatrixTranspose(DirectX::XMMatrixScaling(x, y, z));
}

DirectX::XMFLOAT3 engine::Model::getPosition(void) const
{
	DirectX::XMFLOAT3 tmp;
	tmp.x = DirectX::XMVectorGetZ(_matrix->modelMatrix.r[0]);
	tmp.y = DirectX::XMVectorGetZ(_matrix->modelMatrix.r[1]);
	tmp.z = DirectX::XMVectorGetZ(_matrix->modelMatrix.r[2]);

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
  
	if(_program == NULL)
	{
		MessageBox(NULL, "Load a program before!", "Model", MB_OK);
		exit(1);
	}
	if(cam == NULL)
	{
		MessageBox(NULL, "Bad Camera!", "Model", MB_OK);
		exit(1);
	}

	_matrix->MVP = cam->getVPMatrix() * _matrix->modelMatrix;
	_matrix->normalMatrix = DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(NULL, _matrix->modelMatrix));

	// Shader
	g->getContext()->VSSetShader(_program->getVertexShader(), NULL, 0);
	g->getContext()->GSSetShader(_program->getGeometryShader(), NULL, 0);
	g->getContext()->PSSetShader(_program->getPixelShader(), NULL, 0);
  
	// Matrix Buffer
	g->getContext()->UpdateSubresource(_pMatrixBuffer, 0, NULL, _matrix, 0, 0);
	g->getContext()->VSSetConstantBuffers(0, 1, &_pMatrixBuffer);
  
	for (i = 0; i<_tD3DObject->size(); i++)
		if ((*_tD3DObject)[i]->getTransparency() == 1.0f)
			(*_tD3DObject)[i]->display(g);
}