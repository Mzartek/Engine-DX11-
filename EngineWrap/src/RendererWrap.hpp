#pragma once
#pragma warning(disable:4561)

#include <Engine/Renderer.hpp>

namespace EngineWrap
{
	public ref class RendererWrap
	{
	private:
		Engine::Renderer *pRenderer;
	public:
		RendererWrap(void);
		~RendererWrap(void);
		void initWindow(const HINSTANCE &hInstance, const INT &nCmdShow, const TCHAR *szTitle, const UINT &width, const UINT &height, const BOOL &fullScreen);
		void setDisplayFunc(void(*f) (FLOAT));
	};
}