#include <Engine/OctreeSystem.hpp>
#include <Engine/Camera.hpp>
#include <Engine/Model.hpp>
#include <Engine/ParticlesManager.hpp>
#include <Engine/GBuffer.hpp>

struct Engine::Octree
{
	XMFLOAT3 position;
	XMFLOAT3 vertex[8];
	FLOAT dim;
	FLOAT dim_2;
	FLOAT radius;
	std::vector<Engine::Model *> modelContainer;
	struct Octree *next;
};

static inline BOOL checkCamInOctree(const Engine::Octree *octree, const Engine::Camera *cam)
{
	const XMVECTOR p = cam->getCameraPosition();

	if (XMVectorGetX(p) >= octree->vertex[0].x && XMVectorGetX(p) < octree->vertex[7].x &&
		XMVectorGetY(p) >= octree->vertex[0].y && XMVectorGetY(p) < octree->vertex[7].y &&
		XMVectorGetZ(p) >= octree->vertex[0].z && XMVectorGetZ(p) < octree->vertex[7].z)
		return TRUE;
	return FALSE;
}

static inline BOOL checkOctreeInCamSphere(const Engine::Octree *octree, const Engine::Camera *cam)
{
	const XMVECTOR octree_position = XMLoadFloat3(&octree->position);
	const FLOAT distance = XMVectorGetX(XMVector3Length(octree_position - cam->getFrusSpherePosition()));

	if (distance < octree->radius + cam->getFrusSphereRadius())
		return TRUE;
	return FALSE;
}

static inline BOOL checkOctreeInCamFrus(const Engine::Octree *octree, const Engine::Camera *cam)
{
	const XMVECTOR camera_position = cam->getCameraPosition();
	const XMVECTOR view_vector = cam->getViewVector();
	const FLOAT fov_2 = cam->getFOV() / 2;

	XMVECTOR position = XMLoadFloat3(&octree->position);
	XMVECTOR direction = XMVector3Normalize(position - camera_position);
	FLOAT dot = XMVectorGetX(XMVector3Dot(view_vector, direction));

	if (acosf(dot) < fov_2) return TRUE;
	for (INT i = 0; i < 8; i++)
	{
		position = XMLoadFloat3(&octree->vertex[i]);
		direction = XMVector3Normalize(position - camera_position);
		dot = XMVectorGetX(XMVector3Dot(view_vector, direction));

		if (acosf(dot) < fov_2) return TRUE;
	}
	return FALSE;
}

void Engine::OctreeSystem::initOctree(const UINT &depth, Octree *octree, const XMFLOAT3 &position, const FLOAT &dim) const
{
	if (depth >= _maxDepth)	return;

	FLOAT newDim = dim / 2;
	FLOAT tmp = newDim / 2;

	octree->position = position;
	octree->dim = dim;
	octree->dim_2 = newDim;
	octree->radius = XMVectorGetX(XMVector3Length(XMVectorSet(newDim, newDim, newDim, 0)));
	octree->next = new Octree[8];

	octree->vertex[0] = XMFLOAT3(position.x - newDim, position.y - newDim, position.z - newDim);
	octree->vertex[1] = XMFLOAT3(position.x - newDim, position.y - newDim, position.z + newDim);
	octree->vertex[2] = XMFLOAT3(position.x - newDim, position.y + newDim, position.z - newDim);
	octree->vertex[3] = XMFLOAT3(position.x - newDim, position.y + newDim, position.z + newDim);
	octree->vertex[4] = XMFLOAT3(position.x + newDim, position.y - newDim, position.z - newDim);
	octree->vertex[5] = XMFLOAT3(position.x + newDim, position.y - newDim, position.z + newDim);
	octree->vertex[6] = XMFLOAT3(position.x + newDim, position.y + newDim, position.z - newDim);
	octree->vertex[7] = XMFLOAT3(position.x + newDim, position.y + newDim, position.z + newDim);

	initOctree(depth + 1, &octree->next[0], XMFLOAT3(position.x - tmp, position.y - tmp, position.z - tmp), newDim);
	initOctree(depth + 1, &octree->next[1], XMFLOAT3(position.x - tmp, position.y - tmp, position.z + tmp), newDim);
	initOctree(depth + 1, &octree->next[2], XMFLOAT3(position.x - tmp, position.y + tmp, position.z - tmp), newDim);
	initOctree(depth + 1, &octree->next[3], XMFLOAT3(position.x - tmp, position.y + tmp, position.z + tmp), newDim);
	initOctree(depth + 1, &octree->next[4], XMFLOAT3(position.x + tmp, position.y - tmp, position.z - tmp), newDim);
	initOctree(depth + 1, &octree->next[5], XMFLOAT3(position.x + tmp, position.y - tmp, position.z + tmp), newDim);
	initOctree(depth + 1, &octree->next[6], XMFLOAT3(position.x + tmp, position.y + tmp, position.z - tmp), newDim);
	initOctree(depth + 1, &octree->next[7], XMFLOAT3(position.x + tmp, position.y + tmp, position.z + tmp), newDim);
}

void Engine::OctreeSystem::destroyOctree(const UINT &depth, Octree *octree) const
{
	if (depth >= _maxDepth)	return;

	for (UINT i = 0; i < 8; i++)
		destroyOctree(depth + 1, &(octree->next[i]));
	delete[] octree->next;
}

BOOL Engine::OctreeSystem::addModelOctree(const UINT &depth, Octree *octree, Model *model, const FLOAT &dim) const
{
	if (depth >= _maxDepth) return FALSE;
	if (dim > octree->dim) return FALSE;

	XMFLOAT3 octree_pos = octree->position;
	XMFLOAT3 model_pos; XMStoreFloat3(&model_pos, model->getPosition());

	// Check the position
	if (model_pos.x < (octree_pos.x - octree->dim_2) || model_pos.x >= (octree_pos.x + octree->dim_2) ||
		model_pos.y < (octree_pos.y - octree->dim_2) || model_pos.y >= (octree_pos.y + octree->dim_2) ||
		model_pos.z < (octree_pos.z - octree->dim_2) || model_pos.z >= (octree_pos.z + octree->dim_2))
		return FALSE;

	// Recursive call
	if (addModelOctree(depth + 1, &octree->next[0], model, dim)) return TRUE;
	if (addModelOctree(depth + 1, &octree->next[1], model, dim)) return TRUE;
	if (addModelOctree(depth + 1, &octree->next[2], model, dim)) return TRUE;
	if (addModelOctree(depth + 1, &octree->next[3], model, dim)) return TRUE;
	if (addModelOctree(depth + 1, &octree->next[4], model, dim)) return TRUE;
	if (addModelOctree(depth + 1, &octree->next[5], model, dim)) return TRUE;
	if (addModelOctree(depth + 1, &octree->next[6], model, dim)) return TRUE;
	if (addModelOctree(depth + 1, &octree->next[7], model, dim)) return TRUE;

	octree->modelContainer.push_back(model);

	return TRUE;
}

void Engine::OctreeSystem::removeModelOctree(const UINT &depth, Octree *octree, Model *model) const
{
	if (depth >= _maxDepth) return;

	UINT i;

	for (i = 0; i < octree->modelContainer.size(); i++)
	{
		if (octree->modelContainer[i] == model)
			octree->modelContainer.erase(octree->modelContainer.begin() + i);
	}

	for (i = 0; i < 8; i++)
		removeModelOctree(depth + 1, &(octree->next[i]), model);
}

void Engine::OctreeSystem::getModelOctree(const UINT &depth, Octree *octree, GBuffer *gbuffer, Camera *cam, std::vector<Model *> *modelVector) const
{
	if (depth >= _maxDepth)	return;

	if (!checkCamInOctree(octree, cam))
	{
		if (!checkOctreeInCamSphere(octree, cam)) return;
		if (!checkOctreeInCamFrus(octree, cam)) return;
	}

	UINT i;

	for (i = 0; i < octree->modelContainer.size(); i++)
	{
		modelVector->push_back(octree->modelContainer[i]);
	}

	for (i = 0; i < 8; i++)
		getModelOctree(depth + 1, &(octree->next[i]), gbuffer, cam, modelVector);
}

Engine::OctreeSystem::OctreeSystem(const UINT &maxDepth, const XMVECTOR &pos, const FLOAT &dim)
{
	XMFLOAT3 position; XMStoreFloat3(&position, pos);

	_maxDepth = maxDepth;
	_octree = new Octree;

	initOctree(0, _octree, position, dim);
}

Engine::OctreeSystem::~OctreeSystem()
{
	destroyOctree(0, _octree);
	delete _octree;
}

void Engine::OctreeSystem::addModel(Model *model, const FLOAT &dim) const
{
	if (!addModelOctree(0, _octree, model, dim))
		std::cerr << "Unable to add model on the OctreeSystem" << std::endl;
}

void Engine::OctreeSystem::removeModel(Model *model) const
{
	removeModelOctree(0, _octree, model);
}

void Engine::OctreeSystem::getModel(GBuffer *gbuffer, Camera *cam, std::vector<Model *> *modelVector) const
{
	getModelOctree(0, _octree, gbuffer, cam, modelVector);
}