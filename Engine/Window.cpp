#include <Engine/Window.hpp>

engine::Window::Window(void)
{
	_driverType = D3D_DRIVER_TYPE_NULL;
	_featureLevel = D3D_FEATURE_LEVEL_11_1;
	_display = NULL;
	_idle = NULL;
	_reshape = NULL;
	_keyboard = NULL;
	_mouseMove = NULL;
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
	if (_pImmediateContext1) 
		_pImmediateContext1->Release();
	if (_pImmediateContext) 
		_pImmediateContext->Release();
	if (_pd3dDevice1) 
		_pd3dDevice1->Release();
	if (_pd3dDevice) 
		_pd3dDevice->Release();
}

HRESULT engine::Window::initWindow(const HINSTANCE hInstance, LRESULT(CALLBACK *WndProc) (HWND, UINT, WPARAM, LPARAM), const TCHAR *szWindowClass, const TCHAR *szTitle, UINT width, UINT height)
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
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = _hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	// Create the Device
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);
	hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, featureLevels, numFeatureLevels, D3D11_SDK_VERSION,
		&sd, &_pSwapChain, &_pd3dDevice, &_featureLevel, &_pImmediateContext);
	if (FAILED(hr))
		return hr;

	// Obtain the Direct3D 11.1 versions if available
	hr = _pd3dDevice->QueryInterface(__uuidof(ID3D11Device1), (void **)&_pd3dDevice1);
	if (SUCCEEDED(hr))
	{
		_pImmediateContext->QueryInterface(__uuidof(ID3D11DeviceContext1), (void **)&_pImmediateContext1);
	}

	// Create a render target view
	ID3D11Texture2D *pBackBuffer = NULL;
	hr = _pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void **)&pBackBuffer);
	if (FAILED(hr))
		return hr;
	hr = _pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &_pRenderTargetView);
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
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = _pd3dDevice->CreateTexture2D(&descDepth, NULL, &pDepthStencil);
	if (FAILED(hr))
		return hr;

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = _pd3dDevice->CreateDepthStencilView(pDepthStencil, &descDSV, &_pDepthStencilView);
	if (FAILED(hr))
		return hr;

	// Create rasterizerState
	ID3D11RasterizerState1 *pRasterState = NULL;
	D3D11_RASTERIZER_DESC1 rasterizerState;
	ZeroMemory(&rasterizerState, sizeof(rasterizerState));
	rasterizerState.FillMode = D3D11_FILL_SOLID;
	rasterizerState.CullMode = D3D11_CULL_NONE;
	rasterizerState.FrontCounterClockwise = true;
	rasterizerState.DepthBias = false;
	rasterizerState.DepthBiasClamp = 0;
	rasterizerState.SlopeScaledDepthBias = 0;
	rasterizerState.DepthClipEnable = true;
	rasterizerState.ScissorEnable = false;
	rasterizerState.MultisampleEnable = false;
	rasterizerState.AntialiasedLineEnable = false;
	hr = _pd3dDevice1->CreateRasterizerState1(&rasterizerState, &pRasterState);
	if (FAILED(hr))
		return hr;

	// Create blendState
	ID3D11BlendState1 *pBlendState = NULL;
	D3D11_BLEND_DESC1 blendState;
	ZeroMemory(&blendState, sizeof(blendState));
	blendState.AlphaToCoverageEnable = false;
	blendState.IndependentBlendEnable = false;
	blendState.RenderTarget[0].BlendEnable = true;
	blendState.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendState.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendState.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendState.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	blendState.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
	blendState.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendState.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	_pd3dDevice1->CreateBlendState1(&blendState, &pBlendState);
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
	_pImmediateContext->OMSetBlendState(pBlendState, NULL, 0xffffffff);
	_pImmediateContext->RSSetState(pRasterState);
	_pImmediateContext->RSSetViewports(1, &vp);

	pBackBuffer->Release();
	pDepthStencil->Release();
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

void engine::Window::setKeyboardFunc(void(*f) (UCHAR, BOOL))
{
	_keyboard = f;
}

void engine::Window::setMouseMoveFunc(void(*f) (INT, INT))
{
	_mouseMove = f;
}

UINT engine::Window::getWidth(void)
{
	return _width;
}

UINT engine::Window::getHeight(void)
{
	return _height;
}

ID3D11Device *engine::Window::getD3DDevice(void)
{
	return _pd3dDevice;
}

ID3D11DeviceContext *engine::Window::getImmediateContext(void)
{
	return _pImmediateContext;
}

IDXGISwapChain *engine::Window::getSwapChain(void)
{
	return _pSwapChain;
}

ID3D11RenderTargetView *engine::Window::getRenderTargetView(void)
{
	return _pRenderTargetView;
}

ID3D11DepthStencilView *engine::Window::getDepthStencilView(void)
{
	return _pDepthStencilView;
}

void engine::Window::mainLoop(int nCmdShow)
{
	ShowWindow(_hWnd, nCmdShow);

	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			if (_idle)
				_idle();
			if (_display)
				_display();
		}
	}
}

void engine::Window::clear(void)
{
	_pImmediateContext->ClearRenderTargetView(_pRenderTargetView, DirectX::Colors::Black);
	_pImmediateContext->ClearDepthStencilView(_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}