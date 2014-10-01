#pragma once
#pragma warning(disable:4561)

#include <Engine/ShaderProgram.hpp>

namespace WEngine
{
	public ref class WShaderProgram
	{
	private:
		Engine::ShaderProgram *pShaderProgram;
	public:
		WShaderProgram(LPCWSTR vs, LPCWSTR hs, LPCWSTR ds, LPCWSTR gs, LPCWSTR ps);
		~WShaderProgram(void);
	};
}