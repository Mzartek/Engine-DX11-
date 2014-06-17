#ifndef GRAPHICS
#define GRAPHICS

#include <Engine/Window.hpp>
#include <Engine/Camera.hpp>
#include <Engine/Model.hpp>
#include <Engine/ShaderProgram.hpp>

// Globales variables
extern engine::Window*				window;
extern engine::Camera*				cam;
extern engine::Model*				sol;
extern engine::Model*				heli;
extern engine::ShaderProgram*		mainProgram;

extern HRESULT configShader(void);
extern HRESULT configModels(void);

#endif