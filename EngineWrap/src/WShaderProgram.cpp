#include "WShaderProgram.hpp"

WEngine::WShaderProgram::WShaderProgram(LPCWSTR vs, LPCWSTR hs, LPCWSTR ds, LPCWSTR gs, LPCWSTR ps)
{
	pShaderProgram = new Engine::ShaderProgram(vs, hs, ds, gs, ps);
}

WEngine::WShaderProgram::~WShaderProgram(void)
{
	delete pShaderProgram;
}