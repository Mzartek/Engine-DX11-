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

extern ID3D11Device1 *Device;
extern ID3D11DeviceContext1 *DeviceContext;

Engine::Model::Model(ShaderProgram *gProgram, ShaderProgram *smProgram)
	: _tMesh(NULL)
{
	_MVPMatrixBuffer = new Buffer;
	_normalMatrixBuffer = new Buffer;
	_ModelMatrix = (XMMATRIX *)_aligned_malloc(sizeof *_ModelMatrix, 16);

	_MVPMatrixBuffer->createStore(D3D11_BIND_CONSTANT_BUFFER, NULL, sizeof XMMATRIX, D3D11_USAGE_DYNAMIC);
	_normalMatrixBuffer->createStore(D3D11_BIND_CONSTANT_BUFFER, NULL, sizeof XMMATRIX, D3D11_USAGE_DYNAMIC);
	matIdentity();

	_gProgram = gProgram;
	_smProgram = smProgram;

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
	UINT i;
	if (_tMesh != NULL && isMirror != TRUE)
	{
		for (i = 0; i < _tMesh->size(); i++)
			delete (*_tMesh)[i];
		delete _tMesh;
	}
	delete _MVPMatrixBuffer;
	delete _normalMatrixBuffer;
	_aligned_free(_ModelMatrix);
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

void Engine::Model::addMesh(const UINT &sizeVertexArray, const FLOAT *vertexArray,
	const UINT &sizeIndexArray, const UINT *indexArray,
	const CHAR *colorTexture, const CHAR *NMTexture,
	const XMFLOAT4 &ambient, const XMFLOAT4 &diffuse, const XMFLOAT4 &specular, const FLOAT &shininess)
{
	Mesh *newone = new Mesh;

	newone->setColorTexture(colorTexture);
	newone->setNMTexture(NMTexture);
	newone->setAmbient(ambient);
	newone->setDiffuse(diffuse);
	newone->setSpecular(specular);
	newone->setShininess(shininess);
	newone->load(sizeVertexArray, vertexArray,
		sizeIndexArray, indexArray);
  
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

	std::vector<FLOAT> vertices;
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

			vertices.push_back(pPos->x), vertices.push_back(pPos->y), vertices.push_back(pPos->z);
			vertices.push_back(pTexCoord->x), vertices.push_back(pTexCoord->y);
			vertices.push_back(pNormal->x), vertices.push_back(pNormal->y), vertices.push_back(pNormal->z);
			vertices.push_back(pTangent->x), vertices.push_back(pTangent->y), vertices.push_back(pTangent->z);
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

		addMesh(vertices.size() * sizeof(FLOAT), &vertices[0],
			indices.size() * sizeof(UINT), &indices[0],
			colorPath.c_str(), NMPath.c_str(),
			XMFLOAT4(mat_ambient.r, mat_ambient.g, mat_ambient.b, mat_ambient.a), XMFLOAT4(mat_diffuse.r, mat_diffuse.g, mat_diffuse.b, mat_diffuse.a), XMFLOAT4(mat_specular.r, mat_specular.g, mat_specular.b, mat_specular.a),
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
	*_ModelMatrix = XMMatrixRotationAxis(XMVectorSet(x, y, z, 1.0f), angle * ((FLOAT)XM_PI / 180)) * *_ModelMatrix;
}

void Engine::Model::matScale(const FLOAT &x, const FLOAT &y, const FLOAT &z)
{
	*_ModelMatrix = XMMatrixScaling(x, y, z) * *_ModelMatrix;
}

XMFLOAT3 Engine::Model::getPosition(void) const
{
	XMFLOAT3 tmp;
	tmp.x = XMVectorGetW(_ModelMatrix->r[0]);
	tmp.y = XMVectorGetW(_ModelMatrix->r[1]);
	tmp.z = XMVectorGetW(_ModelMatrix->r[2]);

	return tmp;
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
	XMMATRIX MVPMatrix = *_ModelMatrix * cam->getVPMatrix();
	XMMATRIX NormalMatrix = XMMatrixTranspose(XMMatrixInverse(NULL, *_ModelMatrix));

	gbuf->setGeometryState();

	DeviceContext->VSSetShader(_gProgram->getVertexShader(), NULL, 0);
	DeviceContext->HSSetShader(_gProgram->getHullShader(), NULL, 0);
	DeviceContext->DSSetShader(_gProgram->getDomainShader(), NULL, 0);
	DeviceContext->GSSetShader(_gProgram->getGeometryShader(), NULL, 0);
	DeviceContext->PSSetShader(_gProgram->getPixelShader(), NULL, 0);

	_MVPMatrixBuffer->updateStoreMap(&MVPMatrix);
	_normalMatrixBuffer->updateStoreMap(&NormalMatrix);

	ID3D11Buffer *buf[] =
	{
		_MVPMatrixBuffer->getBuffer(),
		_normalMatrixBuffer->getBuffer(),
	};
	DeviceContext->VSSetConstantBuffers(0, ARRAYSIZE(buf), buf);

	DeviceContext->IASetInputLayout(_pInputLayout);
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	for (i = 0; i<_tMesh->size(); i++)
		if ((*_tMesh)[i]->getTransparency() == 1.0f)
			(*_tMesh)[i]->display();
}

void Engine::Model::displayTransparent(GBuffer *gbuf, Camera *cam) const
{
	UINT i;
	XMMATRIX MVPMatrix = *_ModelMatrix * cam->getVPMatrix();
	XMMATRIX NormalMatrix = XMMatrixTranspose(XMMatrixInverse(NULL, *_ModelMatrix));

	gbuf->setGeometryState();

	DeviceContext->VSSetShader(_gProgram->getVertexShader(), NULL, 0);
	DeviceContext->HSSetShader(_gProgram->getHullShader(), NULL, 0);
	DeviceContext->DSSetShader(_gProgram->getDomainShader(), NULL, 0);
	DeviceContext->GSSetShader(_gProgram->getGeometryShader(), NULL, 0);
	DeviceContext->PSSetShader(_gProgram->getPixelShader(), NULL, 0);

	_MVPMatrixBuffer->updateStoreMap(&MVPMatrix);
	_normalMatrixBuffer->updateStoreMap(&NormalMatrix);

	ID3D11Buffer *buf[] =
	{
		_MVPMatrixBuffer->getBuffer(),
		_normalMatrixBuffer->getBuffer(),
	};
	DeviceContext->VSSetConstantBuffers(0, ARRAYSIZE(buf), buf);

	DeviceContext->IASetInputLayout(_pInputLayout);
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	for (i = 0; i<_tMesh->size(); i++)
		if ((*_tMesh)[i]->getTransparency() != 1.0f)
			(*_tMesh)[i]->display();
}

void Engine::Model::displayShadowMap(Light *light) const
{
	UINT i;
	XMMATRIX MVPMatrix = *_ModelMatrix * light->getVPMatrix();

	light->getShadowMap()->setState();

	DeviceContext->VSSetShader(_smProgram->getVertexShader(), NULL, 0);
	DeviceContext->HSSetShader(_gProgram->getHullShader(), NULL, 0);
	DeviceContext->DSSetShader(_gProgram->getDomainShader(), NULL, 0);
	DeviceContext->GSSetShader(_smProgram->getGeometryShader(), NULL, 0);
	DeviceContext->PSSetShader(_smProgram->getPixelShader(), NULL, 0);

	_MVPMatrixBuffer->updateStoreMap(&MVPMatrix);

	ID3D11Buffer *buf[] =
	{
		_MVPMatrixBuffer->getBuffer(),
	};
	DeviceContext->VSSetConstantBuffers(0, ARRAYSIZE(buf), buf);

	DeviceContext->IASetInputLayout(_pInputLayout);
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	for (i = 0; i<_tMesh->size(); i++)
		if ((*_tMesh)[i]->getTransparency() == 1.0f)
			(*_tMesh)[i]->displayShadow();
}