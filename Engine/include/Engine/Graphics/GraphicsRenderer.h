#pragma once

#include "..\GameLoop.h"
#include "..\Logger.h"

namespace Engine
{
	namespace Graphics
	{
		class DLLAPI GraphicsRenderer : public Object
		{
		private:
			BOOL _stopLoop;

			UINT _width;
			UINT _height;

			HWND _hWnd;

			// SwapChain
			IDXGISwapChain1 *_pSwapChain;

			// Devices
			ID3D11Device1 *_pDevice;
			ID3D11DeviceContext1 *_pDeviceContext;

			// View
			ID3D11RenderTargetView *_pRenderTargetView;

			// State
			ID3D11DepthStencilState *_pDepthStencilState;
			ID3D11BlendState *_pBlendState;
			ID3D11RasterizerState *_pRasterizerState;

			// ViewPort
			D3D11_VIEWPORT _VP;

			// More Info
			std::string _shaderLevel;

		public:
			static GraphicsRenderer &Instance(void);

		private:
			GraphicsRenderer(void);
			~GraphicsRenderer(void);

			ULONG64 getTime(void) const;
			std::string getShaderLevel(const D3D_FEATURE_LEVEL &featureLevel) const;

		public:
			void init(const LPCWSTR szTitle, const UINT &width, const UINT &height, const BOOL &fullScreen, HINSTANCE hInstance, int nCmdShow);

			void setState(void) const;

			ID3D11Device1 *getDevice(void) const;
			ID3D11DeviceContext1 *getDeviceContext(void) const;
			std::string getShaderLevel(void) const;

			void mainLoop(const std::shared_ptr<GameLoop> &gameLoop);
			void clear(void) const;
		};
	}
}