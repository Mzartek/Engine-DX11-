#ifndef WINDOW
#define WINDOW

#include "Object.hpp"

namespace engine
{
	class DLLAPI Window : public Object
	{
	private:
		BOOL _stopLoop;
		UINT _width;
		UINT _height;
		HINSTANCE _hInst;
		HWND _hWnd;
		ID3D11Device *_pd3dDevice;
		ID3D11DeviceContext *_pImmediateContext;
		IDXGISwapChain *_pSwapChain;
		ID3D11Texture2D *_pDepthStencilTex;
		ID3D11RenderTargetView *_pRenderTargetView;
		ID3D11DepthStencilView *_pDepthStencilView;
		ID3D11RasterizerState *_pRasterState;
		ID3D11BlendState *_pBlendState;
		// Function Pointer
		void(*_display) (void);
		void(*_idle) (void);
		void(*_reshape) (UINT, UINT);
	public:
		Window(void);
		~Window(void);
		HRESULT initWindow(const HINSTANCE &hInstance, LRESULT(CALLBACK *WndProc) (HWND, UINT, WPARAM, LPARAM), const TCHAR *szWindowClass, 
			const TCHAR *szTitle, const UINT &width, const UINT &height, const BOOL &fullScreen);
		void setDisplayFunc(void(*f) (void));
		void setIdleFunc(void(*f) (void));
		void setReshapeFunc(void(*f) (UINT, UINT));
		UINT getWidth(void);
		UINT getHeight(void);
		HINSTANCE getHINSTANCE(void);
		HWND getHWND(void);
		ID3D11Device *getD3DDevice(void);
		ID3D11DeviceContext *getImmediateContext(void);
		void mainLoop(int nCmdShow);
		void stopLoop(void);
		void clear(void);
	};
}

#endif
