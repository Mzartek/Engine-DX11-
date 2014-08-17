#include <Engine/GBuffer.hpp>

engine::GBuffer::GBuffer(void)
{
	// Context
	_pContext = NULL;
	// Texture
	_pTexture[GBUF_NORMAL] = NULL;
	_pTexture[GBUF_MATERIAL] = NULL;
	_pTexture[GBUF_DEPTH] = NULL;
	// Shader Resouce View
	_pShaderResourceView[GBUF_NORMAL] = NULL;
	_pShaderResourceView[GBUF_MATERIAL] = NULL;
	_pShaderResourceView[GBUF_DEPTH] = NULL;
	// View
	_pRenderTargetView[GBUF_NORMAL] = NULL;
	_pRenderTargetView[GBUF_MATERIAL] = NULL;
	_pDepthView = NULL;
	// State
	_pDepthState = NULL;
	_pBlendState = NULL;
	_pRasterizerState = NULL;
}

engine::GBuffer::~GBuffer(void)
{
	if (_pContext)
		_pContext->ClearState();

	// State
	if (_pRasterizerState)
		_pRasterizerState->Release();
	if (_pBlendState)
		_pBlendState->Release();
	if (_pDepthState)
		_pDepthState->Release();

	// View
	if (_pDepthView)
		_pDepthView->Release();
	if (_pRenderTargetView[GBUF_MATERIAL])
		_pRenderTargetView[GBUF_MATERIAL]->Release();
	if (_pRenderTargetView[GBUF_NORMAL])
		_pRenderTargetView[GBUF_NORMAL]->Release();

	// Shader Resource View
	if (_pShaderResourceView[GBUF_DEPTH])
		_pShaderResourceView[GBUF_DEPTH]->Release();
	if (_pShaderResourceView[GBUF_MATERIAL])
		_pShaderResourceView[GBUF_MATERIAL]->Release();
	if (_pShaderResourceView[GBUF_NORMAL])
		_pShaderResourceView[GBUF_NORMAL]->Release();

	// Texture
	if (_pTexture[GBUF_DEPTH])
		_pTexture[GBUF_DEPTH]->Release();
	if (_pTexture[GBUF_MATERIAL])
		_pTexture[GBUF_MATERIAL]->Release();
	if (_pTexture[GBUF_NORMAL])
		_pTexture[GBUF_NORMAL]->Release();

	// Context
	if (_pContext)
		_pContext->Release();
}

HRESULT engine::GBuffer::config(const UINT &width, const UINT &height, ID3D11Device *pd3dDevice)
{
	HRESULT hr;
	_width = width;
	_height = height;
	_pd3dDevice = pd3dDevice;

	D3D11_TEXTURE2D_DESC descTexture;
	D3D11_RENDER_TARGET_VIEW_DESC descRenderTargetView;
	D3D11_DEPTH_STENCIL_VIEW_DESC descDepthView;
	D3D11_SHADER_RESOURCE_VIEW_DESC descShaderResourceView;

	hr = _pd3dDevice->CreateDeferredContext(0, &_pContext);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create Deferred Context", "GBuffer", NULL);
		return hr;
	}

	// Normal
	descTexture.Width = width;
	descTexture.Height = height;
	descTexture.MipLevels = 1;
	descTexture.ArraySize = 1;
	descTexture.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	descTexture.SampleDesc.Count = 1;
	descTexture.SampleDesc.Quality = 0;
	descTexture.Usage = D3D11_USAGE_DEFAULT;
	descTexture.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	descTexture.CPUAccessFlags = 0;
	descTexture.MiscFlags = 0;
	hr = _pd3dDevice->CreateTexture2D(&descTexture, NULL, &_pTexture[GBUF_NORMAL]);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create Normal Texture", "GBuffer", NULL);
		return hr;
	}
	descRenderTargetView.Format = descTexture.Format;
	descRenderTargetView.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	descRenderTargetView.Texture2D.MipSlice = 0;
	hr = _pd3dDevice->CreateRenderTargetView(_pTexture[GBUF_NORMAL], &descRenderTargetView, &_pRenderTargetView[GBUF_NORMAL]);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create Normal View", "GBuffer", NULL);
		return hr;
	}
	descShaderResourceView.Format = descTexture.Format;
	descShaderResourceView.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	descShaderResourceView.Texture2D.MostDetailedMip = 0;
	descShaderResourceView.Texture2D.MipLevels = 1;
	hr = _pd3dDevice->CreateShaderResourceView(_pTexture[GBUF_NORMAL], &descShaderResourceView, &_pShaderResourceView[GBUF_NORMAL]);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create Normal Shader Resource View", "GBuffer", NULL);
		return hr;
	}

	// Material
	descTexture.Format = DXGI_FORMAT_R32G32B32A32_UINT;
	hr = _pd3dDevice->CreateTexture2D(&descTexture, NULL, &_pTexture[GBUF_MATERIAL]);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create Material Texture", "GBuffer", NULL);
		return hr;
	}
	descRenderTargetView.Format = descTexture.Format;
	hr = _pd3dDevice->CreateRenderTargetView(_pTexture[GBUF_MATERIAL], &descRenderTargetView, &_pRenderTargetView[GBUF_MATERIAL]);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create Material View", "GBuffer", NULL);
		return hr;
	}
	descShaderResourceView.Format = descTexture.Format;
	hr = _pd3dDevice->CreateShaderResourceView(_pTexture[GBUF_MATERIAL], &descShaderResourceView, &_pShaderResourceView[GBUF_MATERIAL]);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create Material Shader Resource View", "GBuffer", NULL);
		return hr;
	}

	// Depth
	descTexture.Format = DXGI_FORMAT_R32_TYPELESS;
	descTexture.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
	hr = _pd3dDevice->CreateTexture2D(&descTexture, NULL, &_pTexture[GBUF_DEPTH]);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create Depth Texture", "GBuffer", NULL);
		return hr;
	}
	descDepthView.Format = DXGI_FORMAT_D32_FLOAT;
	descDepthView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDepthView.Flags = 0;
	descDepthView.Texture2D.MipSlice = 0;
	hr = _pd3dDevice->CreateDepthStencilView(_pTexture[GBUF_DEPTH], &descDepthView, &_pDepthView);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create Depth View", "GBuffer", NULL);
		return hr;
	}
	descShaderResourceView.Format = DXGI_FORMAT_R32_FLOAT;
	hr = _pd3dDevice->CreateShaderResourceView(_pTexture[GBUF_DEPTH], &descShaderResourceView, &_pShaderResourceView[GBUF_DEPTH]);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create Depth Shader Resource View", "GBuffer", NULL);
		return hr;
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
		return hr;
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
		return hr;
	}

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
		MessageBox(NULL, "Failed to create Rasterizer State", "GBuffer", NULL);
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

	_pContext->OMSetRenderTargets(GBUF_NUM_TEX - 1, _pRenderTargetView, _pDepthView);
	_pContext->OMSetDepthStencilState(_pDepthState, 0);
	_pContext->OMSetBlendState(_pBlendState, NULL, 0xFFFFFFFF);
	_pContext->RSSetState(_pRasterizerState);
	_pContext->RSSetViewports(1, &vp);

	return S_OK;
}

ID3D11ShaderResourceView *engine::GBuffer::getShaderResourceView(const UINT &num) const
{
	return _pShaderResourceView[num];
}

ID3D11DeviceContext *engine::GBuffer::getContext(void) const
{
	return _pContext;
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
	_pContext->OMSetDepthStencilState(_pDepthState, 0);
}

void engine::GBuffer::clear(void) const
{
	_pContext->ClearRenderTargetView(_pRenderTargetView[GBUF_NORMAL], DirectX::Colors::Transparent);
	_pContext->ClearRenderTargetView(_pRenderTargetView[GBUF_MATERIAL], DirectX::Colors::Transparent);
	_pContext->ClearDepthStencilView(_pDepthView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}