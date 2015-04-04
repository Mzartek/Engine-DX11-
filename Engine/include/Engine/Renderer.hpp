#ifndef RENDERER_HEADER
#define RENDERER_HEADER

#include "Object.hpp"

namespace Engine
{
	class DLLAPI GameLoop : public Object
	{
	public:
		GameLoop(void);
		~GameLoop(void);
		virtual void display(FLOAT state) = 0;
		virtual void idle(long long time) = 0;
		virtual void reshape(UINT w, UINT h) = 0;
	};

	class DLLAPI Renderer : public Object
	{
	private:
		BOOL _stopLoop;
		UINT _width;
		UINT _height;
		HWND _hWnd;
		// SwapChain
		IDXGISwapChain1 *_pSwapChain;
		// View
		ID3D11RenderTargetView *_pRenderTargetView;
		ID3D11DepthStencilView *_pDepthStencilView;
		// State
		ID3D11DepthStencilState *_pDepthStencilState;
		ID3D11BlendState *_pBlendState;
		ID3D11RasterizerState *_pRasterizerState;
		// ViewPort
		D3D11_VIEWPORT _VP;
	public:
		Renderer(const LPCWSTR szTitle, const UINT &width, const UINT &height, const BOOL &fullScreen, HINSTANCE hInstance, int nCmdShow);
		~Renderer(void);
		UINT getWidth(void);
		UINT getHeight(void);
		HWND getWindow(void);
		void mainLoop(GameLoop *gameLoop);
		void stopLoop(void);
		void setState(void) const;
		void clear(void) const;
	};
}

#endif
