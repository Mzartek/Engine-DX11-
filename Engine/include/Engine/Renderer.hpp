#ifndef RENDERER
#define RENDERER

#include "Object.hpp"

namespace Engine
{
	class DLLAPI GameLoop : public Object
	{
	public:
		GameLoop(void);
		~GameLoop(void);
		virtual void display(FLOAT state) = 0;
		virtual void idle(void) = 0;
		virtual void reshape(UINT w, UINT h) = 0;
	};

	class DLLAPI Renderer : public Object
	{
	private:
		BOOL _stopLoop;
		UINT _width;
		UINT _height;
		HWND _hWnd;
		// EngineDevice
		EngineDevice _EngineDevice;
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
		Renderer(const LPCTSTR szTitle, const UINT &width, const UINT &height, const BOOL &fullScreen);
		~Renderer(void);
		UINT getWidth(void);
		UINT getHeight(void);
		HWND getWindow(void);
		EngineDevice getEngineDevice(void);
		void mainLoop(GameLoop *gameLoop);
		void stopLoop(void);
		void setState(void) const;
		void clear(void) const;
	};
}

#endif
