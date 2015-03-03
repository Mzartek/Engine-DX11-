#include <Engine/Model.hpp>
#include <Engine/Mesh.hpp>
#include <Engine/Buffer.hpp>
#include <Engine/ShaderProgram.hpp>
#include <Engine/GBuffer.hpp>
#include <Engine/Camera.hpp>
#include <Engine/DirLight.hpp>
#include <Engine/SpotLight.hpp>
#include <Engine/ShadowMap.hpp>
#include <Engine/Texture.hpp>

#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

static struct
{
	XMMATRIX MVP;
	XMMATRIX projection;
	XMMATRIX view;
	XMMATRIX model;
	XMMATRIX normal;
} _matrix;

static struct
{
	XMVECTOR __declspec(align(16)) position;
	XMVECTOR __declspec(align(16)) target;
} _camera;

void Engine::Model::genMatModel(void) const
{
	*_modelMatrix = XMMatrixScaling(_scale->x, _scale->y, _scale->z) *
		XMMatrixRotationZ(_rotation->z) *
		XMMatrixRotationY(_rotation->y) *
		XMMatrixRotationX(_rotation->x) *
		XMMatrixTranslation(_position->x, _position->y, _position->z);
}

void Engine::Model::genMatNormal(void) const
{
	*_normalMatrix = XMMatrixTranspose(XMMatrixInverse(NULL, *_modelMatrix));
}

void Engine::Model::checkMatrix(void)
{
	if (_needMatModel == TRUE)
	{
		genMatModel();
		_needMatModel = FALSE;
	}
	if (_needMatNormal == TRUE)
	{
		genMatNormal();
		_needMatNormal = FALSE;
	}
}

void Engine::Model::deleteMesh(void)
{
	if (_tMesh != NULL && _isMirror != TRUE)
	{
		for (UINT i = 0; i < _tMesh->size(); i++)
			delete (*_tMesh)[i];
		delete _tMesh;
	}
}

Engine::Model::Model(ShaderProgram *gProgram, ShaderProgram *smProgram)
	: _isMirror(FALSE), _tMesh(NULL), _needMatModel(TRUE), _needMatNormal(TRUE), _cubeTexture(NULL), _gProgram(gProgram), _smProgram(smProgram)
{
	_tMesh = new std::vector<Mesh *>;
	_matrixBuffer = new Buffer;
	_cameraBuffer = new Buffer;
	_position = new XMFLOAT3;
	_rotation = new XMFLOAT3;
	_scale = new XMFLOAT3;
	_modelMatrix = (XMMATRIX *)_aligned_malloc(sizeof *_modelMatrix, 16);
	_normalMatrix = (XMMATRIX *)_aligned_malloc(sizeof *_normalMatrix, 16);

	XMStoreFloat3(_position, XMVectorSet(0, 0, 0, 1));
	XMStoreFloat3(_rotation, XMVectorSet(0, 0, 0, 1));
	XMStoreFloat3(_scale, XMVectorSet(1, 1, 1, 1));

	_matrixBuffer->createStore(D3D11_BIND_CONSTANT_BUFFER, NULL, sizeof _matrix, D3D11_USAGE_DYNAMIC);
	_cameraBuffer->createStore(D3D11_BIND_CONSTANT_BUFFER, NULL, sizeof _camera, D3D11_USAGE_DYNAMIC);

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "IN_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "IN_TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 3 * sizeof(FLOAT), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "IN_NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 5 * sizeof(FLOAT), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "IN_TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 8 * sizeof(FLOAT), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	Device->CreateInputLayout(layout, ARRAYSIZE(layout), gProgram->getEntryBufferPointer(), gProgram->getEntryBytecodeLength(), &_pInputLayout);
}

Engine::Model::Model(Model *model, ShaderProgram *gProgram, ShaderProgram *smProgram)
	: _isMirror(TRUE), _tMesh(NULL), _needMatModel(TRUE), _needMatNormal(TRUE), _cubeTexture(NULL), _gProgram(gProgram), _smProgram(smProgram)
{
	_tMesh = model->_tMesh;
	_matrixBuffer = new Buffer;
	_cameraBuffer = new Buffer;
	_position = new XMFLOAT3;
	_rotation = new XMFLOAT3;
	_scale = new XMFLOAT3;
	_modelMatrix = (XMMATRIX *)_aligned_malloc(sizeof *_modelMatrix, 16);
	_normalMatrix = (XMMATRIX *)_aligned_malloc(sizeof *_normalMatrix, 16);

	XMStoreFloat3(_position, XMVectorSet(0, 0, 0, 1));
	XMStoreFloat3(_rotation, XMVectorSet(0, 0, 0, 1));
	XMStoreFloat3(_scale, XMVectorSet(1, 1, 1, 1));

	_matrixBuffer->createStore(D3D11_BIND_CONSTANT_BUFFER, NULL, sizeof _matrix, D3D11_USAGE_DYNAMIC);
	_cameraBuffer->createStore(D3D11_BIND_CONSTANT_BUFFER, NULL, sizeof _camera, D3D11_USAGE_DYNAMIC);

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "IN_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "IN_TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 3 * sizeof(FLOAT), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "IN_NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 5 * sizeof(FLOAT), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "IN_TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 8 * sizeof(FLOAT), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	Device->CreateInputLayout(layout, ARRAYSIZE(layout), gProgram->getEntryBufferPointer(), gProgram->getEntryBytecodeLength(), &_pInputLayout);
}

Engine::Model::~Model(void)
{
	deleteMesh();

	delete _matrixBuffer;
	delete _cameraBuffer;
	delete _position;
	delete _rotation;
	delete _scale;
	_aligned_free(_modelMatrix);
	_aligned_free(_normalMatrix);

	_pInputLayout->Release();
}

void Engine::Model::addMesh(const UINT &numVertex, const Vertex *vertexArray,
	const UINT &numIndex, const UINT *indexArray,
	const CHAR *colorTexture, const CHAR *NMTexture,
	const XMVECTOR &ambient, const XMVECTOR &diffuse, const XMVECTOR &specular, const FLOAT &shininess)
{
	Mesh *newone = new Mesh;

	newone->setColorTexture(colorTexture);
	newone->setNMTexture(NMTexture);
	newone->setAmbient(ambient);
	newone->setDiffuse(diffuse);
	newone->setSpecular(specular);
	newone->setShininess(shininess);
	newone->load(numVertex, vertexArray,
		numIndex, indexArray);
  
	_tMesh->push_back(newone);
}

static std::string getDir(const CHAR *file)
{
	UINT size, i;
	std::string path;

	for (size = i = 0; file[i] != '\0'; i++)
		if (file[i] == '/')
			size = i + 1;

	path.insert(0, file, 0, size);

	return path;
}

void Engine::Model::loadFromFile(const CHAR *szFileName)
{
	Assimp::Importer Importer;
	UINT i, j;

	if (_isMirror == TRUE)
	{
		MessageBox(NULL, TEXT("Error Model configuration"), TEXT(__FILE__), MB_OK);
		exit(1);
	}

	const aiScene *pScene = Importer.ReadFile(szFileName, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices | aiProcess_OptimizeMeshes);
	if (!pScene)
	{
		std::string mes = "Failed to load File: ";
		mes.append(szFileName);
		MessageBoxA(NULL, mes.c_str(), __FILE__, MB_OK);
		exit(1);
	}

	Vertex tmpVertex;
	std::vector<Vertex> vertices;
	std::vector<UINT> indices;
	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);
	const aiVector3D *pPos;
	const aiVector3D *pTexCoord;
	const aiVector3D *pNormal;
	const aiVector3D *pTangent;
	for (i = 0; i<pScene->mNumMeshes; i++)
	{
		// Vertex Buffer
		for (j = 0; j<pScene->mMeshes[i]->mNumVertices; j++)
		{
			pPos = &(pScene->mMeshes[i]->mVertices[j]);
			pTexCoord = pScene->mMeshes[i]->HasTextureCoords(0) ? &(pScene->mMeshes[i]->mTextureCoords[0][j]) : &Zero3D;
			pNormal = pScene->mMeshes[i]->HasNormals() ? &(pScene->mMeshes[i]->mNormals[j]) : &Zero3D;
			pTangent = pScene->mMeshes[i]->HasTangentsAndBitangents() ? &(pScene->mMeshes[i]->mTangents[j]) : &Zero3D;

			tmpVertex.position.x = pPos->x,      tmpVertex.position.y = pPos->y,      tmpVertex.position.z = pPos->z;
			tmpVertex.texCoord.x = pTexCoord->x, tmpVertex.texCoord.y = pTexCoord->y;
			tmpVertex.normal.x = pNormal->x,     tmpVertex.normal.y = pNormal->y,     tmpVertex.normal.z = pNormal->z;
			tmpVertex.tangent.x = pTangent->x,   tmpVertex.tangent.y = pTangent->y,   tmpVertex.tangent.z = pTangent->z;

			vertices.push_back(tmpVertex);
		}

		// Index Buffer
		for (j = 0; j < pScene->mMeshes[i]->mNumFaces; j++)
		{
			indices.push_back(pScene->mMeshes[i]->mFaces[j].mIndices[0]);
			indices.push_back(pScene->mMeshes[i]->mFaces[j].mIndices[1]);
			indices.push_back(pScene->mMeshes[i]->mFaces[j].mIndices[2]);
		}

		aiString path;
		std::string dir, colorPath, NMPath;
		dir = getDir(szFileName);
		if (pScene->mMaterials[pScene->mMeshes[i]->mMaterialIndex]->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
			colorPath += dir + path.C_Str();
		else
			colorPath = "resources/none.png";
		if (pScene->mMaterials[pScene->mMeshes[i]->mMaterialIndex]->GetTexture(aiTextureType_NORMALS, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
			NMPath += dir + "NM_" + path.C_Str();
		else
			NMPath = "resources/NM_none.png";

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

		addMesh((UINT)vertices.size(), vertices.data(),
			(UINT)indices.size(), indices.data(),
			colorPath.c_str(), NMPath.c_str(),
			XMVectorSet(mat_ambient.r, mat_ambient.g, mat_ambient.b, mat_ambient.a), XMVectorSet(mat_diffuse.r, mat_diffuse.g, mat_diffuse.b, mat_diffuse.a), XMVectorSet(mat_specular.r, mat_specular.g, mat_specular.b, mat_specular.a),
			mat_shininess);

		vertices.clear();
		indices.clear();
	}
}

void Engine::Model::sortMesh(void)
{
	std::sort(_tMesh->begin(), _tMesh->end(), CompareMesh());
}

void Engine::Model::setPosition(const XMVECTOR &position)
{
	XMStoreFloat3(_position, position);
	_needMatModel = TRUE;
}

void Engine::Model::setRotation(const XMVECTOR &rotation)
{
	XMStoreFloat3(_rotation, rotation);
	_needMatModel = TRUE;
	_needMatNormal = TRUE;
}

void Engine::Model::setScale(const XMVECTOR &scale)
{
	XMStoreFloat3(_scale, scale);
	_needMatModel = TRUE;
	_needMatNormal = TRUE;
}

void Engine::Model::setCubeTexture(Texture *cubeTex)
{
	_cubeTexture = cubeTex;
}

XMVECTOR Engine::Model::getPosition(void) const
{
	return XMLoadFloat3(_position);
}

XMVECTOR Engine::Model::getRotation(void) const
{
	return XMLoadFloat3(_rotation);
}

XMVECTOR Engine::Model::getScale(void) const
{
	return XMLoadFloat3(_scale);
}

std::vector<Engine::Mesh *> Engine::Model::getMeshVector(void) const
{
	return *_tMesh;
}
  
void Engine::Model::display(GBuffer *gbuf, Camera *cam)
{
	checkMatrix();

	gbuf->setGeometryState();

	DeviceContext->VSSetShader(_gProgram->getVertexShader(), NULL, 0);
	DeviceContext->HSSetShader(_gProgram->getHullShader(), NULL, 0);
	DeviceContext->DSSetShader(_gProgram->getDomainShader(), NULL, 0);
	DeviceContext->GSSetShader(_gProgram->getGeometryShader(), NULL, 0);
	DeviceContext->PSSetShader(_gProgram->getPixelShader(), NULL, 0);

	_matrix.MVP = *_modelMatrix * cam->getVPMatrix();
	_matrix.projection = cam->getProjectionMatrix();
	_matrix.view = cam->getViewMatrix();
	_matrix.model = *_modelMatrix;
	_matrix.normal = *_normalMatrix;
	_matrixBuffer->updateStoreMap(&_matrix);

	_camera.position = cam->getCameraPosition();
	_camera.target = cam->getTargetPosition();
	_cameraBuffer->updateStoreMap(&_camera);

	ID3D11Buffer *buf[] =
	{
		_matrixBuffer->getBuffer(),
		_cameraBuffer->getBuffer(),
	};
	DeviceContext->VSSetConstantBuffers(0, ARRAYSIZE(buf), buf);

	DeviceContext->IASetInputLayout(_pInputLayout);
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	for (UINT i = 0; i<_tMesh->size(); i++)
		if ((*_tMesh)[i]->getTransparency() == 1.0f)
		{
			if (_cubeTexture)
				(*_tMesh)[i]->display(_cubeTexture);
			else
				(*_tMesh)[i]->display();
		}
}

void Engine::Model::displayTransparent(GBuffer *gbuf, Camera *cam)
{
	checkMatrix();

	gbuf->setGeometryState();

	DeviceContext->VSSetShader(_gProgram->getVertexShader(), NULL, 0);
	DeviceContext->HSSetShader(_gProgram->getHullShader(), NULL, 0);
	DeviceContext->DSSetShader(_gProgram->getDomainShader(), NULL, 0);
	DeviceContext->GSSetShader(_gProgram->getGeometryShader(), NULL, 0);
	DeviceContext->PSSetShader(_gProgram->getPixelShader(), NULL, 0);

	_matrix.MVP = *_modelMatrix * cam->getVPMatrix();
	_matrix.projection = cam->getProjectionMatrix();
	_matrix.view = cam->getViewMatrix();
	_matrix.model = *_modelMatrix;
	_matrix.normal = *_normalMatrix;
	_matrixBuffer->updateStoreMap(&_matrix);

	_camera.position = cam->getCameraPosition();
	_camera.target = cam->getTargetPosition();
	_cameraBuffer->updateStoreMap(&_camera);

	ID3D11Buffer *buf[] =
	{
		_matrixBuffer->getBuffer(),
		_cameraBuffer->getBuffer(),
	};
	DeviceContext->VSSetConstantBuffers(0, ARRAYSIZE(buf), buf);

	DeviceContext->IASetInputLayout(_pInputLayout);
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	for (UINT i = 0; i<_tMesh->size(); i++)
		if ((*_tMesh)[i]->getTransparency() != 1.0f)
		{
			if (_cubeTexture)
				(*_tMesh)[i]->display(_cubeTexture);
			else
				(*_tMesh)[i]->display();
		}
}

void Engine::Model::displayShadowMap(DirLight *light)
{
	checkMatrix();

	DeviceContext->VSSetShader(_smProgram->getVertexShader(), NULL, 0);
	DeviceContext->HSSetShader(_gProgram->getHullShader(), NULL, 0);
	DeviceContext->DSSetShader(_gProgram->getDomainShader(), NULL, 0);
	DeviceContext->GSSetShader(_smProgram->getGeometryShader(), NULL, 0);
	DeviceContext->PSSetShader(_smProgram->getPixelShader(), NULL, 0);

	DeviceContext->IASetInputLayout(_pInputLayout);
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	_matrix.model = *_modelMatrix;
	_matrix.normal = *_normalMatrix;

	for (UINT i = 0; i < CSM_NUM; i++)
	{
		light->getShadowMap(i)->setState();

		_matrix.MVP = *_modelMatrix * light->getVPMatrix(i);
		_matrix.projection = light->getProjectionMatrix(i);
		_matrix.view = light->getViewMatrix(i);
		_matrixBuffer->updateStoreMap(&_matrix);

		ID3D11Buffer *buf[] =
		{
			_matrixBuffer->getBuffer(),
		};
		DeviceContext->VSSetConstantBuffers(0, ARRAYSIZE(buf), buf);

		for (UINT j = 0; j < _tMesh->size(); j++)
			if ((*_tMesh)[j]->getTransparency() == 1.0f)
				(*_tMesh)[j]->displayShadow();
	}
}

void Engine::Model::displayShadowMap(SpotLight *light)
{
	checkMatrix();

	light->getShadowMap()->setState();

	DeviceContext->VSSetShader(_smProgram->getVertexShader(), NULL, 0);
	DeviceContext->HSSetShader(_gProgram->getHullShader(), NULL, 0);
	DeviceContext->DSSetShader(_gProgram->getDomainShader(), NULL, 0);
	DeviceContext->GSSetShader(_smProgram->getGeometryShader(), NULL, 0);
	DeviceContext->PSSetShader(_smProgram->getPixelShader(), NULL, 0);

	_matrix.MVP = *_modelMatrix * light->getVPMatrix();
	_matrix.projection = light->getProjectionMatrix();
	_matrix.view = light->getViewMatrix();
	_matrix.model = *_modelMatrix;
	_matrix.normal = *_normalMatrix;
	_matrixBuffer->updateStoreMap(&_matrix);

	ID3D11Buffer *buf[] =
	{
		_matrixBuffer->getBuffer(),
	};
	DeviceContext->VSSetConstantBuffers(0, ARRAYSIZE(buf), buf);

	DeviceContext->IASetInputLayout(_pInputLayout);
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	for (UINT i = 0; i<_tMesh->size(); i++)
		if ((*_tMesh)[i]->getTransparency() == 1.0f)
			(*_tMesh)[i]->displayShadow();
}