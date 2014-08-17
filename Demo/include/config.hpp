#ifndef GRAPHICS
#define GRAPHICS

#include <Engine/Renderer.hpp>
#include <Engine/Screen.hpp>
#include <Engine/Input.hpp>
#include <Engine/GBuffer.hpp>
#include <Engine/FreeCam.hpp>
#include <Engine/Model.hpp>
#include <Engine/SkyBox.hpp>
#include <Engine/ShaderProgram.hpp>

// Globales variables
extern engine::Renderer*			renderer;
extern engine::Input*				input;
extern engine::GBuffer*				gBuffer;
extern engine::FreeCam*				cam;
extern engine::Model*				sol;
extern engine::Model*				heli;
extern engine::Screen*				screen;
extern engine::SkyBox*				skybox;
extern engine::ShaderProgram*		gObjectProgram;
extern engine::ShaderProgram*		gSkyboxProgram;
extern engine::ShaderProgram*		screenProgram;

extern HRESULT configShader(void);
extern HRESULT configBuffer(void);
extern HRESULT configModels(void);
extern HRESULT configScreen(void);
extern HRESULT configSkyBox(void);

#endif