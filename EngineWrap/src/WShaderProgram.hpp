#pragma once

#include <Engine/ShaderProgram.hpp>

namespace WEngine
{
	public ref class WShaderProgram
	{
	private:
		Engine::ShaderProgram *pShaderProgram;
	public:
		WShaderProgram(void);
		~WShaderProgram(void);
		void loadProgram(LPCWSTR vs, LPCWSTR hs, LPCWSTR ds, LPCWSTR gs, LPCWSTR ps);
	};
}