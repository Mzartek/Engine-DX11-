#pragma once
#pragma warning(disable:4561)

#include <Engine/Renderer.hpp>

namespace WEngine
{
	public ref class WGameLoop abstract
	{
	public:
		WGameLoop(void);
		~WGameLoop(void);
		virtual void display(FLOAT state) = 0;
		virtual void idle(void) = 0;
		virtual void reshape(UINT w, UINT h) = 0;
	};

	public ref class WRenderer
	{
	private:
		Engine::Renderer *pRenderer;
	public:
		WRenderer(const LPCTSTR szTitle, const UINT &width, const UINT &height, const BOOL &fullScreen);
		~WRenderer(void);
		UINT getWidth(void);
		UINT getHeight(void);
		HWND getWindow(void);
		void mainLoop(WGameLoop ^gameLoop);
		void stopLoop(void);
		void setState(void);
		void clear(void);
	};
}