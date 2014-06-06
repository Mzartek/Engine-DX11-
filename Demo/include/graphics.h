#ifndef GRAPHICS
#define GRAPHICS

#include <Engine/Window.hpp>
#include <Engine/Camera.hpp>
#include <Engine/ShaderProgram.hpp>

// Globales variables
extern engine::Window*				window;
extern engine::Camera*				cam;
extern engine::ShaderProgram*		mainProgram;

extern HRESULT initShader(void);
extern HRESULT initBuffer(void);

#endif