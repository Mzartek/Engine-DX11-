#ifndef GRAPHICS
#define GRAPHICS

#include <Engine/Window.hpp>
#include <Engine/Input.hpp>
#include <Engine/GBuffer.hpp>
#include <Engine/FreeCam.hpp>
#include <Engine/Model.hpp>
#include <Engine/ShaderProgram.hpp>

// Globales variables
extern engine::Window*				window;
extern engine::Input*				input;
extern engine::GBuffer*				gBuffer;
extern engine::FreeCam*				cam;
extern engine::Model*				sol;
extern engine::Model*				heli;
extern engine::ShaderProgram*		mainProgram;

extern HRESULT configShader(void);
extern HRESULT configBuffer(void);
extern HRESULT configModels(void);

#endif