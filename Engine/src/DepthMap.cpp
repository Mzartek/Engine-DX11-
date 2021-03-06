#include <Engine/DepthMap.hpp>

Engine::DepthMap::DepthMap(void)
{
	// Shader Resource View
	_pShaderResourceView = NULL;
	// View
	_pDepthView = NULL;
	// State
	_pDepthState = NULL;
	_pRasterizerState = NULL;
	_pSamplerComparisonState = NULL;
}

Engine::DepthMap::~DepthMap(void)
{
	// Shader Resource View
	if (_pShaderResourceView) _pShaderResourceView->Release();
	// View
	if (_pDepthView) _pDepthView->Release();
	// State
	if (_pDepthState) _pDepthState->Release();
	if (_pRasterizerState) _pRasterizerState->Release();
	if (_pSamplerComparisonState) _pSamplerComparisonState->Release();
}

void Engine::DepthMap::config(const UINT &width, const UINT &height)
{
	FrameBuffer::config(width, height);

	ID3D11Texture2D *texture;

	// Shader Resource View
	if (_pShaderResourceView) _pShaderResourceView->Release();
	// View
	if (_pDepthView) _pDepthView->Release();
	// State
	if (_pDepthState) _pDepthState->Release();
	if (_pRasterizerState) _pRasterizerState->Release();
	if (_pSamplerComparisonState) _pSamplerComparisonState->Release();

	D3D11_TEXTURE2D_DESC descTexture;
	descTexture.Width = _width;
	descTexture.Height = _height;
	descTexture.MipLevels = 1;
	descTexture.ArraySize = 1;
	descTexture.SampleDesc.Count = 1;
	descTexture.SampleDesc.Quality = 0;
	descTexture.Usage = D3D11_USAGE_DEFAULT;
	descTexture.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
	descTexture.CPUAccessFlags = 0;
	descTexture.MiscFlags = 0;

	D3D11_SHADER_RESOURCE_VIEW_DESC descShaderResourceView;
	descShaderResourceView.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	descShaderResourceView.Texture2D.MostDetailedMip = 0;
	descShaderResourceView.Texture2D.MipLevels = 1;

	D3D11_DEPTH_STENCIL_VIEW_DESC descDepthView;
	descDepthView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDepthView.Flags = 0;
	descDepthView.Texture2D.MipSlice = 0;
	
	descTexture.Format = DXGI_FORMAT_R24G8_TYPELESS;
	Device->CreateTexture2D(&descTexture, NULL, &texture);
	descShaderResourceView.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	Device->CreateShaderResourceView(texture, &descShaderResourceView, &_pShaderResourceView);
	descDepthView.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	Device->CreateDepthStencilView(texture, &descDepthView, &_pDepthView);
	texture->Release();
	
	// State
	D3D11_DEPTH_STENCIL_DESC descDepth;
	descDepth.DepthEnable = TRUE;
	descDepth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	descDepth.DepthFunc = D3D11_COMPARISON_LESS;
	descDepth.StencilEnable = FALSE;
	Device->CreateDepthStencilState(&descDepth, &_pDepthState);

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
	Device->CreateRasterizerState(&descRasterizer, &_pRasterizerState);

	D3D11_SAMPLER_DESC descSampler;
	descSampler.Filter = D3D11_FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR;
	descSampler.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	descSampler.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	descSampler.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	descSampler.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
	descSampler.MipLODBias = 0.0f;
	descSampler.MaxAnisotropy = 1;
	descSampler.BorderColor[0] = 1.0f;
	descSampler.BorderColor[1] = 1.0f;
	descSampler.BorderColor[2] = 1.0f;
	descSampler.BorderColor[3] = 1.0f;
	descSampler.MinLOD = 0;
	descSampler.MaxLOD = D3D11_FLOAT32_MAX;
	Device->CreateSamplerState(&descSampler, &_pSamplerComparisonState);

	// Create the Viewport
	_VP.TopLeftX = 0.0f;
	_VP.TopLeftY = 0.0f;
	_VP.Width = (FLOAT)_width;
	_VP.Height = (FLOAT)_height;
	_VP.MinDepth = 0.0f;
	_VP.MaxDepth = 1.0f;
}

ID3D11ShaderResourceView *Engine::DepthMap::getShaderResourceView(void) const
{
	return _pShaderResourceView;
}

ID3D11SamplerState *Engine::DepthMap::getSamplerComparisonState(void) const
{
	return _pSamplerComparisonState;
}

void Engine::DepthMap::setState(void) const
{
	DeviceContext->OMSetRenderTargets(0, NULL, _pDepthView);
	DeviceContext->OMSetDepthStencilState(_pDepthState, 0);
	DeviceContext->OMSetBlendState(NULL, NULL, 0xFFFFFFFF);
	DeviceContext->RSSetState(_pRasterizerState);
	DeviceContext->RSSetViewports(1, &_VP);
}

void Engine::DepthMap::clear(void) const
{
	DeviceContext->ClearDepthStencilView(_pDepthView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}