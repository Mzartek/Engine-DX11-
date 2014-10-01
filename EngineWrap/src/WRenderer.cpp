#include "WRenderer.hpp"

WEngine::WGameLoop::WGameLoop(void)
{
}

WEngine::WGameLoop::~WGameLoop(void)
{
}

WEngine::WRenderer::WRenderer(const LPCTSTR szTitle, const UINT &width, const UINT &height, const BOOL &fullScreen)
{
	pRenderer = new Engine::Renderer(szTitle, width, height, fullScreen);
}

WEngine::WRenderer::~WRenderer(void)
{
	delete pRenderer;
}

UINT WEngine::WRenderer::getWidth(void)
{
	return pRenderer->getWidth();
}

UINT WEngine::WRenderer::getHeight(void)
{
	return pRenderer->getHeight();
}

HWND WEngine::WRenderer::getWindow(void)
{
	return pRenderer->getWindow();
}

void WEngine::WRenderer::mainLoop(WGameLoop ^gameLoop)
{
}

void WEngine::WRenderer::stopLoop(void)
{
	pRenderer->stopLoop();
}

void WEngine::WRenderer::setState(void)
{
	pRenderer->setState();
}

void WEngine::WRenderer::clear(void)
{
	pRenderer->clear();
}
