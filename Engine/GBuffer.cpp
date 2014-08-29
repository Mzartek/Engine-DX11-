#include <Engine/GBuffer.hpp>

engine::GBuffer::GBuffer(void)
{
	// Shader Resouce View
	_pResourceTexture[GBUF_NORMAL] = NULL;
	_pResourceTexture[GBUF_MATERIAL] = NULL;
	_pResourceTexture[GBUF_DEPTH_STENCIL] = NULL;
	_pShaderResourceView[GBUF_NORMAL] = NULL;
	_pShaderResourceView[GBUF_MATERIAL] = NULL;
	_pShaderResourceView[GBUF_DEPTH_STENCIL] = NULL;
	// View
	_pRenderTexture[GBUF_NORMAL] = NULL;
	_pRenderTexture[GBUF_MATERIAL] = NULL;
	_pRenderTexture[GBUF_DEPTH_STENCIL] = NULL;
	_pRenderTargetView[GBUF_NORMAL] = NULL;
	_pRenderTargetView[GBUF_MATERIAL] = NULL;
	_pDepthStencilView = NULL;
	// State
	_pDepthState = NULL;
	_pBlendState = NULL;
	_pRasterizerState = NULL;
}

engine::GBuffer::~GBuffer(void)
{
	// State
	if (_pRasterizerState)
		_pRasterizerState->Release();
	if (_pBlendState)
		_pBlendState->Release();
	if (_pDepthState)
		_pDepthState->Release();

	// View
	if (_pDepthStencilView)
		_pDepthStencilView->Release();
	if (_pRenderTargetView[GBUF_MATERIAL])
		_pRenderTargetView[GBUF_MATERIAL]->Release();
	if (_pRenderTargetView[GBUF_NORMAL])
		_pRenderTargetView[GBUF_NORMAL]->Release();
	if (_pRenderTexture[GBUF_DEPTH_STENCIL])
		_pRenderTexture[GBUF_DEPTH_STENCIL]->Release();
	if (_pRenderTexture[GBUF_MATERIAL])
		_pRenderTexture[GBUF_MATERIAL]->Release();
	if (_pRenderTexture[GBUF_NORMAL])
		_pRenderTexture[GBUF_NORMAL]->Release();

	// Shader Resource View
	if (_pShaderResourceView[GBUF_DEPTH_STENCIL])
		_pShaderResourceView[GBUF_DEPTH_STENCIL]->Release();
	if (_pShaderResourceView[GBUF_MATERIAL])
		_pShaderResourceView[GBUF_MATERIAL]->Release();
	if (_pShaderResourceView[GBUF_NORMAL])
		_pShaderResourceView[GBUF_NORMAL]->Release();
	if (_pResourceTexture[GBUF_DEPTH_STENCIL])
		_pResourceTexture[GBUF_DEPTH_STENCIL]->Release();
	if (_pResourceTexture[GBUF_MATERIAL])
		_pResourceTexture[GBUF_MATERIAL]->Release();
	if (_pResourceTexture[GBUF_NORMAL])
		_pResourceTexture[GBUF_NORMAL]->Release();
}

void engine::GBuffer::config(const UINT &width, const UINT &height, ID3D11Device *pd3dDevice, ID3D11DeviceContext *pContext)
{
	HRESULT hr;

	_width = width;
	_height = height;
	_pd3dDevice = pd3dDevice;
	_pContext = pContext;

	hr = _pd3dDevice->CreateDeferredContext(0, &_pDeferredContext);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create Deferred Context", "GBuffer", NULL);
		exit(1);
	}

	D3D11_TEXTURE2D_DESC descTexture;
	descTexture.Width = _width;
	descTexture.Height = _height;
	descTexture.MipLevels = 1;
	descTexture.ArraySize = 1;
	descTexture.SampleDesc.Count = 1;
	descTexture.SampleDesc.Quality = 0;
	descTexture.Usage = D3D11_USAGE_DEFAULT;
	descTexture.CPUAccessFlags = 0;
	descTexture.MiscFlags = 0;

	D3D11_SHADER_RESOURCE_VIEW_DESC descShaderResourceView;
	descShaderResourceView.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	descShaderResourceView.Texture2D.MostDetailedMip = 0;
	descShaderResourceView.Texture2D.MipLevels = 1;

	D3D11_RENDER_TARGET_VIEW_DESC descRenderTargetView;
	descRenderTargetView.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	descRenderTargetView.Texture2D.MipSlice = 0;

	D3D11_DEPTH_STENCIL_VIEW_DESC descDepthView;
	descDepthView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDepthView.Flags = 0;
	descDepthView.Texture2D.MipSlice = 0;

	// Normal
	{
		//Format
		descTexture.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		descShaderResourceView.Format = descTexture.Format;
		descRenderTargetView.Format = descTexture.Format;

		// Create Resource
		descTexture.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		hr = _pd3dDevice->CreateTexture2D(&descTexture, NULL, &_pResourceTexture[GBUF_NORMAL]);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to create Normal Resource Texture", "GBuffer", NULL);
			exit(1);
		}
		hr = _pd3dDevice->CreateShaderResourceView(_pResourceTexture[GBUF_NORMAL], &descShaderResourceView, &_pShaderResourceView[GBUF_NORMAL]);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to create Normal Resource View", "GBuffer", NULL);
			exit(1);
		}

		// Create Render
		descTexture.BindFlags = D3D11_BIND_RENDER_TARGET;
		hr = _pd3dDevice->CreateTexture2D(&descTexture, NULL, &_pRenderTexture[GBUF_NORMAL]);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to create Normal Render Texture", "GBuffer", NULL);
			exit(1);
		}
		hr = _pd3dDevice->CreateRenderTargetView(_pRenderTexture[GBUF_NORMAL], &descRenderTargetView, &_pRenderTargetView[GBUF_NORMAL]);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to create Normal Render View", "GBuffer", NULL);
			exit(1);
		}
	}

	// Material
	{
		// Format
		descTexture.Format = DXGI_FORMAT_R32G32B32A32_UINT;
		descRenderTargetView.Format = descTexture.Format;
		descShaderResourceView.Format = descTexture.Format;

		// Create Resource
		descTexture.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		hr = _pd3dDevice->CreateTexture2D(&descTexture, NULL, &_pResourceTexture[GBUF_MATERIAL]);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to create Material Resource Texture", "GBuffer", NULL);
			exit(1);
		}
		hr = _pd3dDevice->CreateShaderResourceView(_pResourceTexture[GBUF_MATERIAL], &descShaderResourceView, &_pShaderResourceView[GBUF_MATERIAL]);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to create Material Resource View", "GBuffer", NULL);
			exit(1);
		}

		// Create Render
		descTexture.BindFlags = D3D11_BIND_RENDER_TARGET;
		hr = _pd3dDevice->CreateTexture2D(&descTexture, NULL, &_pRenderTexture[GBUF_MATERIAL]);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to create Material Render Texture", "GBuffer", NULL);
			exit(1);
		}
		hr = _pd3dDevice->CreateRenderTargetView(_pRenderTexture[GBUF_MATERIAL], &descRenderTargetView, &_pRenderTargetView[GBUF_MATERIAL]);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to create Material Render View", "GBuffer", NULL);
			exit(1);
		}
	}

	// Depth
	{
		// Format
		descTexture.Format = DXGI_FORMAT_R24G8_TYPELESS;
		descShaderResourceView.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		descDepthView.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

		// Create Resource
		descTexture.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		hr = _pd3dDevice->CreateTexture2D(&descTexture, NULL, &_pResourceTexture[GBUF_DEPTH_STENCIL]);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to create Depth Resource Texture", "GBuffer", NULL);
			exit(1);
		}
		hr = _pd3dDevice->CreateShaderResourceView(_pResourceTexture[GBUF_DEPTH_STENCIL], &descShaderResourceView, &_pShaderResourceView[GBUF_DEPTH_STENCIL]);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to create Depth Resource View", "GBuffer", NULL);
			exit(1);
		}

		// Create Render
		descTexture.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		hr = _pd3dDevice->CreateTexture2D(&descTexture, NULL, &_pRenderTexture[GBUF_DEPTH_STENCIL]);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to create Depth Render Texture", "GBuffer", NULL);
			exit(1);
		}
		hr = _pd3dDevice->CreateDepthStencilView(_pRenderTexture[GBUF_DEPTH_STENCIL], &descDepthView, &_pDepthStencilView);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to create Depth Render View", "GBuffer", NULL);
			exit(1);
		}
	}

	// State
	D3D11_DEPTH_STENCIL_DESC descDepth;
	descDepth.DepthEnable = TRUE;
	descDepth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	descDepth.DepthFunc = D3D11_COMPARISON_LESS;
	descDepth.StencilEnable = FALSE;
	hr = _pd3dDevice->CreateDepthStencilState(&descDepth, &_pDepthState);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create DepthStencil State", "GBuffer", NULL);
		exit(1);
	}

	D3D11_BLEND_DESC descBlend;
	descBlend.AlphaToCoverageEnable = FALSE;
	descBlend.IndependentBlendEnable = FALSE;
	for (int i = 0; i < (GBUF_NUM_TEX - 1); i++)
	{
		descBlend.RenderTarget[i].BlendEnable = FALSE;
		descBlend.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	}
	hr = _pd3dDevice->CreateBlendState(&descBlend, &_pBlendState);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create Blend State", "GBuffer", NULL);
		exit(1);
	}

	D3D11_RASTERIZER_DESC descRasterizer;
	descRasterizer.FillMode = D3D11_FILL_SOLID;
	descRasterizer.CullMode = D3D11_CULL_NONE;
	descRasterizer.FrontCounterClockwise = FALSE;
	descRasterizer.DepthBias = 0;
	descRasterizer.DepthBiasClamp = 0.0f;
	descRasterizer.SlopeScaledDepthBias = 0.0f;
	descRasterizer.DepthClipEnable = TRUE;
	descRasterizer.ScissorEnable = FALSE;
	descRasterizer.MultisampleEnable = FALSE;
	descRasterizer.AntialiasedLineEnable = FALSE;
	hr = _pd3dDevice->CreateRasterizerState(&descRasterizer, &_pRasterizerState);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create Rasterizer State", "GBuffer", NULL);
		exit(1);
	}

	// Create the Viewport
	D3D11_VIEWPORT vp;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Width = (FLOAT)_width;
	vp.Height = (FLOAT)_height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	_pDeferredContext->OMSetRenderTargets(GBUF_NUM_TEX - 1, _pRenderTargetView, _pDepthStencilView);
	_pDeferredContext->OMSetDepthStencilState(_pDepthState, 0);
	_pDeferredContext->OMSetBlendState(_pBlendState, NULL, 0xFFFFFFFF);
	_pDeferredContext->RSSetState(_pRasterizerState);
	_pDeferredContext->RSSetViewports(1, &vp);
}

ID3D11ShaderResourceView *engine::GBuffer::getShaderResourceView(const UINT &num) const
{
	return _pShaderResourceView[num];
}

void engine::GBuffer::enableDepthMask(const BOOL &mask)
{
	D3D11_DEPTH_STENCIL_DESC descDepth;
	if (_pDepthState == NULL)
	{
		MessageBox(NULL, "You need to configure the GBuffer before", "GBuffer", NULL);
		return;
	}

	_pDepthState->GetDesc(&descDepth);
	_pDepthState->Release();
	if (mask)
		descDepth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	else
		descDepth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	_pd3dDevice->CreateDepthStencilState(&descDepth, &_pDepthState);
	_pDeferredContext->OMSetDepthStencilState(_pDepthState, 0);
}

void engine::GBuffer::clear(void) const
{
	_pDeferredContext->ClearRenderTargetView(_pRenderTargetView[GBUF_NORMAL], Colors::Transparent);
	_pDeferredContext->ClearRenderTargetView(_pRenderTargetView[GBUF_MATERIAL], Colors::Transparent);
	_pDeferredContext->ClearDepthStencilView(_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void engine::GBuffer::actualizeResource(void) const
{
	_pContext->CopyResource(_pResourceTexture[GBUF_NORMAL], _pRenderTexture[GBUF_NORMAL]);
	_pContext->CopyResource(_pResourceTexture[GBUF_MATERIAL], _pRenderTexture[GBUF_MATERIAL]);
	_pContext->CopyResource(_pResourceTexture[GBUF_DEPTH_STENCIL], _pRenderTexture[GBUF_DEPTH_STENCIL]);
}