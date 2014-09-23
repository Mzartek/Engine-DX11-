#include <Engine/Renderer.hpp>

D3D_FEATURE_LEVEL FeatureLevel;
std::string ShaderLevel;
ID3D11Device *Device;
ID3D11DeviceContext *DeviceContext; 

std::vector<IDXGIAdapter*> EnumerateAdapters(void)
{
	HRESULT hr;
	IDXGIFactory *pFactory;
	IDXGIAdapter *pAdapter;
	std::vector <IDXGIAdapter*> vAdapters;

	// Create a DXGIFactory object.
	hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Failed to enumerate Adapters", TEXT(__FILE__), MB_OK);
	}

	for (UINT i = 0; pFactory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND; i++)
	{
		vAdapters.push_back(pAdapter);
	}
	pFactory->Release();

	return vAdapters;
}

static std::string checkShaderVersion(const D3D_FEATURE_LEVEL &featureLevel)
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
	HRESULT hr;
	ID3D11Texture2D *texture;

	_width = width;
	_height = height;
	_hInst = hInstance;

	if (DeviceContext) DeviceContext->ClearState();
	if (_pSwapChain) _pSwapChain->SetFullscreenState(FALSE, NULL);

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
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"EngineWindowClass";

	RegisterClass(&wcex);
	_hWnd = CreateWindow(wcex.lpszClassName, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, _width, _height, NULL, NULL, hInstance, NULL);
	if (!_hWnd)
	{
		MessageBox(NULL, L"Failed to create Window", TEXT(__FILE__), MB_OK);
		exit(1);
	}
	ShowWindow(_hWnd, nCmdShow);

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
	sd.BufferCount = 2;
	sd.OutputWindow = _hWnd;
	sd.Windowed = !fullScreen;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// Create Device and SwapChain
	std::vector<IDXGIAdapter *> vAdapters = EnumerateAdapters();
	D3D_FEATURE_LEVEL featureLevels[]
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	hr = D3D11CreateDeviceAndSwapChain(vAdapters[0], D3D_DRIVER_TYPE_UNKNOWN, NULL, NULL, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION,
		&sd, &_pSwapChain, &Device, &FeatureLevel, &DeviceContext);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Failed to create Device and SwapChain", TEXT(__FILE__), MB_OK);
		exit(1);
	}
	ShaderLevel = checkShaderVersion(FeatureLevel);
	// Release Adapters List
	for (UINT i = 0; i < vAdapters.size(); i++)
		vAdapters[i]->Release();

	// Create the RenderTargetView
	hr = _pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void **)&texture);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Failed to get BackBuffer", TEXT(__FILE__), MB_OK);
		exit(1);
	}
	hr = Device->CreateRenderTargetView(texture, NULL, &_pRenderTargetView);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Failed to create RenderTarget View", TEXT(__FILE__), MB_OK);
		exit(1);
	}
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
	hr = Device->CreateTexture2D(&descDepthStencilTexture, NULL, &texture);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Failed to create DepthStencil Texture", TEXT(__FILE__), MB_OK);
		exit(1);
	}

	// Create the DepthStencilView
	D3D11_DEPTH_STENCIL_VIEW_DESC descDepthStencilView;
	descDepthStencilView.Format = descDepthStencilTexture.Format;
	descDepthStencilView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDepthStencilView.Flags = 0;
	descDepthStencilView.Texture2D.MipSlice = 0;
	hr = Device->CreateDepthStencilView(texture, &descDepthStencilView, &_pDepthStencilView);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Failed to create DepthStencil View", TEXT(__FILE__), MB_OK);
		exit(1);
	}
	texture->Release();

	// Create the DepthStencilState
	D3D11_DEPTH_STENCIL_DESC descDepth;
	descDepth.DepthEnable = FALSE;
	descDepth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	descDepth.StencilEnable = FALSE;
	hr = Device->CreateDepthStencilState(&descDepth, &_pDepthStencilState);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Failed to create DepthStencil State", TEXT(__FILE__), MB_OK);
		exit(1);
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
	hr = Device->CreateBlendState(&descBlend, &_pBlendState);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Failed to create Blend State", TEXT(__FILE__), MB_OK);
		exit(1);
	}

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
	hr = Device->CreateRasterizerState(&descRasterizer, &_pRasterizerState);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Failed to create Rasterizer State", TEXT(__FILE__), MB_OK);
		exit(1);
	}

	// Create the Viewport
	_VP.TopLeftX = 0.0f;
	_VP.TopLeftY = 0.0f;
	_VP.Width = (FLOAT)_width;
	_VP.Height = (FLOAT)_height;
	_VP.MinDepth = 0.0f;
	_VP.MaxDepth = 1.0f;
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

void engine::Renderer::mainLoop(void)
{
	MSG msg;

	if (!_reshape || !_idle || !_display)
	{
		MessageBox(NULL, L"You need to set the Reshape, Idle and Display Function before", TEXT(__FILE__), MB_OK);
		return;
	}

	_stopLoop = FALSE;
	_reshape(_width, _height);
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

		_idle();
		_display();

		_pSwapChain->Present(1, 0);
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