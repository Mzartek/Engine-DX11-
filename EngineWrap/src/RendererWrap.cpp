#include "RendererWrap.hpp"

EngineWrap::RendererWrap::RendererWrap(void)
{
	pRenderer = new Engine::Renderer();
}

EngineWrap::RendererWrap::~RendererWrap(void)
{
	delete pRenderer;
}

void EngineWrap::RendererWrap::initWindow(const HINSTANCE &hInstance, const INT &nCmdShow, const TCHAR *szTitle, const UINT &width, const UINT &height, const BOOL &fullScreen)
{
	pRenderer->initWindow(hInstance, nCmdShow, szTitle, width, height, fullScreen);
}

void EngineWrap::RendererWrap::setDisplayFunc(void(*f) (FLOAT))
{
	pRenderer->setDisplayFunc(f);
}