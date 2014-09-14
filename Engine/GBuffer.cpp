#include <Engine/GBuffer.hpp>

engine::GBuffer::GBuffer(void)
{
	// Texture
	_pTexture[GBUF_NORMAL] = NULL;
	_pTexture[GBUF_MATERIAL] = NULL;
	_pTexture[GBUF_LIGHT] = NULL;
	_pTexture[GBUF_BACKGROUND] = NULL;
	_pTexture[GBUF_DEPTH] = NULL;
	// Shader Resouce View
	_pShaderResourceView[GBUF_NORMAL] = NULL;
	_pShaderResourceView[GBUF_MATERIAL] = NULL;
	_pShaderResourceView[GBUF_LIGHT] = NULL;
	_pShaderResourceView[GBUF_BACKGROUND] = NULL;
	_pShaderResourceView[GBUF_DEPTH] = NULL;
	_pShaderResourceView[GBUF_STENCIL] = NULL;
	// Render View
	_pRenderTargetView[GBUF_NORMAL] = NULL;
	_pRenderTargetView[GBUF_MATERIAL] = NULL;
	_pRenderTargetView[GBUF_LIGHT] = NULL;
	_pRenderTargetView[GBUF_BACKGROUND] = NULL;
	// Depth Stencil View
	_pDepthStencilView = NULL;
	// Depth Stencil State
	_pSkyboxDepthStencilState = NULL;
	_pGeometryDepthStencilState = NULL;
	_pLightDepthStencilState = NULL;
	_pBackgroundDepthStencilState = NULL;
	// Blend State
	_pSkyboxBlendState = NULL;
	_pGeometryBlendState = NULL;
	_pLightBlendState = NULL;
	_pBackgroundBlendState = NULL;
	// Rasterizer State
	_pRasterizerState = NULL;
}

engine::GBuffer::~GBuffer(void)
{
	// Rasterizer State
	if (_pRasterizerState)
		_pRasterizerState->Release();

	// Blend State
	if (_pBackgroundBlendState)
		_pBackgroundBlendState->Release();
	if (_pLightBlendState)
		_pLightBlendState->Release();
	if (_pGeometryBlendState)
		_pGeometryBlendState->Release();

	// Depth Stencil State
	if (_pBackgroundDepthStencilState)
		_pBackgroundDepthStencilState->Release();
	if (_pLightDepthStencilState)
		_pLightDepthStencilState->Release();
	if (_pGeometryDepthStencilState)
		_pGeometryDepthStencilState->Release();

	// Depth View
	if (_pDepthStencilView)
		_pDepthStencilView->Release();

	// Render View
	if (_pRenderTargetView[GBUF_BACKGROUND])
		_pRenderTargetView[GBUF_BACKGROUND]->Release();
	if (_pRenderTargetView[GBUF_LIGHT])
		_pRenderTargetView[GBUF_LIGHT]->Release();
	if (_pRenderTargetView[GBUF_MATERIAL])
		_pRenderTargetView[GBUF_MATERIAL]->Release();
	if (_pRenderTargetView[GBUF_NORMAL])
		_pRenderTargetView[GBUF_NORMAL]->Release();

	// Shader Resource View
	if (_pShaderResourceView[GBUF_STENCIL])
		_pShaderResourceView[GBUF_STENCIL]->Release();
	if (_pShaderResourceView[GBUF_DEPTH])
		_pShaderResourceView[GBUF_DEPTH]->Release();
	if (_pShaderResourceView[GBUF_BACKGROUND])
		_pShaderResourceView[GBUF_BACKGROUND]->Release();
	if (_pShaderResourceView[GBUF_LIGHT])
		_pShaderResourceView[GBUF_LIGHT]->Release();
	if (_pShaderResourceView[GBUF_MATERIAL])
		_pShaderResourceView[GBUF_MATERIAL]->Release();
	if (_pShaderResourceView[GBUF_NORMAL])
		_pShaderResourceView[GBUF_NORMAL]->Release();

	// Texture
	if (_pTexture[GBUF_DEPTH])
		_pTexture[GBUF_DEPTH]->Release();
	if (_pTexture[GBUF_BACKGROUND])
		_pTexture[GBUF_BACKGROUND]->Release();
	if (_pTexture[GBUF_LIGHT])
		_pTexture[GBUF_LIGHT]->Release();
	if (_pTexture[GBUF_MATERIAL])
		_pTexture[GBUF_MATERIAL]->Release();
	if (_pTexture[GBUF_NORMAL])
		_pTexture[GBUF_NORMAL]->Release();
}

void engine::GBuffer::config(const UINT &width, const UINT &height, ID3D11Device *pd3dDevice, ID3D11DeviceContext *pContext)
{
	HRESULT hr;

	_width = width;
	_height = height;
	_pd3dDevice = pd3dDevice;
	_pContext = pContext;

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
		// Create Texture
		descTexture.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		hr = _pd3dDevice->CreateTexture2D(&descTexture, NULL, &_pTexture[GBUF_NORMAL]);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to create Normal Texture", "GBuffer", NULL);
			exit(1);
		}

		// Create Shader Resource
		descShaderResourceView.Format = descTexture.Format;
		hr = _pd3dDevice->CreateShaderResourceView(_pTexture[GBUF_NORMAL], &descShaderResourceView, &_pShaderResourceView[GBUF_NORMAL]);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to create Normal Resource View", "GBuffer", NULL);
			exit(1);
		}

		// Create Render
		descRenderTargetView.Format = descTexture.Format;
		hr = _pd3dDevice->CreateRenderTargetView(_pTexture[GBUF_NORMAL], &descRenderTargetView, &_pRenderTargetView[GBUF_NORMAL]);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to create Normal Render View", "GBuffer", NULL);
			exit(1);
		}
	}

	// Material
	{
		// Create Resource
		descTexture.Format = DXGI_FORMAT_R32G32B32A32_UINT;
		hr = _pd3dDevice->CreateTexture2D(&descTexture, NULL, &_pTexture[GBUF_MATERIAL]);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to create Material Texture", "GBuffer", NULL);
			exit(1);
		}

		// Create Shader Resource
		descShaderResourceView.Format = descTexture.Format;
		hr = _pd3dDevice->CreateShaderResourceView(_pTexture[GBUF_MATERIAL], &descShaderResourceView, &_pShaderResourceView[GBUF_MATERIAL]);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to create Material Resource View", "GBuffer", NULL);
			exit(1);
		}

		// Create Render
		descRenderTargetView.Format = descTexture.Format;
		hr = _pd3dDevice->CreateRenderTargetView(_pTexture[GBUF_MATERIAL], &descRenderTargetView, &_pRenderTargetView[GBUF_MATERIAL]);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to create Material Render View", "GBuffer", NULL);
			exit(1);
		}
	}

	// Light
	{
		// Create Resource
		descTexture.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		hr = _pd3dDevice->CreateTexture2D(&descTexture, NULL, &_pTexture[GBUF_LIGHT]);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to create Light Texture", "GBuffer", NULL);
			exit(1);
		}

		// Create Shader Resource
		descShaderResourceView.Format = descTexture.Format;
		hr = _pd3dDevice->CreateShaderResourceView(_pTexture[GBUF_LIGHT], &descShaderResourceView, &_pShaderResourceView[GBUF_LIGHT]);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to create Light Resource View", "GBuffer", NULL);
			exit(1);
		}

		// Create Render
		descRenderTargetView.Format = descTexture.Format;
		hr = _pd3dDevice->CreateRenderTargetView(_pTexture[GBUF_LIGHT], &descRenderTargetView, &_pRenderTargetView[GBUF_LIGHT]);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to create Light Render View", "GBuffer", NULL);
			exit(1);
		}
	}

	// Background
	{
		// Create Resource
		descTexture.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		hr = _pd3dDevice->CreateTexture2D(&descTexture, NULL, &_pTexture[GBUF_BACKGROUND]);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to create Background Texture", "GBuffer", NULL);
			exit(1);
		}

		// Create Shader Resource
		descShaderResourceView.Format = descTexture.Format;
		hr = _pd3dDevice->CreateShaderResourceView(_pTexture[GBUF_BACKGROUND], &descShaderResourceView, &_pShaderResourceView[GBUF_BACKGROUND]);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to create Background Resource View", "GBuffer", NULL);
			exit(1);
		}

		// Create Render
		descRenderTargetView.Format = descTexture.Format;
		hr = _pd3dDevice->CreateRenderTargetView(_pTexture[GBUF_BACKGROUND], &descRenderTargetView, &_pRenderTargetView[GBUF_BACKGROUND]);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to create Background Render View", "GBuffer", NULL);
			exit(1);
		}
	}

	// Depth
	{
		// Create Resource
		descTexture.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
		descTexture.Format = DXGI_FORMAT_R24G8_TYPELESS;
		hr = _pd3dDevice->CreateTexture2D(&descTexture, NULL, &_pTexture[GBUF_DEPTH]);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to create Depth Stencil Texture", "GBuffer", NULL);
			exit(1);
		}

		// Create Shader Resource
		descShaderResourceView.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		hr = _pd3dDevice->CreateShaderResourceView(_pTexture[GBUF_DEPTH], &descShaderResourceView, &_pShaderResourceView[GBUF_DEPTH]);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to create Depth Resource View", "GBuffer", NULL);
			exit(1);
		}
		descShaderResourceView.Format = DXGI_FORMAT_X24_TYPELESS_G8_UINT;
		hr = _pd3dDevice->CreateShaderResourceView(_pTexture[GBUF_DEPTH], &descShaderResourceView, &_pShaderResourceView[GBUF_STENCIL]);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to create Stencil Resource View", "GBuffer", NULL);
			exit(1);
		}

		// Create Render
		descDepthView.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		hr = _pd3dDevice->CreateDepthStencilView(_pTexture[GBUF_DEPTH], &descDepthView, &_pDepthStencilView);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to create Depth Stencil Render View", "GBuffer", NULL);
			exit(1);
		}
	}

	// Depth Stencil State
	{
		D3D11_DEPTH_STENCIL_DESC descDepth;

		// Skybox Depth Stencil
		descDepth.DepthEnable = TRUE;
		descDepth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		descDepth.DepthFunc = D3D11_COMPARISON_LESS;
		descDepth.StencilEnable = FALSE;
		hr = _pd3dDevice->CreateDepthStencilState(&descDepth, &_pSkyboxDepthStencilState);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to create Skybox Depth Stencil State", "GBuffer", NULL);
			exit(1);
		}

		// Geometry Depth Stencil
		descDepth.DepthEnable = TRUE;
		descDepth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		descDepth.DepthFunc = D3D11_COMPARISON_LESS;
		descDepth.StencilEnable = TRUE;
		descDepth.StencilReadMask = 0XFF;
		descDepth.StencilWriteMask = 0XFF;
		descDepth.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		descDepth.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		descDepth.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		descDepth.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		descDepth.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		descDepth.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		descDepth.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		descDepth.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		hr = _pd3dDevice->CreateDepthStencilState(&descDepth, &_pGeometryDepthStencilState);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to create Geometry Depth Stencil State", "GBuffer", NULL);
			exit(1);
		}

		// Light Depth Stencil
		descDepth.DepthEnable = FALSE;
		descDepth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		descDepth.StencilEnable = FALSE;
		hr = _pd3dDevice->CreateDepthStencilState(&descDepth, &_pLightDepthStencilState);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to create Light Depth Stencil State", "GBuffer", NULL);
			exit(1);
		}

		// Background Depth Stencil
		descDepth.DepthEnable = FALSE;
		descDepth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		descDepth.StencilEnable = TRUE;
		descDepth.StencilReadMask = 0XFF;
		descDepth.StencilWriteMask = 0XFF;
		descDepth.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		descDepth.FrontFace.StencilPassOp = D3D11_STENCIL_OP_ZERO;
		descDepth.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
		descDepth.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		descDepth.BackFace.StencilPassOp = D3D11_STENCIL_OP_ZERO;
		descDepth.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;
		hr = _pd3dDevice->CreateDepthStencilState(&descDepth, &_pBackgroundDepthStencilState);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to create Light Depth Stencil State", "GBuffer", NULL);
			exit(1);
		}
	}

	// Blending State
	{
		D3D11_BLEND_DESC descBlend;
		descBlend.AlphaToCoverageEnable = FALSE;
		descBlend.IndependentBlendEnable = FALSE;

		// Skybox Blending
		descBlend.RenderTarget[0].BlendEnable = FALSE;
		descBlend.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		hr = _pd3dDevice->CreateBlendState(&descBlend, &_pSkyboxBlendState);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to create Skybox Blend State", "GBuffer", NULL);
			exit(1);
		}

		// Geometry Blending
		descBlend.RenderTarget[0].BlendEnable = FALSE;
		descBlend.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		hr = _pd3dDevice->CreateBlendState(&descBlend, &_pGeometryBlendState);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to create Geometry Blend State", "GBuffer", NULL);
			exit(1);
		}

		// Light Blending
		descBlend.RenderTarget[0].BlendEnable = TRUE;
		descBlend.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		descBlend.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		descBlend.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		descBlend.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		descBlend.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		descBlend.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		descBlend.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		hr = _pd3dDevice->CreateBlendState(&descBlend, &_pLightBlendState);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to create Light Blend State", "GBuffer", NULL);
			exit(1);
		}

		// Background Blending
		descBlend.RenderTarget[0].BlendEnable = TRUE;
		descBlend.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		descBlend.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		descBlend.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		descBlend.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
		descBlend.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		descBlend.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		descBlend.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		hr = _pd3dDevice->CreateBlendState(&descBlend, &_pBackgroundBlendState);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to create Background Blend State", "GBuffer", NULL);
			exit(1);
		}
	}

	D3D11_RASTERIZER_DESC descRasterizer;
	descRasterizer.FillMode = D3D11_FILL_SOLID;
	descRasterizer.CullMode = D3D11_CULL_NONE;
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
	_pVP->TopLeftX = 0.0f;
	_pVP->TopLeftY = 0.0f;
	_pVP->Width = (FLOAT)_width;
	_pVP->Height = (FLOAT)_height;
	_pVP->MinDepth = 0.0f;
	_pVP->MaxDepth = 1.0f;
}

ID3D11ShaderResourceView *engine::GBuffer::getShaderResourceView(const UINT &num) const
{
	return _pShaderResourceView[num];
}

void engine::GBuffer::setSkyboxConfig(void) const
{
	ID3D11RenderTargetView *render[]
	{
		_pRenderTargetView[GBUF_BACKGROUND],
	};
	_pContext->OMSetRenderTargets(ARRAYSIZE(render), render, _pDepthStencilView);
	_pContext->OMSetDepthStencilState(_pSkyboxDepthStencilState, 1);
	_pContext->OMSetBlendState(_pSkyboxBlendState, NULL, 0xFFFFFFFF);
	_pContext->RSSetState(_pRasterizerState);
	_pContext->RSSetViewports(1, _pVP);
}

void engine::GBuffer::setGeometryConfig(void) const
{
	ID3D11RenderTargetView *render[]
	{
		_pRenderTargetView[GBUF_NORMAL],
		_pRenderTargetView[GBUF_MATERIAL],
	};
	_pContext->OMSetRenderTargets(ARRAYSIZE(render), render, _pDepthStencilView);
	_pContext->OMSetDepthStencilState(_pGeometryDepthStencilState, 1);
	_pContext->OMSetBlendState(_pGeometryBlendState, NULL, 0xFFFFFFFF);
	_pContext->RSSetState(_pRasterizerState);
	_pContext->RSSetViewports(1, _pVP);
}

void engine::GBuffer::setLightConfig(void) const
{
	ID3D11RenderTargetView *render[]
	{
		_pRenderTargetView[GBUF_LIGHT],
	};
	_pContext->OMSetRenderTargets(ARRAYSIZE(render), render, NULL);
	_pContext->OMSetDepthStencilState(_pLightDepthStencilState, 1);
	_pContext->OMSetBlendState(_pLightBlendState, NULL, 0xFFFFFFFF);
	_pContext->RSSetState(_pRasterizerState);
	_pContext->RSSetViewports(1, _pVP);
}

void engine::GBuffer::setBackgroundConfig(void) const
{
	ID3D11RenderTargetView *render[]
	{
		_pRenderTargetView[GBUF_BACKGROUND],
	};
	_pContext->OMSetRenderTargets(ARRAYSIZE(render), render, _pDepthStencilView);
	_pContext->OMSetDepthStencilState(_pBackgroundDepthStencilState, 1);
	_pContext->OMSetBlendState(_pBackgroundBlendState, NULL, 0xFFFFFFFF);
	_pContext->RSSetState(_pRasterizerState);
	_pContext->RSSetViewports(1, _pVP);
}

void engine::GBuffer::clear(void) const
{
	_pContext->ClearRenderTargetView(_pRenderTargetView[GBUF_NORMAL], Colors::Transparent);
	_pContext->ClearRenderTargetView(_pRenderTargetView[GBUF_MATERIAL], Colors::Transparent);
	_pContext->ClearRenderTargetView(_pRenderTargetView[GBUF_LIGHT], Colors::Transparent);
	_pContext->ClearRenderTargetView(_pRenderTargetView[GBUF_BACKGROUND], Colors::Transparent);
	_pContext->ClearDepthStencilView(_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void engine::GBuffer::clearLight(void) const
{
	_pContext->ClearRenderTargetView(_pRenderTargetView[GBUF_LIGHT], Colors::Transparent);
}