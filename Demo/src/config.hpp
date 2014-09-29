#ifndef GRAPHICS
#define GRAPHICS

#include <Engine/Renderer.hpp>
#include <Engine/Input.hpp>
#include <Engine/FreeCam.hpp>
#include <Engine/DirLight.hpp>
#include <Engine/SpotLight.hpp>
#include <Engine/Model.hpp>
#include <Engine/SkyBox.hpp>
#include <Engine/Screen.hpp>
#include <Engine/GBuffer.hpp>
#include <Engine/ShaderProgram.hpp>

// Globales variables
extern Engine::Renderer *renderer;
extern Engine::Input *input;
extern Engine::FreeCam *cam;
extern Engine::DirLight *sun;
extern Engine::SpotLight *torch;
extern Engine::Model *sol;
extern Engine::Model *heli;
extern Engine::SkyBox *skybox;
extern Engine::Screen *screen;
extern Engine::GBuffer *gBuffer;

extern Engine::ShaderProgram *objectProgram;
extern Engine::ShaderProgram *dirLightProgram;
extern Engine::ShaderProgram *spotLightProgram;
extern Engine::ShaderProgram *vLightProgram;
extern Engine::ShaderProgram *shadowMapProgram;
extern Engine::ShaderProgram *skyboxProgram;
extern Engine::ShaderProgram *backgroundProgram;
extern Engine::ShaderProgram *screenProgram;

extern void configShaders(void);
extern void configBuffers(void);
extern void configLights(void);
extern void configScreen(void);
extern void configModels(void);
extern void configSkybox(void);

#endif