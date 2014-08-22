#include <Engine/ShadowMap.hpp>

engine::ShadowMap::ShadowMap()
{
	_pTexture = NULL;
	_pShaderResourceView = NULL;
	_pDepthView = NULL;
	_pDepthState = NULL;
	_pRasterizerState = NULL;
	_program = NULL;
}

engine::ShadowMap::~ShadowMap()
{
	// State
	if (_pRasterizerState)
		_pRasterizerState->Release();
	if (_pDepthState)
		_pDepthState->Release();

	// View
	if (_pDepthView)
		_pDepthView->Release();

	// Shader Resource View
	if (_pShaderResourceView)
		_pShaderResourceView->Release();

	// Texture
	if (_pTexture)
		_pTexture->Release();
}

HRESULT engine::ShadowMap::config(const UINT &width, const UINT &height, ID3D11Device *pd3dDevice)
{
	HRESULT hr;
	D3D11_TEXTURE2D_DESC descTexture;
	D3D11_DEPTH_STENCIL_VIEW_DESC descDepthView;
	D3D11_SHADER_RESOURCE_VIEW_DESC descShaderResourceView;

	_width = width;
	_height = height;
	_pd3dDevice = pd3dDevice;

	hr = _pd3dDevice->CreateDeferredContext(0, &_pDefferedContext);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create Deferred Context", "ShadowMap", NULL);
		return hr;
	}

	// Depth
	descTexture.Width = width;
	descTexture.Height = height;
	descTexture.MipLevels = 1;
	descTexture.ArraySize = 1;
	descTexture.Format = DXGI_FORMAT_R32_TYPELESS;
	descTexture.SampleDesc.Count = 1;
	descTexture.SampleDesc.Quality = 0;
	descTexture.Usage = D3D11_USAGE_DEFAULT;
	descTexture.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
	hr = _pd3dDevice->CreateTexture2D(&descTexture, NULL, &_pTexture);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create Depth Texture", "ShadowMap", NULL);
		return hr;
	}
	descDepthView.Format = DXGI_FORMAT_D32_FLOAT;
	descDepthView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDepthView.Flags = 0;
	descDepthView.Texture2D.MipSlice = 0;
	hr = _pd3dDevice->CreateDepthStencilView(_pTexture, &descDepthView, &_pDepthView);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create Depth View", "ShadowMap", NULL);
		return hr;
	}
	descShaderResourceView.Format = DXGI_FORMAT_R32_FLOAT;
	descShaderResourceView.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	descShaderResourceView.Texture2D.MostDetailedMip = 0;
	descShaderResourceView.Texture2D.MipLevels = 1;
	hr = _pd3dDevice->CreateShaderResourceView(_pTexture, &descShaderResourceView, &_pShaderResourceView);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create Depth Shader Resource View", "ShadowMap", NULL);
		return hr;
	}

	// State
	D3D11_SAMPLER_DESC descSampler;
	descSampler.Filter = D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
	descSampler.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	descSampler.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	descSampler.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	descSampler.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
	descSampler.MipLODBias = 0.0f;
	descSampler.MaxAnisotropy = 1;
	descSampler.BorderColor[0] = 0.0f;
	descSampler.BorderColor[1] = 0.0f;
	descSampler.BorderColor[2] = 0.0f;
	descSampler.BorderColor[3] = 0.0f;
	descSampler.MinLOD = 0;
	descSampler.MaxLOD = D3D11_FLOAT32_MAX;
	hr = pd3dDevice->CreateSamplerState(&descSampler, &_pSamplerState);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Error while creating the SamplerState", "ShadowMap", MB_OK);
		return hr;
	}

	D3D11_DEPTH_STENCIL_DESC descDepth;
	descDepth.DepthEnable = TRUE;
	descDepth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	descDepth.DepthFunc = D3D11_COMPARISON_LESS;
	descDepth.StencilEnable = FALSE;
	hr = _pd3dDevice->CreateDepthStencilState(&descDepth, &_pDepthState);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create DepthStencil State", "ShadowMap", NULL);
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
		MessageBox(NULL, "Failed to create Rasterizer State", "ShadowMap", NULL);
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

	_pDefferedContext->OMSetRenderTargets(0, NULL, _pDepthView);
	_pDefferedContext->OMSetDepthStencilState(_pDepthState, 0);
	_pDefferedContext->RSSetState(_pRasterizerState);
	_pDefferedContext->RSSetViewports(1, &vp);

	return S_OK;
}