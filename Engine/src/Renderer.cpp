#include <Engine/Renderer.hpp>

D3D_FEATURE_LEVEL FeatureLevel;
std::string ShaderLevel;
ID3D11Device1 *Device;
ID3D11DeviceContext1 *DeviceContext;

static std::string ShaderVersion(const D3D_FEATURE_LEVEL &featureLevel)
{
	std::string shaderLevel;

	switch (featureLevel)
	{
	case D3D_FEATURE_LEVEL_11_1:
		shaderLevel = "5_0";
		break;
	case D3D_FEATURE_LEVEL_11_0:
		shaderLevel = "5_0";
		break;
	case D3D_FEATURE_LEVEL_10_1:
		shaderLevel = "4_1";
		break;
	case D3D_FEATURE_LEVEL_10_0:
		shaderLevel = "4_0";
		break;
	default:
		shaderLevel = "unknown";
		break;
	}
	return shaderLevel;
}

engine::Renderer::Renderer(void)
	: _hWnd(NULL), _pSwapChain(NULL), 
	_pRenderTargetView(NULL), _pDepthStencilView(NULL), 
	_pDepthStencilState(NULL), _pBlendState(NULL), _pRasterizerState(NULL), 
	_display(NULL), _idle(NULL), _reshape(NULL)
{
}

engine::Renderer::~Renderer(void)
{
	// Device
	if (Device) Device->Release();
	if (DeviceContext) DeviceContext->Release();
	// SwapChain
	if (_pSwapChain) _pSwapChain->Release();
	// View
	if (_pRenderTargetView) _pRenderTargetView->Release();
	if (_pDepthStencilView) _pDepthStencilView->Release();
	// State
	if (_pDepthStencilState) _pDepthStencilState->Release();
	if (_pBlendState) _pBlendState->Release();
	if (_pRasterizerState) _pRasterizerState->Release();
}

void engine::Renderer::initWindow(const HINSTANCE &hInstance, LRESULT(CALLBACK *WndProc) (HWND, UINT, WPARAM, LPARAM), const int &nCmdShow,
	const TCHAR *szTitle, const UINT &width, const UINT &height, const BOOL &fullScreen)
{
	ID3D11Texture2D *texture;

	_width = width;
	_height = height;
	_hInst = hInstance;

	// Device
	if (Device) Device->Release();
	if (DeviceContext) DeviceContext->Release();
	// SwapChain
	if (_pSwapChain) _pSwapChain->Release();
	// View
	if (_pRenderTargetView) _pRenderTargetView->Release();
	if (_pDepthStencilView) _pDepthStencilView->Release();
	// State
	if (_pDepthStencilState) _pDepthStencilState->Release();
	if (_pBlendState) _pBlendState->Release();
	if (_pRasterizerState) _pRasterizerState->Release();

	WNDCLASS wcex;
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = _hInst;
	wcex.hIcon = LoadIcon(_hInst, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(_hInst, IDC_ARROW);
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = TEXT("EngineWindowClass");
	RegisterClass(&wcex);
	_hWnd = CreateWindow(wcex.lpszClassName, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, _width, _height, NULL, NULL, hInstance, NULL);
	if (_hWnd == NULL)
	{
		MessageBox(NULL, TEXT("Failed to create Window"), TEXT(__FILE__), MB_OK);
		exit(1);
	}
	ShowWindow(_hWnd, nCmdShow);

	// Create Device and SwapChain
	ID3D11Device *pd3dDevice;
	ID3D11DeviceContext *pd3dDeviceContext;
	IDXGIFactory1 *pFactory1;
	IDXGIFactory2 *pFactory2;
	D3D_FEATURE_LEVEL featureLevels[]
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	DXGI_SWAP_CHAIN_DESC1 sd;
	sd.Width = _width;
	sd.Height = _height;
	sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.Stereo = FALSE;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 2;
	sd.Scaling = DXGI_SCALING_STRETCH;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	DXGI_SWAP_CHAIN_FULLSCREEN_DESC sfd;
	sfd.RefreshRate.Numerator = 60;
	sfd.RefreshRate.Denominator = 1;
	sfd.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sfd.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sfd.Windowed = !fullScreen;

	D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &pd3dDevice, &FeatureLevel, &pd3dDeviceContext);
	pd3dDevice->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(&Device));
	pd3dDeviceContext->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&DeviceContext));
	CreateDXGIFactory1(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&pFactory1));
	pFactory1->QueryInterface(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&pFactory2));
	pFactory2->CreateSwapChainForHwnd(Device, _hWnd, &sd, &sfd, NULL, &_pSwapChain);

	pd3dDevice->Release();
	pd3dDeviceContext->Release();
	pFactory1->Release();
	pFactory2->Release();

	ShaderLevel = ShaderVersion(FeatureLevel);

	// Create the RenderTargetView
	_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&texture));
	Device->CreateRenderTargetView(texture, NULL, &_pRenderTargetView);
	texture->Release();

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
	Device->CreateTexture2D(&descDepthStencilTexture, NULL, &texture);

	// Create the DepthStencilView
	D3D11_DEPTH_STENCIL_VIEW_DESC descDepthStencilView;
	descDepthStencilView.Format = descDepthStencilTexture.Format;
	descDepthStencilView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDepthStencilView.Flags = 0;
	descDepthStencilView.Texture2D.MipSlice = 0;
	Device->CreateDepthStencilView(texture, &descDepthStencilView, &_pDepthStencilView);
	texture->Release();

	// Create the DepthStencilState
	D3D11_DEPTH_STENCIL_DESC descDepth;
	descDepth.DepthEnable = FALSE;
	descDepth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	descDepth.StencilEnable = FALSE;
	Device->CreateDepthStencilState(&descDepth, &_pDepthStencilState);

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
	Device->CreateBlendState(&descBlend, &_pBlendState);

	// Create the RasterizerState
	D3D11_RASTERIZER_DESC descRasterizer;
	descRasterizer.FillMode = D3D11_FILL_SOLID;
	descRasterizer.CullMode = D3D11_CULL_BACK;
	descRasterizer.FrontCounterClockwise = TRUE;
	descRasterizer.DepthBias = 0;
	descRasterizer.DepthBiasClamp = 0.0f;
	descRasterizer.SlopeScaledDepthBias = 0.0f;
	descRasterizer.DepthClipEnable = TRUE;
	descRasterizer.ScissorEnable = FALSE;
	descRasterizer.MultisampleEnable = FALSE;
	descRasterizer.AntialiasedLineEnable = FALSE;
	Device->CreateRasterizerState(&descRasterizer, &_pRasterizerState);

	// Create the Viewport
	_VP.TopLeftX = 0.0f;
	_VP.TopLeftY = 0.0f;
	_VP.Width = (FLOAT)_width;
	_VP.Height = (FLOAT)_height;
	_VP.MinDepth = 0.0f;
	_VP.MaxDepth = 1.0f;
}

void engine::Renderer::setDisplayFunc(void(*f) (FLOAT))
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

static long long milliseconds_now()
{
	static LARGE_INTEGER s_frequency;
	static BOOL s_use_qpc = QueryPerformanceFrequency(&s_frequency);
	if (s_use_qpc)
	{
		LARGE_INTEGER now;
		QueryPerformanceCounter(&now);
		return (1000LL * now.QuadPart) / s_frequency.QuadPart;
	}
	else
		return GetTickCount();
}

void engine::Renderer::mainLoop(void)
{
	MSG msg;
	long long currentTime, newTime, frameTime;
	long long accumulator = 0;
	long long dt = 16;

	if (!_reshape || !_idle || !_display)
	{
		MessageBox(NULL, TEXT("You need to set the Reshape, Idle and Display Function before"), TEXT(__FILE__), MB_OK);
		return;
	}

	_stopLoop = FALSE;
	_reshape(_width, _height);
	currentTime = milliseconds_now();
	while (!_stopLoop)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			switch (msg.message)
			{
			case WM_QUIT:
				_stopLoop = TRUE;
				break;
			}
		}
		newTime = milliseconds_now();
		frameTime = newTime - currentTime;
		currentTime = newTime;
		for (accumulator += frameTime; accumulator >= dt; accumulator -= dt)
			_idle();
		_display((FLOAT)accumulator / dt);
		_pSwapChain->Present(0, 0);
	}
}

void engine::Renderer::stopLoop(void)
{
	_stopLoop = TRUE;
}

void engine::Renderer::setState(void) const
{
	DeviceContext->OMSetRenderTargets(1, &_pRenderTargetView, _pDepthStencilView);
	DeviceContext->OMSetDepthStencilState(_pDepthStencilState, 0);
	DeviceContext->OMSetBlendState(_pBlendState, NULL, 0xFFFFFFFF);
	DeviceContext->RSSetState(_pRasterizerState);
	DeviceContext->RSSetViewports(1, &_VP);
}

void engine::Renderer::clear(void)
{
	DeviceContext->ClearRenderTargetView(_pRenderTargetView, Colors::Transparent);
	DeviceContext->ClearDepthStencilView(_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}