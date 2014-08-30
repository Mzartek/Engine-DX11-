#include <Engine/GBuffer.hpp>

engine::GBuffer::GBuffer(void)
{
	// Texture
	_pTextureA[GBUF_NORMAL] = NULL;
	_pTextureA[GBUF_MATERIAL] = NULL;
	_pTextureA[GBUF_DEPTH_STENCIL] = NULL;
	_pTextureB[GBUF_NORMAL] = NULL;
	_pTextureB[GBUF_MATERIAL] = NULL;
	_pTextureB[GBUF_DEPTH_STENCIL] = NULL;
	// Shader Resouce View
	_pShaderResourceViewA[GBUF_NORMAL] = NULL;
	_pShaderResourceViewA[GBUF_MATERIAL] = NULL;
	_pShaderResourceViewA[GBUF_DEPTH_STENCIL] = NULL;
	_pShaderResourceViewB[GBUF_NORMAL] = NULL;
	_pShaderResourceViewB[GBUF_MATERIAL] = NULL;
	_pShaderResourceViewB[GBUF_DEPTH_STENCIL] = NULL;
	// Render View
	_pRenderTargetViewA[GBUF_NORMAL] = NULL;
	_pRenderTargetViewA[GBUF_MATERIAL] = NULL;
	_pRenderTargetViewB[GBUF_NORMAL] = NULL;
	_pRenderTargetViewB[GBUF_MATERIAL] = NULL;
	// Depth View
	_pDepthStencilViewA = NULL;
	_pDepthStencilViewB = NULL;
	// State
	_pDepthStencilState = NULL;
	_pBlendState = NULL;
	_pRasterizerState = NULL;
	// Bool
	_buffer = 0;
}

engine::GBuffer::~GBuffer(void)
{
	// State
	if (_pRasterizerState)
		_pRasterizerState->Release();
	if (_pBlendState)
		_pBlendState->Release();
	if (_pDepthStencilState)
		_pDepthStencilState->Release();

	// Depth View
	if (_pDepthStencilViewB)
		_pDepthStencilViewB->Release();
	if (_pDepthStencilViewA)
		_pDepthStencilViewA->Release();

	// Render View
	if (_pRenderTargetViewB[GBUF_MATERIAL])
		_pRenderTargetViewB[GBUF_MATERIAL]->Release();
	if (_pRenderTargetViewB[GBUF_NORMAL])
		_pRenderTargetViewB[GBUF_NORMAL]->Release();
	if (_pRenderTargetViewA[GBUF_MATERIAL])
		_pRenderTargetViewA[GBUF_MATERIAL]->Release();
	if (_pRenderTargetViewA[GBUF_NORMAL])
		_pRenderTargetViewA[GBUF_NORMAL]->Release();

	// Shader Resource View
	if (_pShaderResourceViewB[GBUF_DEPTH_STENCIL])
		_pShaderResourceViewB[GBUF_DEPTH_STENCIL]->Release();
	if (_pShaderResourceViewB[GBUF_MATERIAL])
		_pShaderResourceViewB[GBUF_MATERIAL]->Release();
	if (_pShaderResourceViewB[GBUF_NORMAL])
		_pShaderResourceViewB[GBUF_NORMAL]->Release();
	if (_pShaderResourceViewA[GBUF_DEPTH_STENCIL])
		_pShaderResourceViewA[GBUF_DEPTH_STENCIL]->Release();
	if (_pShaderResourceViewA[GBUF_MATERIAL])
		_pShaderResourceViewA[GBUF_MATERIAL]->Release();
	if (_pShaderResourceViewA[GBUF_NORMAL])
		_pShaderResourceViewA[GBUF_NORMAL]->Release();

	// Texture
	if (_pTextureB[GBUF_DEPTH_STENCIL])
		_pTextureB[GBUF_DEPTH_STENCIL]->Release();
	if (_pTextureB[GBUF_MATERIAL])
		_pTextureB[GBUF_MATERIAL]->Release();
	if (_pTextureB[GBUF_NORMAL])
		_pTextureB[GBUF_NORMAL]->Release();
	if (_pTextureA[GBUF_DEPTH_STENCIL])
		_pTextureA[GBUF_DEPTH_STENCIL]->Release();
	if (_pTextureA[GBUF_MATERIAL])
		_pTextureA[GBUF_MATERIAL]->Release();
	if (_pTextureA[GBUF_NORMAL])
		_pTextureA[GBUF_NORMAL]->Release();
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
	descTexture.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
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

		// Create Texture
		hr = _pd3dDevice->CreateTexture2D(&descTexture, NULL, &_pTextureA[GBUF_NORMAL]);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to create Normal Texture A", "GBuffer", NULL);
			exit(1);
		}
		hr = _pd3dDevice->CreateTexture2D(&descTexture, NULL, &_pTextureB[GBUF_NORMAL]);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to create Normal Texture B", "GBuffer", NULL);
			exit(1);
		}

		// Create Shader Resource
		hr = _pd3dDevice->CreateShaderResourceView(_pTextureA[GBUF_NORMAL], &descShaderResourceView, &_pShaderResourceViewA[GBUF_NORMAL]);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to create Normal Resource View A", "GBuffer", NULL);
			exit(1);
		}
		hr = _pd3dDevice->CreateShaderResourceView(_pTextureB[GBUF_NORMAL], &descShaderResourceView, &_pShaderResourceViewB[GBUF_NORMAL]);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to create Normal Resource View B", "GBuffer", NULL);
			exit(1);
		}

		// Create Render
		hr = _pd3dDevice->CreateRenderTargetView(_pTextureA[GBUF_NORMAL], &descRenderTargetView, &_pRenderTargetViewA[GBUF_NORMAL]);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to create Normal Render View A", "GBuffer", NULL);
			exit(1);
		}
		hr = _pd3dDevice->CreateRenderTargetView(_pTextureB[GBUF_NORMAL], &descRenderTargetView, &_pRenderTargetViewB[GBUF_NORMAL]);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to create Normal Render View B", "GBuffer", NULL);
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
		hr = _pd3dDevice->CreateTexture2D(&descTexture, NULL, &_pTextureA[GBUF_MATERIAL]);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to create Material Texture A", "GBuffer", NULL);
			exit(1);
		}
		hr = _pd3dDevice->CreateTexture2D(&descTexture, NULL, &_pTextureB[GBUF_MATERIAL]);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to create Material Texture B", "GBuffer", NULL);
			exit(1);
		}

		// Create Shader Resource
		hr = _pd3dDevice->CreateShaderResourceView(_pTextureA[GBUF_MATERIAL], &descShaderResourceView, &_pShaderResourceViewA[GBUF_MATERIAL]);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to create Material Resource View A", "GBuffer", NULL);
			exit(1);
		}
		hr = _pd3dDevice->CreateShaderResourceView(_pTextureB[GBUF_MATERIAL], &descShaderResourceView, &_pShaderResourceViewB[GBUF_MATERIAL]);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to create Material Resource View B", "GBuffer", NULL);
			exit(1);
		}

		// Create Render
		hr = _pd3dDevice->CreateRenderTargetView(_pTextureA[GBUF_MATERIAL], &descRenderTargetView, &_pRenderTargetViewA[GBUF_MATERIAL]);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to create Material Render View A", "GBuffer", NULL);
			exit(1);
		}
		hr = _pd3dDevice->CreateRenderTargetView(_pTextureB[GBUF_MATERIAL], &descRenderTargetView, &_pRenderTargetViewB[GBUF_MATERIAL]);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to create Material Render View B", "GBuffer", NULL);
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
		descTexture.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
		hr = _pd3dDevice->CreateTexture2D(&descTexture, NULL, &_pTextureA[GBUF_DEPTH_STENCIL]);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to create Depth Texture A", "GBuffer", NULL);
			exit(1);
		}
		hr = _pd3dDevice->CreateTexture2D(&descTexture, NULL, &_pTextureB[GBUF_DEPTH_STENCIL]);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to create Depth Texture B", "GBuffer", NULL);
			exit(1);
		}

		// Create Shader Resource
		hr = _pd3dDevice->CreateShaderResourceView(_pTextureA[GBUF_DEPTH_STENCIL], &descShaderResourceView, &_pShaderResourceViewA[GBUF_DEPTH_STENCIL]);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to create Depth Resource View A", "GBuffer", NULL);
			exit(1);
		}
		hr = _pd3dDevice->CreateShaderResourceView(_pTextureB[GBUF_DEPTH_STENCIL], &descShaderResourceView, &_pShaderResourceViewB[GBUF_DEPTH_STENCIL]);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to create Depth Resource View B", "GBuffer", NULL);
			exit(1);
		}

		// Create Render
		hr = _pd3dDevice->CreateDepthStencilView(_pTextureA[GBUF_DEPTH_STENCIL], &descDepthView, &_pDepthStencilViewA);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to create Depth Render View A", "GBuffer", NULL);
			exit(1);
		}
		hr = _pd3dDevice->CreateDepthStencilView(_pTextureB[GBUF_DEPTH_STENCIL], &descDepthView, &_pDepthStencilViewB);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to create Depth Render View B", "GBuffer", NULL);
			exit(1);
		}
	}

	// State
	D3D11_DEPTH_STENCIL_DESC descDepth;
	descDepth.DepthEnable = TRUE;
	descDepth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	descDepth.DepthFunc = D3D11_COMPARISON_LESS;
	descDepth.StencilEnable = FALSE;
	hr = _pd3dDevice->CreateDepthStencilState(&descDepth, &_pDepthStencilState);
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
	descRasterizer.CullMode = D3D11_CULL_BACK;
	descRasterizer.FrontCounterClockwise = TRUE;
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

	_pDeferredContext->OMSetRenderTargets(GBUF_NUM_TEX - 1, _pRenderTargetViewA, _pDepthStencilViewA);
	_pDeferredContext->OMSetDepthStencilState(_pDepthStencilState, 0);
	_pDeferredContext->OMSetBlendState(_pBlendState, NULL, 0xFFFFFFFF);
	_pDeferredContext->RSSetState(_pRasterizerState);
	_pDeferredContext->RSSetViewports(1, &vp);

	_buffer = 0;
}

ID3D11ShaderResourceView *engine::GBuffer::getBindBufferResourceView(const UINT &num) const
{
	if (_buffer == 0)
		return _pShaderResourceViewA[num];
	return _pShaderResourceViewB[num];
}

ID3D11ShaderResourceView *engine::GBuffer::getUnbindBufferResourceView(const UINT &num) const
{
	if (_buffer == 1)
		return _pShaderResourceViewA[num];
	return _pShaderResourceViewB[num];
}

void engine::GBuffer::depthMask(const D3D11_DEPTH_WRITE_MASK &writeMask)
{
	D3D11_DEPTH_STENCIL_DESC descDepth;
	if (_pDepthStencilState == NULL)
	{
		MessageBox(NULL, "You need to configure the GBuffer before", "GBuffer", NULL);
		return;
	}

	_pDepthStencilState->GetDesc(&descDepth);
	_pDepthStencilState->Release();
	descDepth.DepthWriteMask = writeMask;
	_pd3dDevice->CreateDepthStencilState(&descDepth, &_pDepthStencilState);
	_pDeferredContext->OMSetDepthStencilState(_pDepthStencilState, 0);
}

void engine::GBuffer::depthFunc(const D3D11_COMPARISON_FUNC &func)
{
	D3D11_DEPTH_STENCIL_DESC descDepth;
	if (_pDepthStencilState == NULL)
	{
		MessageBox(NULL, "You need to configure the GBuffer before", "GBuffer", NULL);
		return;
	}

	_pDepthStencilState->GetDesc(&descDepth);
	_pDepthStencilState->Release();
	descDepth.DepthFunc = func;
	_pd3dDevice->CreateDepthStencilState(&descDepth, &_pDepthStencilState);
	_pDeferredContext->OMSetDepthStencilState(_pDepthStencilState, 0);
}

void engine::GBuffer::clear(void) const
{
	if (_buffer == 0)
	{
		_pDeferredContext->ClearRenderTargetView(_pRenderTargetViewA[GBUF_NORMAL], Colors::Transparent);
		_pDeferredContext->ClearRenderTargetView(_pRenderTargetViewA[GBUF_MATERIAL], Colors::Transparent);
		_pDeferredContext->ClearDepthStencilView(_pDepthStencilViewA, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}
	else
	{
		_pDeferredContext->ClearRenderTargetView(_pRenderTargetViewB[GBUF_NORMAL], Colors::Transparent);
		_pDeferredContext->ClearRenderTargetView(_pRenderTargetViewB[GBUF_MATERIAL], Colors::Transparent);
		_pDeferredContext->ClearDepthStencilView(_pDepthStencilViewB, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}
}

void engine::GBuffer::swapBuffer(void)
{
	if (_buffer == 0)
	{
		_buffer = 1;
		_pDeferredContext->OMSetRenderTargets(GBUF_NUM_TEX - 1, _pRenderTargetViewB, _pDepthStencilViewB);
	}
	else
	{
		_buffer = 0;
		_pDeferredContext->OMSetRenderTargets(GBUF_NUM_TEX - 1, _pRenderTargetViewA, _pDepthStencilViewA);
	}
}