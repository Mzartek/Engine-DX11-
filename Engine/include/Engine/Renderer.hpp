#ifndef RENDERER
#define RENDERER

#include "Object.hpp"

namespace engine
{
	extern D3D_FEATURE_LEVEL EngineGraphicsFeatureLevel;
	extern std::string EngineShaderLevel;
	class DLLAPI Renderer : public Object
	{
	private:
		BOOL _stopLoop;
		UINT _width;
		UINT _height;
		HINSTANCE _hInst;
		HWND _hWnd;
		// Device
		ID3D11Device *_pd3dDevice;
		ID3D11DeviceContext *_pContext;
		// Texture
		IDXGISwapChain *_pSwapChain;
		ID3D11Texture2D *_pDepthStencilTexture;
		// View
		ID3D11RenderTargetView *_pRenderTargetView;
		ID3D11DepthStencilView *_pDepthStencilView;
		// State
		ID3D11DepthStencilState *_pDepthStencilState;
		ID3D11BlendState *_pBlendState;
		ID3D11RasterizerState *_pRasterizerState;
		// ViewPort
		D3D11_VIEWPORT *_pVP;
		// Function Pointer
		void(*_display) (void);
		void(*_idle) (void);
		void(*_reshape) (UINT, UINT);
	public:
		Renderer(void);
		~Renderer(void);
		void initWindow(const HINSTANCE &hInstance, LRESULT(CALLBACK *WndProc) (HWND, UINT, WPARAM, LPARAM), 
			const TCHAR *szTitle, const UINT &width, const UINT &height, const BOOL &fullScreen);
		void setDisplayFunc(void(*f) (void));
		void setIdleFunc(void(*f) (void));
		void setReshapeFunc(void(*f) (UINT, UINT));
		UINT getWidth(void);
		UINT getHeight(void);
		HINSTANCE getHINSTANCE(void);
		HWND getHWND(void);
		ID3D11Device *getD3DDevice(void);
		ID3D11DeviceContext *getContext(void);
		void mainLoop(int nCmdShow);
		void stopLoop(void);
		void setConfig(void) const;
		void clear(void);
	};
}

#endif
