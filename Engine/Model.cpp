#include <Engine/Model.hpp>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

engine::Model::Model(void)
{
	_tObject = NULL;
	_pConstantBuffer0 = NULL;
	_pConstantBuffer1 = NULL;
	_matrix = (struct uniform0 *)_aligned_malloc(sizeof *_matrix, 16);
	_screen = (struct uniform1 *)_aligned_malloc(sizeof *_screen, 16);
	_program = NULL;

	matIdentity();
}

engine::Model::~Model(void)
{
	UINT i;
	if(_tObject != NULL && isMirror == FALSE)
	{
		for (i = 0; i < _tObject->size(); i++)
			delete (*_tObject)[i];
		delete _tObject;
	}

	if (_pConstantBuffer0)
		_pConstantBuffer0->Release();
	if (_pConstantBuffer1)
		_pConstantBuffer1->Release();

	_aligned_free(_matrix);
	_aligned_free(_screen);
}

void engine::Model::initObjectArray(void)
{
	UINT i;
	if(_tObject != NULL && isMirror == FALSE)
	{
		for (i = 0; i < _tObject->size(); i++)
			delete (*_tObject)[i];
		delete _tObject;
	}
	isMirror = FALSE;
	_tObject = new std::vector<D3DObject *>;
}

void engine::Model::initObjectMirror(Model *m)
{
	isMirror = TRUE;
	_tObject = m->_tObject;
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
	hr = pd3dDevice->CreateBuffer(&bd, NULL, &_pConstantBuffer0);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create Constant Buffer", "Model", MB_OK);
		return hr;
	}

	bd.ByteWidth = sizeof(*_screen) + (((sizeof(*_screen) % 16) == 0) ? 0 : (16 - (sizeof(*_screen) % 16)));
	hr = pd3dDevice->CreateBuffer(&bd, NULL, &_pConstantBuffer1);

	return hr;
}

HRESULT engine::Model::createObject(const UINT &sizeVertexArray, const FLOAT *vertexArray,
	const UINT &sizeIndexArray, const UINT *indexArray,
	const TCHAR *pathTexture,
	const FLOAT *ambient, const FLOAT *diffuse, const FLOAT *specular, const FLOAT *shininess,
	ID3D11Device *pd3dDevice)
{
	HRESULT hr;
	D3DObject *newone = new D3DObject;
	ID3D11ShaderResourceView *pshr;
	ID3D11SamplerState *psam;

	hr = loadTextureFromFile(pathTexture, &pshr, &psam, pd3dDevice);
	if (FAILED(hr))
	{
		std::string text = "Fail to load Texture: ";
		text.append(pathTexture);
		MessageBox(NULL, text.c_str(), "Error", MB_OK);
		return hr;
	}

	hr = newone->config(_program, pd3dDevice);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Fail to load ShaderProgram", "Error", MB_OK);
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
		MessageBox(NULL, "Fail to load an Object", "Error", MB_OK);
		return hr;
	}
  
	_tObject->push_back(newone);

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

HRESULT engine::Model::loadFromFile(const TCHAR *szFileName, ID3D11Device *pd3dDevice)
{
	Assimp::Importer Importer;
	UINT i, j;

	if (_tObject != NULL && isMirror == FALSE)
	{
		_tObject->clear();
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
			pd3dDevice)))
			return E_FAIL;

		vertices.clear();
		indices.clear();
	}

	return S_OK;
}

void engine::Model::sortObject(void)
{
	qsort(&(*_tObject)[0], _tObject->size(), sizeof (*_tObject)[0], comparD3DObject);
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
	if(num>=_tObject->size())
	{
		MessageBox(NULL, "Bad num Object!", "Error", MB_OK);
		exit(1);
	}
	return (*_tObject)[num];
}
  
void engine::Model::display(GBuffer *g, Camera *cam)// , LBuffer *l) const
{
	UINT i;
  
	if(_program == NULL)
	{
		MessageBox(NULL, "Load a program before!", "Error", MB_OK);
		exit(1);
	}
	if(cam == NULL)
	{
		MessageBox(NULL, "Bad Camera!", "Error", MB_OK);
		exit(1);
	}

	_matrix->MVP = *cam->getMatrix() * _matrix->modelMatrix;
	_matrix->normalMatrix = DirectX::XMMatrixInverse(NULL, _matrix->modelMatrix);

	_screen->screen[0] = (FLOAT)g->getWidth();
	_screen->screen[1] = (FLOAT)g->getHeight();
  
	g->getContext()->UpdateSubresource(_pConstantBuffer0, 0, NULL, _matrix, 0, 0);
	g->getContext()->VSSetConstantBuffers(0, 1, &_pConstantBuffer0);
	g->getContext()->UpdateSubresource(_pConstantBuffer1, 0, NULL, _screen, 0, 0);
	g->getContext()->PSSetConstantBuffers(0, 1, &_pConstantBuffer1);
  
	for(i=0 ; i<_tObject->size(); i++)
		(*_tObject)[i]->display(g);
}

/*void engine::Model::displayOnGBuffer(Camera *cam, GBuffer *g) const
{
	GLuint i;
	GLfloat tmp[16];
	
	if(cam == NULL)
	{
		std::cerr << "Bad Camera" << std::endl;
		return;
	}
	if(g == NULL)
	{
		std::cerr << "Bad GBuffer!" << std::endl;
		return;
	}
        
	glUseProgram(g->getProgramId());
	
	matrixMultiply(tmp, cam->getMatrix(), _modelMatrix);
	glUniformMatrix4fv(g->getMVPLocation(), 1, GL_FALSE, tmp);
	glUniformMatrix4fv(g->getModelMatrixLocation(), 1, GL_FALSE, _modelMatrix);
	matrixNormalFromModel(tmp, _modelMatrix);
	glUniformMatrix3fv(g->getNormalMatrixLocation(), 1, GL_FALSE, tmp);
	
	glUseProgram(0);
	
	for(i=0 ; i<_tObject->size(); i++)
		(*_tObject)[i]->displayOnGBuffer(g);
}

void engine::Model::displayShadow(Light *l) const
{
	GLuint i;
	GLfloat tmp[16];

	if(l == NULL)
	{
		std::cerr << "Bad Light!" << std::endl;
		return;
	}
	if(l->getShadowMap() == NULL)
	{
		std::cerr << "You need to config the ShadowMap before!" << std::endl;
		return;
	}
	
	glUseProgram(l->getShadowMap()->getProgramId());
	
	matrixMultiply(tmp, l->getMatrix(), _modelMatrix);
	glUniformMatrix4fv(l->getShadowMap()->getMVPLocation(), 1, GL_FALSE, tmp);
	
	glUseProgram(0);
	for(i=0 ; i<_tObject->size(); i++)
		(*_tObject)[i]->displayShadow(l);
}*/
