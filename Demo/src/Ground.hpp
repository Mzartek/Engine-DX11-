#ifndef GROUND_HEADER
#define GROUND_HEADER

#include <Engine/Engine.hpp>

class Ground
{
private:
	Engine::Texture2D *_texture;
	Engine::Material *_material;
	Engine::Mesh *_mesh;
	Engine::Model *_model;
public:
	Ground(Engine::ShaderProgram *objectProgram, Engine::ShaderProgram *depthMapProgram);
	~Ground(void);
	Engine::Model *getModel(void) const;
};

#endif