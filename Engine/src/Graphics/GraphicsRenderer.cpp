#include <Engine\Graphics\GraphicsRenderer.h>

static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	
	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

Engine::Graphics::GraphicsRenderer &Engine::Graphics::GraphicsRenderer::Instance(void)
{
	static GraphicsRenderer instance;
	return instance;
}

Engine::Graphics::GraphicsRenderer::GraphicsRenderer(void) :
	_stopLoop(FALSE), _width(0), _height(0),
	_hWnd(NULL), _pSwapChain(NULL), _pRenderTargetView(NULL), _pDepthStencilState(NULL), _pBlendState(NULL), _pRasterizerState(NULL),
	_VP({ 0 }), _shaderLevel("")
{
}

Engine::Graphics::GraphicsRenderer::~GraphicsRenderer(void)
{
	if (_pSwapChain) _pSwapChain->Release();
	if (_pRenderTargetView) _pRenderTargetView->Release();
	if (_pDepthStencilState) _pDepthStencilState->Release();
	if (_pBlendState) _pBlendState->Release();
	if (_pRasterizerState) _pRasterizerState->Release();

	if (_hWnd) DestroyWindow(_hWnd);
}

ULONG64 Engine::Graphics::GraphicsRenderer::getTime(void) const
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

std::string Engine::Graphics::GraphicsRenderer::getShaderLevel(const D3D_FEATURE_LEVEL &featureLevel) const
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

void Engine::Graphics::GraphicsRenderer::init(const LPCWSTR szTitle, const UINT &width, const UINT &height, const BOOL &fullScreen, HINSTANCE hInstance, int nCmdShow)
{
	if (_pSwapChain) _pSwapChain->Release();
	if (_pRenderTargetView) _pRenderTargetView->Release();
	if (_pDepthStencilState) _pDepthStencilState->Release();
	if (_pBlendState) _pBlendState->Release();
	if (_pRasterizerState) _pRasterizerState->Release();

	if (_hWnd) DestroyWindow(_hWnd);

	_width = width;
	_height = height;

	Logger::Instance().debug(L"GraphicsRenderer", L"Init the GraphicsRenderer (width=" + std::to_wstring(_width) + L':' +
		L"height=" + std::to_wstring(height) + L':' +
		L"fullscreen=" + std::to_wstring(fullScreen) + L')');
	
	// Create the Window
	WNDCLASS wcex;
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(hInstance, IDC_ARROW);
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = TEXT("EngineWindowClass");

	RegisterClass(&wcex);

	_hWnd = CreateWindow(wcex.lpszClassName, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, _width, _height, NULL, NULL, hInstance, NULL);
	if (_hWnd == NULL)
	{
		MessageBox(NULL, TEXT("Failed to create Window"), TEXT(__FILE__), MB_OK);
		abort();
	}

	// Create Device and SwapChain
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
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	sd.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	DXGI_SWAP_CHAIN_FULLSCREEN_DESC sfd;
	sfd.RefreshRate.Numerator = 60;
	sfd.RefreshRate.Denominator = 1;
	sfd.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sfd.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sfd.Windowed = !fullScreen;

	D3D_FEATURE_LEVEL featureLevels[]
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Device *pd3dDevice;
	ID3D11DeviceContext *pd3dDeviceContext;
	D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &pd3dDevice, &featureLevel, &pd3dDeviceContext);
	pd3dDevice->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(&_pDevice));
	pd3dDevice->Release();
	pd3dDeviceContext->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&_pDeviceContext));
	pd3dDeviceContext->Release();

	IDXGIFactory1 *pFactory1;
	IDXGIFactory2 *pFactory2;
	CreateDXGIFactory1(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&pFactory1));
	pFactory1->QueryInterface(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&pFactory2));
	pFactory1->Release();
	pFactory2->CreateSwapChainForHwnd(_pDevice, _hWnd, &sd, &sfd, NULL, &_pSwapChain);
	pFactory2->Release();

	_shaderLevel = this->getShaderLevel(featureLevel);

	// Create the RenderTargetView
	ID3D11Texture2D *texture;
	_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&texture));
	_pDevice->CreateRenderTargetView(texture, NULL, &_pRenderTargetView);
	texture->Release();

	// Create the DepthStencilState
	D3D11_DEPTH_STENCIL_DESC descDepth;
	descDepth.DepthEnable = FALSE;
	descDepth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	descDepth.StencilEnable = FALSE;
	_pDevice->CreateDepthStencilState(&descDepth, &_pDepthStencilState);

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
	_pDevice->CreateBlendState(&descBlend, &_pBlendState);

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
	_pDevice->CreateRasterizerState(&descRasterizer, &_pRasterizerState);

	// Create the Viewport
	_VP.TopLeftX = 0.0f;
	_VP.TopLeftY = 0.0f;
	_VP.Width = (FLOAT)_width;
	_VP.Height = (FLOAT)_height;
	_VP.MinDepth = 0.0f;
	_VP.MaxDepth = 1.0f;

	ShowWindow(_hWnd, nCmdShow);
}

void Engine::Graphics::GraphicsRenderer::setState(void) const
{
	_pDeviceContext->OMSetRenderTargets(1, &_pRenderTargetView, NULL);
	_pDeviceContext->OMSetDepthStencilState(_pDepthStencilState, 0);
	_pDeviceContext->OMSetBlendState(_pBlendState, NULL, 0xffffffff);
	_pDeviceContext->RSSetState(_pRasterizerState);
	_pDeviceContext->RSSetViewports(1, &_VP);
}

ID3D11Device1 *Engine::Graphics::GraphicsRenderer::getDevice(void) const
{
	return _pDevice;
}

ID3D11DeviceContext1 *Engine::Graphics::GraphicsRenderer::getDeviceContext(void) const
{
	return _pDeviceContext;
}

std::string Engine::Graphics::GraphicsRenderer::getShaderLevel(void) const
{
	return _shaderLevel;
}

void Engine::Graphics::GraphicsRenderer::mainLoop(const std::shared_ptr<GameLoop> &gameLoop)
{
	if (gameLoop.get() == NULL)
	{
		std::invalid_argument ex("The GameLoop is invalid");
		Logger::Instance().error(L"GraphicsRenderer", ex);

		throw ex;
	}

	MSG msg;
	ULONG64 startTime, currentTime, newTime, frameTime;
	ULONG64 accumulator = 0;
	ULONG64 dt = 16;

	_stopLoop = FALSE;

	gameLoop->reshape(_width, _height);

	startTime = this->getTime();
	currentTime = this->getTime() - startTime;

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
		
		newTime = this->getTime() - startTime;
		frameTime = newTime - currentTime;
		currentTime = newTime;

		accumulator += frameTime;
		if (accumulator >= dt)
		{
			while (accumulator >= dt)
			{
				gameLoop->state(currentTime);
				accumulator -= dt;
			}
			gameLoop->lastState();
		}

		gameLoop->display(static_cast<FLOAT>(accumulator) / dt);
		_pSwapChain->Present(0, 0);
	}
}

void Engine::Graphics::GraphicsRenderer::clear(void) const
{
	_pDeviceContext->ClearRenderTargetView(_pRenderTargetView, Colors::Blue);
}