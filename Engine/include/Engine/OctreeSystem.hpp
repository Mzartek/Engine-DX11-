#ifndef OCTREESYSTEM_HEADER
#define OCTREESYSTEM_HEADER

#include "Object.hpp"

namespace Engine
{
	class Camera;
	class Model;
	class ParticlesManager;
	class GBuffer;

	struct Octree;

	class DLLAPI OctreeSystem : public Object
	{
	private:
		UINT _maxDepth;
		Octree *_octree;
		void _initOctree(const UINT &depth, Octree *octree, const XMFLOAT3 &position, const FLOAT &dim) const;
		void _destroyOctree(const UINT &depth, Octree *octree) const;
		BOOL _addModelOctree(const UINT &depth, Octree *octree, Model *model, const FLOAT &dim) const;
		void _removeModelOctree(const UINT &depth, Octree *octree, Model *model) const;
		void _getModelsOctree(const UINT &depth, Octree *octree, GBuffer *gbuffer, Camera *cam, std::set<Model *> *modelSet) const;
	public:
		OctreeSystem(const UINT &maxDepth, const XMVECTOR &pos, const FLOAT &dim);
		~OctreeSystem(void);
		void addModel(Model *model, const FLOAT &dim) const;
		void removeModel(Model *model) const;
		void getModels(GBuffer *gbuffer, Camera *cam, std::set<Model *> *modelSet) const;
	};
}

#endif