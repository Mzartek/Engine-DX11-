#ifndef RENDERER
#define RENDERER

#include "Object.hpp"

namespace Engine
{
	class DLLAPI Renderer : public Object
	{
	private:
		BOOL _stopLoop;
		UINT _width;
		UINT _height;
		HINSTANCE _hInst;
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
		// Function Pointer
		void(*_display) (FLOAT);
		void(*_idle) (void);
		void(*_reshape) (UINT, UINT);
	public:
		Renderer(void);
		~Renderer(void);
		void initWindow(const HINSTANCE &hInstance, const INT &nCmdShow, const TCHAR *szTitle, const UINT &width, const UINT &height, const BOOL &fullScreen);
		void setDisplayFunc(void(*f) (FLOAT));
		void setIdleFunc(void(*f) (void));
		void setReshapeFunc(void(*f) (UINT, UINT));
		UINT getWidth(void);
		UINT getHeight(void);
		HINSTANCE getHINSTANCE(void);
		HWND getHWND(void);
		void mainLoop(void);
		void stopLoop(void);
		void setState(void) const;
		void clear(void);
	};
}

#endif
