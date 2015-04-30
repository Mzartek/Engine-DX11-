#include <Engine/Model.hpp>
#include <Engine/Mesh.hpp>
#include <Engine/Buffer.hpp>
#include <Engine/ShaderProgram.hpp>
#include <Engine/GBuffer.hpp>
#include <Engine/PerspCamera.hpp>
#include <Engine/DirLight.hpp>
#include <Engine/SpotLight.hpp>
#include <Engine/DepthMap.hpp>
#include <Engine/Texture2D.hpp>
#include <Engine/TextureCube.hpp>
#include <Engine/Material.hpp>

#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

inline std::string getDir(const CHAR *file)
{
	UINT size, i;
	std::string path;

	for (size = i = 0; file[i] != '\0'; i++)
		if (file[i] == '/')
			size = i + 1;

	path.insert(0, file, 0, size);

	return path;
}

void Engine::Model::genMatModel(void) const
{
	*_modelMatrix = 
		XMMatrixScaling(_scale->x, _scale->y, _scale->z) * 
		XMMatrixRotationQuaternion(*_rotation) * 
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

Engine::Model::Model(ShaderProgram *gProgram, ShaderProgram *smProgram)
	: _isMirror(FALSE), _needMatModel(TRUE), _needMatNormal(TRUE), _cubeTexture(NULL), _gProgram(gProgram), _smProgram(smProgram)
{
	_tMesh = new std::vector < Mesh * > ;
	_tObject = new std::vector < Object * > ;
	_matrixBuffer = new Buffer;
	_cameraBuffer = new Buffer;
	_position = new XMFLOAT3;
	_scale = new XMFLOAT3;
	_rotation = (XMVECTOR *)_aligned_malloc(sizeof *_rotation, 16);
	_modelMatrix = (XMMATRIX *)_aligned_malloc(sizeof *_modelMatrix, 16);
	_normalMatrix = (XMMATRIX *)_aligned_malloc(sizeof *_normalMatrix, 16);

	XMStoreFloat3(_position, XMVectorSet(0, 0, 0, 0));
	XMStoreFloat3(_scale, XMVectorSet(1, 1, 1, 0));
	*_rotation = XMQuaternionIdentity();

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
	: _isMirror(TRUE), _needMatModel(TRUE), _needMatNormal(TRUE), _cubeTexture(NULL), _gProgram(gProgram), _smProgram(smProgram)
{
	_tMesh = model->_tMesh;
	_tObject = new std::vector < Object * >;
	_matrixBuffer = new Buffer;
	_cameraBuffer = new Buffer;
	_position = new XMFLOAT3;
	_scale = new XMFLOAT3;
	_rotation = (XMVECTOR *)_aligned_malloc(sizeof *_rotation, 16);
	_modelMatrix = (XMMATRIX *)_aligned_malloc(sizeof *_modelMatrix, 16);
	_normalMatrix = (XMMATRIX *)_aligned_malloc(sizeof *_normalMatrix, 16);

	XMStoreFloat3(_position, XMVectorSet(0, 0, 0, 0));
	XMStoreFloat3(_scale, XMVectorSet(1, 1, 1, 0));
	*_rotation = XMQuaternionIdentity();

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
	if (!_isMirror) delete _tMesh;

	for (std::vector<Object *>::iterator it = _tObject->begin(); it != _tObject->end(); it++)
		delete *it;
	delete _tObject;

	delete _matrixBuffer;
	delete _cameraBuffer;
	delete _position;
	delete _scale;
	_aligned_free(_rotation);
	_aligned_free(_modelMatrix);
	_aligned_free(_normalMatrix);

	_pInputLayout->Release();
}

void Engine::Model::addMesh(Mesh *mesh)
{
	if (_isMirror == TRUE)
	{
		MessageBox(NULL, TEXT("Error Model configuration"), TEXT(__FILE__), MB_OK);
		abort();
	}

	_tMesh->push_back(mesh);
}

void Engine::Model::loadFromFile(const CHAR *szFileName)
{
	if (_isMirror == TRUE)
	{
		MessageBox(NULL, TEXT("Error Model configuration"), TEXT(__FILE__), MB_OK);
		abort();
	}

	Assimp::Importer Importer;
	const aiScene *pScene = Importer.ReadFile(szFileName, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices | aiProcess_OptimizeMeshes);
	if (!pScene)
	{
		std::string mes = "Failed to load File: ";
		mes.append(szFileName);
		MessageBoxA(NULL, mes.c_str(), __FILE__, MB_OK);
		abort();
	}

	std::vector<Vertex> vertices;
	std::vector<UINT> indices;
	for (UINT i = 0; i<pScene->mNumMeshes; i++)
	{
		// Vertex Buffer
		for (UINT j = 0; j<pScene->mMeshes[i]->mNumVertices; j++)
		{
			const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);
			const aiVector3D pPos = pScene->mMeshes[i]->mVertices[j];
			const aiVector3D pTexCoord = pScene->mMeshes[i]->HasTextureCoords(0) ? pScene->mMeshes[i]->mTextureCoords[0][j] : Zero3D;
			const aiVector3D pNormal = pScene->mMeshes[i]->HasNormals() ? pScene->mMeshes[i]->mNormals[j] : Zero3D;
			const aiVector3D pTangent = pScene->mMeshes[i]->HasTangentsAndBitangents() ? pScene->mMeshes[i]->mTangents[j] : Zero3D;

			Vertex newVertex = {
				{ pPos.x, pPos.y, pPos.z },
				{ pTexCoord.x, pTexCoord.y },
				{ pNormal.x, pNormal.y, pNormal.z },
				{ pTangent.x, pTangent.y, pTangent.z}
			};

			vertices.push_back(newVertex);
		}

		// Index Buffer
		for (UINT j = 0; j < pScene->mMeshes[i]->mNumFaces; j++)
		{
			indices.push_back(pScene->mMeshes[i]->mFaces[j].mIndices[0]);
			indices.push_back(pScene->mMeshes[i]->mFaces[j].mIndices[1]);
			indices.push_back(pScene->mMeshes[i]->mFaces[j].mIndices[2]);
		}

		Mesh *newMesh = new Mesh;
		Material *newMaterial = new Material;

		_tObject->push_back(newMesh);
		_tObject->push_back(newMaterial);

		std::string dir = getDir(szFileName);

		const aiTextureType _textureType[] = {
			aiTextureType_DIFFUSE, aiTextureType_SPECULAR, 
			aiTextureType_AMBIENT, aiTextureType_EMISSIVE, 
			aiTextureType_SHININESS, aiTextureType_OPACITY, 
			aiTextureType_HEIGHT, aiTextureType_NORMALS, 
			aiTextureType_DISPLACEMENT, aiTextureType_LIGHTMAP,
		};

		// Textures
		for (UINT j = 0; j < ARRAYSIZE(_textureType); j++)
		{
			aiString path;
			if (pScene->mMaterials[pScene->mMeshes[i]->mMaterialIndex]->GetTexture(_textureType[j], 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
			{
				std::string filePath = dir + path.C_Str();
				Texture2D *newTexture = new Texture2D;
				_tObject->push_back(newTexture);

				newTexture->loadFromFile(filePath.c_str());

				switch (_textureType[j])
				{
				case aiTextureType_DIFFUSE:
					newMaterial->setDiffuseTexture(newTexture);
					break;
				case aiTextureType_SPECULAR:
					newMaterial->setSpecularTexture(newTexture);
					break;
				case aiTextureType_AMBIENT:
					newMaterial->setAmbientTexture(newTexture);
					break;
				case aiTextureType_EMISSIVE:
					newMaterial->setEmissiveTexture(newTexture);
					break;
				case aiTextureType_SHININESS:
					newMaterial->setShininessTexture(newTexture);
					break;
				case aiTextureType_OPACITY:
					newMaterial->setOpacityTexture(newTexture);
					break;
				case aiTextureType_HEIGHT:
					newMaterial->setBumpMap(newTexture);
					break;
				case aiTextureType_NORMALS:
					newMaterial->setNormalMap(newTexture);
					break;
				case aiTextureType_DISPLACEMENT:
					newMaterial->setDisplacementMap(newTexture);
					break;
				case aiTextureType_LIGHTMAP:
					newMaterial->setLightMap(newTexture);
					break;
				}
			}
		}

		{
			aiColor4D mat;

			pScene->mMaterials[pScene->mMeshes[i]->mMaterialIndex]->Get(AI_MATKEY_COLOR_DIFFUSE, mat);
			newMaterial->setDiffuse(XMFLOAT3(mat.r, mat.g, mat.b));

			pScene->mMaterials[pScene->mMeshes[i]->mMaterialIndex]->Get(AI_MATKEY_COLOR_SPECULAR, mat);
			newMaterial->setSpecular(XMFLOAT3(mat.r, mat.g, mat.b));

			pScene->mMaterials[pScene->mMeshes[i]->mMaterialIndex]->Get(AI_MATKEY_COLOR_AMBIENT, mat);
			newMaterial->setAmbient(XMFLOAT3(mat.r, mat.g, mat.b));

			pScene->mMaterials[pScene->mMeshes[i]->mMaterialIndex]->Get(AI_MATKEY_COLOR_EMISSIVE, mat);
			newMaterial->setEmissive(XMFLOAT3(mat.r, mat.g, mat.b));

			pScene->mMaterials[pScene->mMeshes[i]->mMaterialIndex]->Get(AI_MATKEY_SHININESS, mat);
			newMaterial->setShininess(mat.r);

			pScene->mMaterials[pScene->mMeshes[i]->mMaterialIndex]->Get(AI_MATKEY_OPACITY, mat);
			newMaterial->setOpacity(mat.r);
		}

		newMesh->setMaterial(newMaterial);
		newMesh->load((UINT)vertices.size(), vertices.data(), (UINT)indices.size(), indices.data());

		addMesh(newMesh);

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

void Engine::Model::setScale(const XMVECTOR &scale)
{
	XMStoreFloat3(_scale, scale);
	_needMatModel = TRUE;
	_needMatNormal = TRUE;
}

void Engine::Model::setRotation(const XMVECTOR &rotation)
{
	*_rotation = XMQuaternionRotationRollPitchYawFromVector(rotation);
	_needMatModel = TRUE;
	_needMatNormal = TRUE;
}

void Engine::Model::setRotation(const XMVECTOR &axis, const FLOAT &angle)
{
	*_rotation = XMQuaternionRotationAxis(axis, fmod(angle, XM_PI * 2));
	_needMatModel = TRUE;
	_needMatNormal = TRUE;
}

void Engine::Model::setCubeTexture(TextureCube *cubeTex)
{
	_cubeTexture = cubeTex;
}

XMVECTOR Engine::Model::getPosition(void) const
{
	return XMLoadFloat3(_position);
}

XMVECTOR Engine::Model::getScale(void) const
{
	return XMLoadFloat3(_scale);
}

std::pair<XMVECTOR, FLOAT> Engine::Model::getAxisAngleRotation(void) const
{
	std::pair<XMVECTOR, FLOAT> axis_angle;

	XMQuaternionToAxisAngle(&axis_angle.first, &axis_angle.second, *_rotation);

	return axis_angle;
}

std::vector<Engine::Mesh *> Engine::Model::getMeshVector(void) const
{
	return *_tMesh;
}
  
void Engine::Model::display(GBuffer *gbuf, PerspCamera *cam)
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
	_camera.forward = cam->getForwardVector();
	_camera.left = cam->getLeftVector();
	_camera.up = cam->getUpVector();
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
		if ((*_tMesh)[i]->getMaterial()->getOpacity() == 1.0f)
		{
			if (_cubeTexture)
				(*_tMesh)[i]->display(_cubeTexture);
			else
				(*_tMesh)[i]->display();
		}
}

void Engine::Model::displayTransparent(GBuffer *gbuf, PerspCamera *cam)
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
	_camera.forward = cam->getForwardVector();
	_camera.left = cam->getLeftVector();
	_camera.up = cam->getUpVector();
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
		if ((*_tMesh)[i]->getMaterial()->getOpacity() < 1.0f)
		{
			if (_cubeTexture)
				(*_tMesh)[i]->display(_cubeTexture);
			else
				(*_tMesh)[i]->display();
		}
}

void Engine::Model::displayDepthMap(DepthMap *dmap, Camera *cam)
{
	checkMatrix();

	dmap->setState();

	DeviceContext->VSSetShader(_smProgram->getVertexShader(), NULL, 0);
	DeviceContext->HSSetShader(_smProgram->getHullShader(), NULL, 0);
	DeviceContext->DSSetShader(_smProgram->getDomainShader(), NULL, 0);
	DeviceContext->GSSetShader(_smProgram->getGeometryShader(), NULL, 0);
	DeviceContext->PSSetShader(_smProgram->getPixelShader(), NULL, 0);

	_matrix.MVP = *_modelMatrix * cam->getVPMatrix();
	_matrix.projection = cam->getProjectionMatrix();
	_matrix.view = cam->getViewMatrix();
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
		if ((*_tMesh)[i]->getMaterial()->getOpacity() == 1.0f)
			(*_tMesh)[i]->displayShadow();
}

void Engine::Model::displayDepthMap(DepthMap *dmaps, DirLight *light)
{
	checkMatrix();

	DeviceContext->VSSetShader(_smProgram->getVertexShader(), NULL, 0);
	DeviceContext->HSSetShader(_smProgram->getHullShader(), NULL, 0);
	DeviceContext->DSSetShader(_smProgram->getDomainShader(), NULL, 0);
	DeviceContext->GSSetShader(_smProgram->getGeometryShader(), NULL, 0);
	DeviceContext->PSSetShader(_smProgram->getPixelShader(), NULL, 0);

	DeviceContext->IASetInputLayout(_pInputLayout);
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	_matrix.model = *_modelMatrix;
	_matrix.normal = *_normalMatrix;

	for (UINT i = 0; i < CSM_NUM; i++)
	{
		dmaps[i].setState();

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
			if ((*_tMesh)[j]->getMaterial()->getOpacity() == 1.0f)
				(*_tMesh)[j]->displayShadow();
	}
}

void Engine::Model::displayDepthMap(DepthMap *dmap, SpotLight *light)
{
	checkMatrix();

	dmap->setState();

	DeviceContext->VSSetShader(_smProgram->getVertexShader(), NULL, 0);
	DeviceContext->HSSetShader(_smProgram->getHullShader(), NULL, 0);
	DeviceContext->DSSetShader(_smProgram->getDomainShader(), NULL, 0);
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
		if ((*_tMesh)[i]->getMaterial()->getOpacity() == 1.0f)
			(*_tMesh)[i]->displayShadow();
}