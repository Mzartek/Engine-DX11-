#include <Engine/Window.hpp>

engine::Window::Window(void)
{
	// Device
	_pd3dDevice = NULL;
	_pImmediateContext = NULL;
	//Texture
	_pSwapChain = NULL;
	_pDepthStencilTexture = NULL;
	//View
	_pRenderTargetView = NULL;
	_pDepthStencilView = NULL;
	//State
	_pDepthStencilState = NULL;
	_pBlendState = NULL;
	_pRasterizerState = NULL;
	// Function Pointer
	_display = NULL;
	_idle = NULL;
	_reshape = NULL;
}

engine::Window::~Window(void)
{
	if (_pImmediateContext)
		_pImmediateContext->ClearState();

	//State
	if (_pRasterizerState)
		_pRasterizerState->Release();
	if (_pBlendState)
		_pBlendState->Release();
	if (_pDepthStencilState)
		_pDepthStencilState->Release();

	//View
	if (_pDepthStencilView)
		_pDepthStencilView->Release();
	if (_pRenderTargetView)
		_pRenderTargetView->Release();

	//Texture
	if (_pDepthStencilTexture)
		_pDepthStencilTexture->Release();
	if (_pSwapChain)
		_pSwapChain->Release();

	//Device
	if (_pImmediateContext)
		_pImmediateContext->Release();
	if (_pd3dDevice) 
		_pd3dDevice->Release();
}

HRESULT engine::Window::initWindow(const HINSTANCE &hInstance, LRESULT(CALLBACK *WndProc) (HWND, UINT, WPARAM, LPARAM), 
	const TCHAR *szTitle, const UINT &width, const UINT &height, const BOOL &fullScreen)
{
	HRESULT hr;
	_width = width;
	_height = height;

	WNDCLASS wcex;
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szTitle;

	RegisterClass(&wcex);

	_hInst = hInstance;
	_hWnd = CreateWindow(szTitle, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, hInstance, NULL);

	if (!_hWnd)
	{
		MessageBox(NULL, "Failed to create Window", "Window", NULL);
		return E_FAIL;
	}

	// Init swap chain
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.SampleDesc.Count = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = _hWnd;
	sd.Windowed = !fullScreen;

	// Create the Device
	D3D_FEATURE_LEVEL featureLevels[]
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION,
		&sd, &_pSwapChain, &_pd3dDevice, NULL, &_pImmediateContext);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create Device and SwapChain", "Window", NULL);
		return hr;
	}

	// Create the RenderTargetView
	ID3D11Texture2D *pBackBuffer;
	hr = _pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void **)&pBackBuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to get BackBuffer", "Window", NULL);
		return hr;
	}

	hr = _pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &_pRenderTargetView);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create RenderTarget View", "Window", NULL);
		return hr;
	}

	// Create the DepthStencilTexture
	D3D11_TEXTURE2D_DESC descDepthStencilTexture;
	ZeroMemory(&descDepthStencilTexture, sizeof(descDepthStencilTexture));
	descDepthStencilTexture.Width = width;
	descDepthStencilTexture.Height = height;
	descDepthStencilTexture.MipLevels = 1;
	descDepthStencilTexture.ArraySize = 1;
	descDepthStencilTexture.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepthStencilTexture.SampleDesc.Count = 1;
	descDepthStencilTexture.Usage = D3D11_USAGE_DEFAULT;
	descDepthStencilTexture.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	hr = _pd3dDevice->CreateTexture2D(&descDepthStencilTexture, NULL, &_pDepthStencilTexture);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create DepthStencil Texture", "Window", NULL);
		return hr;
	}

	// Create the DepthStencilView
	D3D11_DEPTH_STENCIL_VIEW_DESC descDepthStencilView;
	ZeroMemory(&descDepthStencilView, sizeof(descDepthStencilView));
	descDepthStencilView.Format = descDepthStencilTexture.Format;
	descDepthStencilView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	hr = _pd3dDevice->CreateDepthStencilView(_pDepthStencilTexture, &descDepthStencilView, &_pDepthStencilView);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create DepthStencil View", "Window", NULL);
		return hr;
	}

	// Create the DepthStencilState
	D3D11_DEPTH_STENCIL_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.DepthEnable = TRUE;
	descDepth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	descDepth.DepthFunc = D3D11_COMPARISON_LESS;
	descDepth.StencilEnable = FALSE;
	descDepth.StencilReadMask = 0xFF;
	descDepth.StencilWriteMask = 0xFF;
	hr = _pd3dDevice->CreateDepthStencilState(&descDepth, &_pDepthStencilState);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create DepthStencil State", "Window", NULL);
		return hr;
	}

	// Create the BlendState
	D3D11_BLEND_DESC descBlend;
	ZeroMemory(&descBlend, sizeof(descBlend));
	descBlend.RenderTarget[0].BlendEnable = TRUE;
	descBlend.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	descBlend.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	descBlend.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	descBlend.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	descBlend.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
	descBlend.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	descBlend.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	hr = _pd3dDevice->CreateBlendState(&descBlend, &_pBlendState);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create Blend State", "Window", NULL);
		return hr;
	}

	// Create the RasterizerState
	D3D11_RASTERIZER_DESC descRasterizer;
	ZeroMemory(&descRasterizer, sizeof(descRasterizer));
	descRasterizer.FillMode = D3D11_FILL_SOLID;
	descRasterizer.CullMode = D3D11_CULL_NONE;
	hr = _pd3dDevice->CreateRasterizerState(&descRasterizer, &_pRasterizerState);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create Rasterizer State", "Window", NULL);
		return hr;
	}

	// Create the Viewport
	D3D11_VIEWPORT vp;
	ZeroMemory(&vp, sizeof(vp));
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MaxDepth = 1.0f;

	_pImmediateContext->OMSetRenderTargets(1, &_pRenderTargetView, _pDepthStencilView);
	_pImmediateContext->OMSetDepthStencilState(_pDepthStencilState, 0);
	_pImmediateContext->OMSetBlendState(_pBlendState, NULL, 0xFFFFFFFF);
	_pImmediateContext->RSSetState(_pRasterizerState);
	_pImmediateContext->RSSetViewports(1, &vp);

	return S_OK;
}

void engine::Window::setDisplayFunc(void (*f) (void))
{
	_display = f;
}

void engine::Window::setIdleFunc(void (*f) (void))
{
	_idle = f;
}

void engine::Window::setReshapeFunc(void(*f) (UINT, UINT))
{
	_reshape = f;
}

UINT engine::Window::getWidth(void)
{
	return _width;
}

UINT engine::Window::getHeight(void)
{
	return _height;
}

HINSTANCE engine::Window::getHINSTANCE(void)
{
	return _hInst;
}

HWND engine::Window::getHWND(void)
{
	return _hWnd;
}

ID3D11Device *engine::Window::getD3DDevice(void)
{
	return _pd3dDevice;
}

ID3D11DeviceContext *engine::Window::getImmediateContext(void)
{
	return _pImmediateContext;
}

void engine::Window::mainLoop(int nCmdShow)
{
	MSG msg = { 0 };

	_stopLoop = FALSE;
	ShowWindow(_hWnd, nCmdShow);
	//ShowCursor(FALSE);
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			if (_stopLoop)
				msg.message = WM_QUIT;
			if (_idle)
				_idle();
			if (_display)
				_display();
			_pSwapChain->Present(1, 0);
		}
	}
}

void engine::Window::stopLoop(void)
{
	_stopLoop = TRUE;
}

void engine::Window::clear(void)
{
	_pImmediateContext->ClearRenderTargetView(_pRenderTargetView, DirectX::Colors::Transparent);
	_pImmediateContext->ClearDepthStencilView(_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}