#include <Engine/Renderer.hpp>

engine::Renderer::Renderer(void)
{
	// Device
	_pd3dDevice = NULL;
	_pImmediateContext = NULL;
	// Texture
	_pSwapChain = NULL;
	_pDepthStencilTexture = NULL;
	// View
	_pRenderTargetView = NULL;
	_pDepthStencilView = NULL;
	// State
	_pDepthStencilState = NULL;
	_pBlendState = NULL;
	_pRasterizerState = NULL;
	// Function Pointer
	_display = NULL;
	_idle = NULL;
	_reshape = NULL;
}

engine::Renderer::~Renderer(void)
{
	if (_pImmediateContext)
		_pImmediateContext->ClearState();

	// State
	if (_pRasterizerState)
		_pRasterizerState->Release();
	if (_pBlendState)
		_pBlendState->Release();
	if (_pDepthStencilState)
		_pDepthStencilState->Release();

	// View
	if (_pDepthStencilView)
		_pDepthStencilView->Release();
	if (_pRenderTargetView)
		_pRenderTargetView->Release();

	// Texture
	if (_pDepthStencilTexture)
		_pDepthStencilTexture->Release();
	if (_pSwapChain)
		_pSwapChain->Release();

	// Device
	if (_pImmediateContext)
		_pImmediateContext->Release();
	if (_pd3dDevice) 
		_pd3dDevice->Release();
}

HRESULT engine::Renderer::initWindow(const HINSTANCE &hInstance, LRESULT(CALLBACK *WndProc) (HWND, UINT, WPARAM, LPARAM), 
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
	_hWnd = CreateWindow(szTitle, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, _width, _height, NULL, NULL, hInstance, NULL);

	if (!_hWnd)
	{
		MessageBox(NULL, "Failed to create Renderer", "Renderer", NULL);
		return E_FAIL;
	}

	// Init swap chain
	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = _width;
	sd.BufferDesc.Height = _height;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = _hWnd;
	sd.Windowed = !fullScreen;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

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
		MessageBox(NULL, "Failed to create Device and SwapChain", "Renderer", NULL);
		return hr;
	}

	// Create the RenderTargetView
	ID3D11Texture2D *pBackBuffer;
	hr = _pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void **)&pBackBuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to get BackBuffer", "Renderer", NULL);
		return hr;
	}
	hr = _pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &_pRenderTargetView);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create RenderTarget View", "Renderer", NULL);
		return hr;
	}

	// Create the DepthStencilTexture
	D3D11_TEXTURE2D_DESC descDepthStencilTexture;
	descDepthStencilTexture.Width = _width;
	descDepthStencilTexture.Height = _height;
	descDepthStencilTexture.MipLevels = 1;
	descDepthStencilTexture.ArraySize = 1;
	descDepthStencilTexture.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepthStencilTexture.SampleDesc.Count = 1;
	descDepthStencilTexture.SampleDesc.Quality = 0;
	descDepthStencilTexture.Usage = D3D11_USAGE_DEFAULT;
	descDepthStencilTexture.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepthStencilTexture.CPUAccessFlags = 0;
	descDepthStencilTexture.MiscFlags = 0;
	hr = _pd3dDevice->CreateTexture2D(&descDepthStencilTexture, NULL, &_pDepthStencilTexture);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create DepthStencil Texture", "Renderer", NULL);
		return hr;
	}

	// Create the DepthStencilView
	D3D11_DEPTH_STENCIL_VIEW_DESC descDepthStencilView;
	descDepthStencilView.Format = descDepthStencilTexture.Format;
	descDepthStencilView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDepthStencilView.Flags = 0;
	descDepthStencilView.Texture2D.MipSlice = 0;
	hr = _pd3dDevice->CreateDepthStencilView(_pDepthStencilTexture, &descDepthStencilView, &_pDepthStencilView);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create DepthStencil View", "Renderer", NULL);
		return hr;
	}

	// Create the DepthStencilState
	D3D11_DEPTH_STENCIL_DESC descDepth;
	descDepth.DepthEnable = TRUE;
	descDepth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	descDepth.DepthFunc = D3D11_COMPARISON_LESS;
	descDepth.StencilEnable = FALSE;
	descDepth.StencilReadMask = 0xFF;
	descDepth.StencilWriteMask = 0xFF;
	descDepth.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	descDepth.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	descDepth.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
	descDepth.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	descDepth.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	descDepth.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	descDepth.BackFace.StencilPassOp = D3D11_STENCIL_OP_DECR;
	descDepth.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	hr = _pd3dDevice->CreateDepthStencilState(&descDepth, &_pDepthStencilState);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create DepthStencil State", "Renderer", NULL);
		return hr;
	}

	// Create the BlendState
	D3D11_BLEND_DESC descBlend;
	descBlend.AlphaToCoverageEnable = FALSE;
	descBlend.IndependentBlendEnable = FALSE;
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
		MessageBox(NULL, "Failed to create Blend State", "Renderer", NULL);
		return hr;
	}

	// Create the RasterizerState
	D3D11_RASTERIZER_DESC descRasterizer;
	descRasterizer.FillMode = D3D11_FILL_SOLID;
	descRasterizer.CullMode = D3D11_CULL_NONE;
	descRasterizer.FrontCounterClockwise = FALSE;
	descRasterizer.DepthBias = FALSE;
	descRasterizer.DepthBiasClamp = FALSE;
	descRasterizer.SlopeScaledDepthBias = FALSE;
	descRasterizer.DepthClipEnable = FALSE;
	descRasterizer.ScissorEnable = FALSE;
	descRasterizer.MultisampleEnable = FALSE;
	descRasterizer.AntialiasedLineEnable = FALSE;
	hr = _pd3dDevice->CreateRasterizerState(&descRasterizer, &_pRasterizerState);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create Rasterizer State", "Renderer", NULL);
		return hr;
	}

	// Create the Viewport
	D3D11_VIEWPORT vp;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Width = (FLOAT)_width;
	vp.Height = (FLOAT)_height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	_pImmediateContext->OMSetRenderTargets(1, &_pRenderTargetView, _pDepthStencilView);
	_pImmediateContext->OMSetDepthStencilState(_pDepthStencilState, 0);
	_pImmediateContext->OMSetBlendState(_pBlendState, NULL, 0xFFFFFFFF);
	_pImmediateContext->RSSetState(_pRasterizerState);
	_pImmediateContext->RSSetViewports(1, &vp);

	return S_OK;
}

void engine::Renderer::clear(void)
{
	_pImmediateContext->ClearRenderTargetView(_pRenderTargetView, DirectX::Colors::Transparent);
	_pImmediateContext->ClearDepthStencilView(_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void engine::Renderer::executeDeferredContext(ID3D11DeviceContext *context)
{
	ID3D11CommandList *commandList;

	context->FinishCommandList(TRUE, &commandList);
	_pImmediateContext->ExecuteCommandList(commandList, TRUE);

	commandList->Release();
}

void engine::Renderer::setDisplayFunc(void (*f) (void))
{
	_display = f;
}

void engine::Renderer::setIdleFunc(void (*f) (void))
{
	_idle = f;
}

void engine::Renderer::setReshapeFunc(void(*f) (UINT, UINT))
{
	_reshape = f;
}

UINT engine::Renderer::getWidth(void)
{
	return _width;
}

UINT engine::Renderer::getHeight(void)
{
	return _height;
}

HINSTANCE engine::Renderer::getHINSTANCE(void)
{
	return _hInst;
}

HWND engine::Renderer::getHWND(void)
{
	return _hWnd;
}

ID3D11Device *engine::Renderer::getD3DDevice(void)
{
	return _pd3dDevice;
}

ID3D11DeviceContext *engine::Renderer::getImmediateContext(void)
{
	return _pImmediateContext;
}

void engine::Renderer::mainLoop(int nCmdShow)
{
	MSG msg = { 0 };

	if (!_reshape || !_idle || !_display)
	{
		MessageBox(NULL, "You need to set the Reshape, Idle and Display Function before", "Renderer", MB_OK);
		return;
	}

	ShowWindow(_hWnd, nCmdShow);
	_stopLoop = FALSE;
	_reshape(_width, _height);
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

			_idle();
			_display();

			_pSwapChain->Present(1, 0);
		}
	}
}

void engine::Renderer::stopLoop(void)
{
	_stopLoop = TRUE;
}