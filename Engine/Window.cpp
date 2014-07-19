#include <Engine/Window.hpp>

engine::Window::Window(void)
{
	_pd3dDevice = NULL;
	_pImmediateContext = NULL;
	_pSwapChain = NULL;
	_pRenderTargetView = NULL;
	_pDepthStencilView = NULL;
	_display = NULL;
	_idle = NULL;
	_reshape = NULL;
}

engine::Window::~Window(void)
{
	if (_pImmediateContext)
		_pImmediateContext->ClearState();

	if (_pDepthStencilView)
		_pDepthStencilView->Release();
	if (_pRenderTargetView)
		_pRenderTargetView->Release();
	if (_pSwapChain)
		_pSwapChain->Release();
	if (_pImmediateContext)
		_pImmediateContext->Release();
	if (_pd3dDevice) 
		_pd3dDevice->Release();
}

HRESULT engine::Window::initWindow(const HINSTANCE &hInstance, LRESULT(CALLBACK *WndProc) (HWND, UINT, WPARAM, LPARAM), const TCHAR *szWindowClass, 
	const TCHAR *szTitle, const UINT &width, const UINT &height, const BOOL &fullScreen)
{
	HRESULT hr;
	_width = width;
	_height = height;

	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL, _T("Call to RegisterClassEx failed!"), _T("Win32 Guided Tour"), NULL);
		return E_FAIL;
	}

	_hInst = hInstance;
	_hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, hInstance, NULL);

	if (!_hWnd)
	{
		MessageBox(NULL, _T("Call to CreateWindow failed!"), _T("Win32 Guided Tour"), NULL);
		return E_FAIL;
	}

	// Init swap chain
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = _hWnd;
	if (fullScreen)
		sd.Windowed = FALSE;
	else
		sd.Windowed = TRUE;

	// Create the Device
	hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL, D3D11_SDK_VERSION,
		&sd, &_pSwapChain, &_pd3dDevice, NULL, &_pImmediateContext);
	if (FAILED(hr))
		return hr;

	// Create a render target view
	ID3D11Texture2D *pBackBuffer = NULL;
	hr = _pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void **)&pBackBuffer);
	if (FAILED(hr))
		return hr;
	hr = _pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &_pRenderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr))
		return hr;

	// Create depth stencil texture
	ID3D11Texture2D *pDepthStencil = NULL;
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	hr = _pd3dDevice->CreateTexture2D(&descDepth, NULL, &pDepthStencil);
	if (FAILED(hr))
		return hr;

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Flags = 0;
	descDSV.Texture2D.MipSlice = 0;
	hr = _pd3dDevice->CreateDepthStencilView(pDepthStencil, &descDSV, &_pDepthStencilView);
	pDepthStencil->Release();
	if (FAILED(hr))
		return hr;

	// Create rasterizerState
	ID3D11RasterizerState *pRasterState = NULL;
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
	hr = _pd3dDevice->CreateRasterizerState(&descRasterizer, &pRasterState);
	if (FAILED(hr))
		return hr;

	// Create blendState
	ID3D11BlendState *pBlendState = NULL;
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
	_pd3dDevice->CreateBlendState(&descBlend, &pBlendState);
	if (FAILED(hr))
		return hr;

	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	_pImmediateContext->OMSetRenderTargets(1, &_pRenderTargetView, _pDepthStencilView);
	_pImmediateContext->OMSetBlendState(pBlendState, NULL, 0xFFFFFFFF);
	_pImmediateContext->RSSetState(pRasterState);
	_pImmediateContext->RSSetViewports(1, &vp);

	pRasterState->Release();
	pBlendState->Release();

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
	ShowWindow(_hWnd, nCmdShow);

	MSG msg = { 0 };
	_stopLoop = FALSE;
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
		}
		_pSwapChain->Present(0, 0);
	}
}

void engine::Window::stopLoop(void)
{
	_stopLoop = TRUE;
}

void engine::Window::clear(void)
{
	_pImmediateContext->ClearRenderTargetView(_pRenderTargetView, DirectX::Colors::Black);
	_pImmediateContext->ClearDepthStencilView(_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}