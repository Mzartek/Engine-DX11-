#include <Engine/LBuffer.hpp>

engine::LBuffer::LBuffer(void)
{
	// Texture
	_pTexture = NULL;
	// Shader Resouce View
	_pShaderResourceView = NULL;
	// Render View
	_pRenderTargetView = NULL;
	// State
	_pDepthStencilState = NULL;
	_pBlendState = NULL;
	_pRasterizerState = NULL;
}

engine::LBuffer::~LBuffer(void)
{
	// State
	if (_pRasterizerState)
		_pRasterizerState->Release();
	if (_pBlendState)
		_pBlendState->Release();
	if (_pDepthStencilState)
		_pDepthStencilState->Release();

	// Render View
	if (_pRenderTargetView)
		_pRenderTargetView->Release();

	// Shader Resource View
	if (_pShaderResourceView)
		_pShaderResourceView->Release();

	// Texture
	if (_pTexture)
		_pTexture->Release();
}

void engine::LBuffer::config(const UINT &width, const UINT &height, ID3D11Device *pd3dDevice, ID3D11DeviceContext *pContext)
{
	HRESULT hr;

	_width = width;
	_height = height;
	_pd3dDevice = pd3dDevice;
	_pContext = pContext;

	hr = _pd3dDevice->CreateDeferredContext(0, &_pDeferredContext);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create Deferred Context", "LBuffer", NULL);
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

	//Format
	descTexture.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	descShaderResourceView.Format = descTexture.Format;
	descRenderTargetView.Format = descTexture.Format;

	// Create Texture
	hr = _pd3dDevice->CreateTexture2D(&descTexture, NULL, &_pTexture);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create Normal Texture", "LBuffer", NULL);
		exit(1);
	}
		
	// Create Shader Resource
	hr = _pd3dDevice->CreateShaderResourceView(_pTexture, &descShaderResourceView, &_pShaderResourceView);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create Normal Resource View", "LBuffer", NULL);
		exit(1);
	}
			
	// Create Render
	hr = _pd3dDevice->CreateRenderTargetView(_pTexture, &descRenderTargetView, &_pRenderTargetView);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create Normal Render View", "LBuffer", NULL);
		exit(1);
	}
	
	// State
	D3D11_DEPTH_STENCIL_DESC descDepth;
	descDepth.DepthEnable = FALSE;
	descDepth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	descDepth.StencilEnable = FALSE;
	hr = _pd3dDevice->CreateDepthStencilState(&descDepth, &_pDepthStencilState);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create DepthStencil State", "LBuffer", NULL);
		exit(1);
	}

	D3D11_BLEND_DESC descBlend;
	descBlend.AlphaToCoverageEnable = FALSE;
	descBlend.IndependentBlendEnable = FALSE;
	descBlend.RenderTarget[0].BlendEnable = TRUE;
	descBlend.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	descBlend.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	descBlend.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	descBlend.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	descBlend.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	descBlend.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	descBlend.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	hr = _pd3dDevice->CreateBlendState(&descBlend, &_pBlendState);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create Blend State", "LBuffer", NULL);
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
		MessageBox(NULL, "Failed to create Rasterizer State", "LBuffer", NULL);
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

	_pDeferredContext->OMSetRenderTargets(1, &_pRenderTargetView, NULL);
	_pDeferredContext->OMSetDepthStencilState(_pDepthStencilState, 0);
	_pDeferredContext->OMSetBlendState(_pBlendState, NULL, 0xFFFFFFFF);
	_pDeferredContext->RSSetState(_pRasterizerState);
	_pDeferredContext->RSSetViewports(1, &vp);
}

ID3D11ShaderResourceView *engine::LBuffer::getShaderResourceView(void) const
{
	return _pShaderResourceView;
}

void engine::LBuffer::clear(void) const
{
	_pDeferredContext->ClearRenderTargetView(_pRenderTargetView, Colors::Black);
}