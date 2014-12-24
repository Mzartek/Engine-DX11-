#include <Engine/Model.hpp>
#include <Engine/Mesh.hpp>
#include <Engine/Buffer.hpp>
#include <Engine/ShaderProgram.hpp>
#include <Engine/GBuffer.hpp>
#include <Engine/Camera.hpp>
#include <Engine/Light.hpp>
#include <Engine/ShadowMap.hpp>

#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

Engine::Model::Model(const EngineDevice &EngineDevice, ShaderProgram *gProgram, ShaderProgram *smProgram)
	: _EngineDevice(EngineDevice), _tMesh(NULL), _gProgram(gProgram), _smProgram(smProgram)
{
	_matrixBuffer = new Buffer(_EngineDevice);
	_cameraBuffer = new Buffer(_EngineDevice);
	_ModelMatrix = (XMMATRIX *)_aligned_malloc(sizeof *_ModelMatrix, 16);
	_NormalMatrix = (XMMATRIX *)_aligned_malloc(sizeof *_NormalMatrix, 16);

	_matrixBuffer->createStore(D3D11_BIND_CONSTANT_BUFFER, NULL, 5 * sizeof XMMATRIX, D3D11_USAGE_DYNAMIC);
	_cameraBuffer->createStore(D3D11_BIND_CONSTANT_BUFFER, NULL, 2 * sizeof XMVECTOR, D3D11_USAGE_DYNAMIC);

	matIdentity();

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "IN_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "IN_TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 3 * sizeof(FLOAT), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "IN_NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 5 * sizeof(FLOAT), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "IN_TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 8 * sizeof(FLOAT), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	_EngineDevice.Device->CreateInputLayout(layout, ARRAYSIZE(layout), gProgram->getEntryBufferPointer(), gProgram->getEntryBytecodeLength(), &_pInputLayout);
}

Engine::Model::~Model(void)
{
	UINT i;
	if (_tMesh != NULL && isMirror != TRUE)
	{
		for (i = 0; i < _tMesh->size(); i++)
			delete (*_tMesh)[i];
		delete _tMesh;
	}
	delete _matrixBuffer;
	delete _cameraBuffer;
	_aligned_free(_ModelMatrix);
	_aligned_free(_NormalMatrix);
	_pInputLayout->Release();
}

void Engine::Model::initMeshArray(void)
{
	UINT i;
	if (_tMesh != NULL && isMirror != TRUE)
	{
		for (i = 0; i < _tMesh->size(); i++)
			delete (*_tMesh)[i];
		delete _tMesh;
	}
	isMirror = FALSE;
	_tMesh = new std::vector<Mesh *>;
}

void Engine::Model::initMeshMirror(Model *m)
{
	UINT i;
	if (_tMesh != NULL && isMirror != TRUE)
	{
		for (i = 0; i < _tMesh->size(); i++)
			delete (*_tMesh)[i];
		delete _tMesh;
	}
	isMirror = TRUE;
	_tMesh = m->_tMesh;
}

void Engine::Model::addMesh(const UINT &numVertex, const Vertex *vertexArray,
	const UINT &numIndex, const UINT *indexArray,
	const CHAR *colorTexture, const CHAR *NMTexture,
	const XMVECTOR &ambient, const XMVECTOR &diffuse, const XMVECTOR &specular, const FLOAT &shininess)
{
	Mesh *newone = new Mesh(_EngineDevice);

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

	if (_tMesh == NULL || isMirror == TRUE)
	{
		MessageBox(NULL, TEXT("Error Model configuration"), TEXT(__FILE__), MB_OK);
		exit(1);
	}
	for (i = 0; i<_tMesh->size(); i++)
		delete (*_tMesh)[i];
	_tMesh->clear();

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

		addMesh(vertices.size(), vertices.data(),
			indices.size(), indices.data(),
			colorPath.c_str(), NMPath.c_str(),
			XMVectorSet(mat_ambient.r, mat_ambient.g, mat_ambient.b, mat_ambient.a), XMVectorSet(mat_diffuse.r, mat_diffuse.g, mat_diffuse.b, mat_diffuse.a), XMVectorSet(mat_specular.r, mat_specular.g, mat_specular.b, mat_specular.a),
			mat_shininess);

		vertices.clear();
		indices.clear();
	}
}

void Engine::Model::sortMesh(void)
{
	qsort(&(*_tMesh)[0], _tMesh->size(), sizeof(*_tMesh)[0], comparMesh);
}

void Engine::Model::matIdentity(void)
{
	*_ModelMatrix = XMMatrixIdentity();
}

void Engine::Model::matTranslate(const FLOAT &x, const FLOAT &y, const FLOAT &z)
{
	*_ModelMatrix = XMMatrixTranslation(x, y, z) * *_ModelMatrix;
}

void Engine::Model::matRotate(const FLOAT &angle, const FLOAT &x, const FLOAT &y, const FLOAT &z)
{
	*_ModelMatrix = XMMatrixRotationAxis(XMVectorSet(x, y, z, 1.0f), angle) * *_ModelMatrix;
}

void Engine::Model::matScale(const FLOAT &x, const FLOAT &y, const FLOAT &z)
{
	*_ModelMatrix = XMMatrixScaling(x, y, z) * *_ModelMatrix;
}

void Engine::Model::genMatNormal(void)
{
	*_NormalMatrix = XMMatrixTranspose(XMMatrixInverse(NULL, *_ModelMatrix));
}

XMVECTOR Engine::Model::getPosition(void) const
{
	return XMVectorSet(XMVectorGetW(_ModelMatrix->r[0]), XMVectorGetW(_ModelMatrix->r[1]), XMVectorGetW(_ModelMatrix->r[2]), XMVectorGetW(_ModelMatrix->r[3]));
}

Engine::Mesh *Engine::Model::getMesh(UINT num) const
{
	if (num >= _tMesh->size())
	{
		MessageBox(NULL, TEXT("Bad num Object!"), TEXT(__FILE__), MB_OK);
		exit(1);
	}
	return (*_tMesh)[num];
}
  
void Engine::Model::display(GBuffer *gbuf, Camera *cam) const
{
	UINT i;

	gbuf->setGeometryState();

	_EngineDevice.DeviceContext->VSSetShader(_gProgram->getVertexShader(), NULL, 0);
	_EngineDevice.DeviceContext->HSSetShader(_gProgram->getHullShader(), NULL, 0);
	_EngineDevice.DeviceContext->DSSetShader(_gProgram->getDomainShader(), NULL, 0);
	_EngineDevice.DeviceContext->GSSetShader(_gProgram->getGeometryShader(), NULL, 0);
	_EngineDevice.DeviceContext->PSSetShader(_gProgram->getPixelShader(), NULL, 0);

	struct
	{
		XMMATRIX MVP;
		XMMATRIX projection;
		XMMATRIX view;
		XMMATRIX model;
		XMMATRIX normal;
	} matrix;
	matrix.MVP = *_ModelMatrix * cam->getVPMatrix();
	matrix.projection = cam->getProjectionMatrix();
	matrix.view = cam->getViewMatrix();
	matrix.model = *_ModelMatrix;
	matrix.normal = *_NormalMatrix;
	_matrixBuffer->updateStoreMap(&matrix);

	struct
	{
		XMVECTOR position;
		XMVECTOR target;
	} camera;
	camera.position = cam->getCameraPosition();
	camera.target = cam->getTargetPosition();
	_cameraBuffer->updateStoreMap(&camera);

	ID3D11Buffer *buf[] =
	{
		_matrixBuffer->getBuffer(),
		_cameraBuffer->getBuffer(),
	};
	_EngineDevice.DeviceContext->VSSetConstantBuffers(0, ARRAYSIZE(buf), buf);

	_EngineDevice.DeviceContext->IASetInputLayout(_pInputLayout);
	_EngineDevice.DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	for (i = 0; i<_tMesh->size(); i++)
		if ((*_tMesh)[i]->getTransparency() == 1.0f)
			(*_tMesh)[i]->display();
}

void Engine::Model::displayTransparent(GBuffer *gbuf, Camera *cam) const
{
	UINT i;

	gbuf->setGeometryState();

	_EngineDevice.DeviceContext->VSSetShader(_gProgram->getVertexShader(), NULL, 0);
	_EngineDevice.DeviceContext->HSSetShader(_gProgram->getHullShader(), NULL, 0);
	_EngineDevice.DeviceContext->DSSetShader(_gProgram->getDomainShader(), NULL, 0);
	_EngineDevice.DeviceContext->GSSetShader(_gProgram->getGeometryShader(), NULL, 0);
	_EngineDevice.DeviceContext->PSSetShader(_gProgram->getPixelShader(), NULL, 0);

	struct
	{
		XMMATRIX MVP;
		XMMATRIX projection;
		XMMATRIX view;
		XMMATRIX model;
		XMMATRIX normal;
	} matrix;
	matrix.MVP = *_ModelMatrix * cam->getVPMatrix();
	matrix.projection = cam->getProjectionMatrix();
	matrix.view = cam->getViewMatrix();
	matrix.model = *_ModelMatrix;
	matrix.normal = *_NormalMatrix;
	_matrixBuffer->updateStoreMap(&matrix);

	struct
	{
		XMVECTOR position;
		XMVECTOR target;
	} camera;
	camera.position = cam->getCameraPosition();
	camera.target = cam->getTargetPosition();
	_cameraBuffer->updateStoreMap(&camera);

	ID3D11Buffer *buf[] =
	{
		_matrixBuffer->getBuffer(),
		_cameraBuffer->getBuffer(),
	};
	_EngineDevice.DeviceContext->VSSetConstantBuffers(0, ARRAYSIZE(buf), buf);

	_EngineDevice.DeviceContext->IASetInputLayout(_pInputLayout);
	_EngineDevice.DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	for (i = 0; i<_tMesh->size(); i++)
		if ((*_tMesh)[i]->getTransparency() != 1.0f)
			(*_tMesh)[i]->display();
}

void Engine::Model::displayShadowMap(Light *light) const
{
	UINT i;

	light->getShadowMap()->setState();

	_EngineDevice.DeviceContext->VSSetShader(_smProgram->getVertexShader(), NULL, 0);
	_EngineDevice.DeviceContext->HSSetShader(_gProgram->getHullShader(), NULL, 0);
	_EngineDevice.DeviceContext->DSSetShader(_gProgram->getDomainShader(), NULL, 0);
	_EngineDevice.DeviceContext->GSSetShader(_smProgram->getGeometryShader(), NULL, 0);
	_EngineDevice.DeviceContext->PSSetShader(_smProgram->getPixelShader(), NULL, 0);

	struct
	{
		XMMATRIX MVP;
		XMMATRIX projection;
		XMMATRIX view;
		XMMATRIX model;
		XMMATRIX normal;
	} matrix;
	matrix.MVP = *_ModelMatrix * light->getVPMatrix();
	matrix.projection = light->getProjectionMatrix();
	matrix.view = light->getViewMatrix();
	matrix.model = *_ModelMatrix;
	matrix.normal = *_NormalMatrix;
	_matrixBuffer->updateStoreMap(&matrix);

	ID3D11Buffer *buf[] =
	{
		_matrixBuffer->getBuffer(),
	};
	_EngineDevice.DeviceContext->VSSetConstantBuffers(0, ARRAYSIZE(buf), buf);

	_EngineDevice.DeviceContext->IASetInputLayout(_pInputLayout);
	_EngineDevice.DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	for (i = 0; i<_tMesh->size(); i++)
		if ((*_tMesh)[i]->getTransparency() == 1.0f)
			(*_tMesh)[i]->displayShadow();
}