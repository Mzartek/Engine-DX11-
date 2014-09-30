#include "WShaderProgram.hpp"

WEngine::WShaderProgram::WShaderProgram(void)
{
	pShaderProgram = new Engine::ShaderProgram();
}

WEngine::WShaderProgram::~WShaderProgram(void)
{
	delete pShaderProgram;
}

void WEngine::WShaderProgram::loadProgram(LPCWSTR vs, LPCWSTR hs, LPCWSTR ds, LPCWSTR gs, LPCWSTR ps)
{
	pShaderProgram->loadProgram(vs, hs, ds, gs, ps);
}