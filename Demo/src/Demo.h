#pragma once

#include <Engine\GameLoop.h>
#include <Engine\Graphics\GraphicsRenderer.h>
#include <Engine\Graphics\ShaderProgram.h>

using namespace Engine;

class Demo : public GameLoop
{
public:
	Demo(void);

	void display(FLOAT state);
	void state(ULONG64 time);
	void lastState(void);
	void reshape(UINT w, UINT h);
};