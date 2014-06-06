#ifndef WINDOW
#define WINDOW

#include "DXHead.hpp"

namespace engine
{
	class DLLAPI Window
	{
	private:
		UINT _width;
		UINT _height;
		HINSTANCE _hInst;
		HWND _hWnd;
		D3D_DRIVER_TYPE _driverType;
		D3D_FEATURE_LEVEL _featureLevel;
		ID3D11Device *_pd3dDevice;
		ID3D11Device1 *_pd3dDevice1;
		ID3D11DeviceContext *_pImmediateContext;
		ID3D11DeviceContext1 *_pImmediateContext1;
		IDXGISwapChain *_pSwapChain;
		ID3D11RenderTargetView *_pRenderTargetView;
		ID3D11DepthStencilView *_pDepthStencilView;
		void(*_display) (void);
		void(*_idle) (void);
		void(*_reshape) (UINT, UINT);
		void(*_keyboard) (UCHAR, BOOL);
		void(*_mouseMove) (INT, INT);
	public:
		Window(void);
		~Window(void);
		HRESULT initWindow(const HINSTANCE hInstance, LRESULT(CALLBACK *WndProc) (HWND, UINT, WPARAM, LPARAM), const TCHAR *szWindowClass, const TCHAR *szTitle, UINT width, UINT height);
		void setDisplayFunc(void(*f) (void));
		void setIdleFunc(void(*f) (void));
		void setReshapeFunc(void(*f) (UINT, UINT));
		void setKeyboardFunc(void(*f) (UCHAR, BOOL));
		void setMouseMoveFunc(void(*f) (INT, INT));
		UINT getWidth(void);
		UINT getHeight(void);
		ID3D11Device *getD3DDevice(void);
		ID3D11DeviceContext *getImmediateContext(void);
		IDXGISwapChain *getSwapChain(void);
		ID3D11RenderTargetView *getRenderTargetView(void);
		ID3D11DepthStencilView *getDepthStencilView(void);
		void mainLoop(int nCmdShow);
		void clear(void);
	};
}

#endif
