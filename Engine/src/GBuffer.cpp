#include <Engine/GBuffer.hpp>

extern ID3D11Device1 *Device;
extern ID3D11DeviceContext1 *DeviceContext;

Engine::GBuffer::GBuffer(void)
{
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
	// Depth Stencil State;
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

Engine::GBuffer::~GBuffer(void)
{
	// Shader Resource View
	if (_pShaderResourceView[GBUF_NORMAL]) _pShaderResourceView[GBUF_NORMAL]->Release();
	if (_pShaderResourceView[GBUF_MATERIAL]) _pShaderResourceView[GBUF_MATERIAL]->Release();
	if (_pShaderResourceView[GBUF_LIGHT]) _pShaderResourceView[GBUF_LIGHT]->Release();
	if (_pShaderResourceView[GBUF_BACKGROUND]) _pShaderResourceView[GBUF_BACKGROUND]->Release();
	if (_pShaderResourceView[GBUF_DEPTH]) _pShaderResourceView[GBUF_DEPTH]->Release();
	if (_pShaderResourceView[GBUF_STENCIL]) _pShaderResourceView[GBUF_STENCIL]->Release();
	// Render View
	if (_pRenderTargetView[GBUF_NORMAL]) _pRenderTargetView[GBUF_NORMAL]->Release();
	if (_pRenderTargetView[GBUF_MATERIAL]) _pRenderTargetView[GBUF_MATERIAL]->Release();
	if (_pRenderTargetView[GBUF_LIGHT]) _pRenderTargetView[GBUF_LIGHT]->Release();
	if (_pRenderTargetView[GBUF_BACKGROUND]) _pRenderTargetView[GBUF_BACKGROUND]->Release();
	// Depth View
	if (_pDepthStencilView) _pDepthStencilView->Release();
	// Depth Stencil State
	if (_pGeometryDepthStencilState) _pGeometryDepthStencilState->Release();
	if (_pLightDepthStencilState) _pLightDepthStencilState->Release();
	if (_pBackgroundDepthStencilState) _pBackgroundDepthStencilState->Release();
	// Blend State
	if (_pGeometryBlendState) _pGeometryBlendState->Release();
	if (_pLightBlendState) _pLightBlendState->Release();
	if (_pBackgroundBlendState) _pBackgroundBlendState->Release();
	// Rasterizer State
	if (_pRasterizerState) _pRasterizerState->Release();

}

void Engine::GBuffer::config(const UINT &width, const UINT &height)
{
	FrameBuffer::config(width, height);

	ID3D11Texture2D *texture;

	// Shader Resource View
	if (_pShaderResourceView[GBUF_NORMAL]) _pShaderResourceView[GBUF_NORMAL]->Release();
	if (_pShaderResourceView[GBUF_MATERIAL]) _pShaderResourceView[GBUF_MATERIAL]->Release();
	if (_pShaderResourceView[GBUF_LIGHT]) _pShaderResourceView[GBUF_LIGHT]->Release();
	if (_pShaderResourceView[GBUF_BACKGROUND]) _pShaderResourceView[GBUF_BACKGROUND]->Release();
	if (_pShaderResourceView[GBUF_DEPTH]) _pShaderResourceView[GBUF_DEPTH]->Release();
	if (_pShaderResourceView[GBUF_STENCIL]) _pShaderResourceView[GBUF_STENCIL]->Release();
	// Render View
	if (_pRenderTargetView[GBUF_NORMAL]) _pRenderTargetView[GBUF_NORMAL]->Release();
	if (_pRenderTargetView[GBUF_MATERIAL]) _pRenderTargetView[GBUF_MATERIAL]->Release();
	if (_pRenderTargetView[GBUF_LIGHT]) _pRenderTargetView[GBUF_LIGHT]->Release();
	if (_pRenderTargetView[GBUF_BACKGROUND]) _pRenderTargetView[GBUF_BACKGROUND]->Release();
	// Depth View
	if (_pDepthStencilView) _pDepthStencilView->Release();
	// Depth Stencil State
	if (_pGeometryDepthStencilState) _pGeometryDepthStencilState->Release();
	if (_pLightDepthStencilState) _pLightDepthStencilState->Release();
	if (_pBackgroundDepthStencilState) _pBackgroundDepthStencilState->Release();
	// Blend State
	if (_pGeometryBlendState) _pGeometryBlendState->Release();
	if (_pLightBlendState) _pLightBlendState->Release();
	if (_pBackgroundBlendState) _pBackgroundBlendState->Release();
	// Rasterizer State
	if (_pRasterizerState) _pRasterizerState->Release();

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
	descTexture.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	Device->CreateTexture2D(&descTexture, NULL, &texture);
	descShaderResourceView.Format = descTexture.Format;
	Device->CreateShaderResourceView(texture, &descShaderResourceView, &_pShaderResourceView[GBUF_NORMAL]);
	descRenderTargetView.Format = descTexture.Format;
	Device->CreateRenderTargetView(texture, &descRenderTargetView, &_pRenderTargetView[GBUF_NORMAL]);
	texture->Release();

	// Material
	descTexture.Format = DXGI_FORMAT_R32G32B32A32_UINT;
	Device->CreateTexture2D(&descTexture, NULL, &texture);
	descShaderResourceView.Format = descTexture.Format;
	Device->CreateShaderResourceView(texture, &descShaderResourceView, &_pShaderResourceView[GBUF_MATERIAL]);
	descRenderTargetView.Format = descTexture.Format;
	Device->CreateRenderTargetView(texture, &descRenderTargetView, &_pRenderTargetView[GBUF_MATERIAL]);
	texture->Release();

	// Light
	descTexture.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	Device->CreateTexture2D(&descTexture, NULL, &texture);
	descShaderResourceView.Format = descTexture.Format;
	Device->CreateShaderResourceView(texture, &descShaderResourceView, &_pShaderResourceView[GBUF_LIGHT]);
	descRenderTargetView.Format = descTexture.Format;
	Device->CreateRenderTargetView(texture, &descRenderTargetView, &_pRenderTargetView[GBUF_LIGHT]);
	texture->Release();

	// Background
	descTexture.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	Device->CreateTexture2D(&descTexture, NULL, &texture);
	descShaderResourceView.Format = descTexture.Format;
	Device->CreateShaderResourceView(texture, &descShaderResourceView, &_pShaderResourceView[GBUF_BACKGROUND]);
	descRenderTargetView.Format = descTexture.Format;
	Device->CreateRenderTargetView(texture, &descRenderTargetView, &_pRenderTargetView[GBUF_BACKGROUND]);
	texture->Release();

	// Depth
	descTexture.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
	descTexture.Format = DXGI_FORMAT_R24G8_TYPELESS;
	Device->CreateTexture2D(&descTexture, NULL, &texture);
	descShaderResourceView.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	Device->CreateShaderResourceView(texture, &descShaderResourceView, &_pShaderResourceView[GBUF_DEPTH]);
	descShaderResourceView.Format = DXGI_FORMAT_X24_TYPELESS_G8_UINT;
	Device->CreateShaderResourceView(texture, &descShaderResourceView, &_pShaderResourceView[GBUF_STENCIL]);
	descDepthView.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	Device->CreateDepthStencilView(texture, &descDepthView, &_pDepthStencilView);
	texture->Release();

	// Depth Stencil State
	D3D11_DEPTH_STENCIL_DESC descDepth;
	// Skybox Depth Stencil
	descDepth.DepthEnable = TRUE;
	descDepth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	descDepth.DepthFunc = D3D11_COMPARISON_LESS;
	descDepth.StencilEnable = FALSE;
	Device->CreateDepthStencilState(&descDepth, &_pSkyboxDepthStencilState);
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
	Device->CreateDepthStencilState(&descDepth, &_pGeometryDepthStencilState);
	// Light Depth Stencil
	descDepth.DepthEnable = FALSE;
	descDepth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	descDepth.StencilEnable = FALSE;
	Device->CreateDepthStencilState(&descDepth, &_pLightDepthStencilState);
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
	Device->CreateDepthStencilState(&descDepth, &_pBackgroundDepthStencilState);

	// Blending State
	D3D11_BLEND_DESC descBlend;
	descBlend.AlphaToCoverageEnable = FALSE;
	descBlend.IndependentBlendEnable = FALSE;
	// Skybox Blending
	descBlend.RenderTarget[0].BlendEnable = FALSE;
	descBlend.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	Device->CreateBlendState(&descBlend, &_pSkyboxBlendState);
	// Geometry Blending
	descBlend.RenderTarget[0].BlendEnable = FALSE;
	descBlend.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	Device->CreateBlendState(&descBlend, &_pGeometryBlendState);
	// Light Blending
	descBlend.RenderTarget[0].BlendEnable = TRUE;
	descBlend.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	descBlend.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	descBlend.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	descBlend.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	descBlend.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	descBlend.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	descBlend.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	Device->CreateBlendState(&descBlend, &_pLightBlendState);
	// Background Blending
	descBlend.RenderTarget[0].BlendEnable = TRUE;
	descBlend.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	descBlend.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	descBlend.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	descBlend.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	descBlend.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
	descBlend.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	descBlend.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	Device->CreateBlendState(&descBlend, &_pBackgroundBlendState);
	
	// Rasterizer
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

	// Create the Viewport
	_VP.TopLeftX = 0.0f;
	_VP.TopLeftY = 0.0f;
	_VP.Width = (FLOAT)_width;
	_VP.Height = (FLOAT)_height;
	_VP.MinDepth = 0.0f;
	_VP.MaxDepth = 1.0f;
}

ID3D11ShaderResourceView *Engine::GBuffer::getShaderResourceView(const UINT &num) const
{
	return _pShaderResourceView[num];
}

void Engine::GBuffer::setSkyboxState(void) const
{
	ID3D11RenderTargetView *render[]
	{
		_pRenderTargetView[GBUF_BACKGROUND],
	};
	DeviceContext->OMSetRenderTargets(ARRAYSIZE(render), render, _pDepthStencilView);
	DeviceContext->OMSetDepthStencilState(_pSkyboxDepthStencilState, 1);
	DeviceContext->OMSetBlendState(_pSkyboxBlendState, NULL, 0xFFFFFFFF);
	DeviceContext->RSSetState(_pRasterizerState);
	DeviceContext->RSSetViewports(1, &_VP);
}

void Engine::GBuffer::setGeometryState(void) const
{
	ID3D11RenderTargetView *render[]
	{
		_pRenderTargetView[GBUF_NORMAL],
		_pRenderTargetView[GBUF_MATERIAL],
	};
	DeviceContext->OMSetRenderTargets(ARRAYSIZE(render), render, _pDepthStencilView);
	DeviceContext->OMSetDepthStencilState(_pGeometryDepthStencilState, 1);
	DeviceContext->OMSetBlendState(_pGeometryBlendState, NULL, 0xFFFFFFFF);
	DeviceContext->RSSetState(_pRasterizerState);
	DeviceContext->RSSetViewports(1, &_VP);
}

void Engine::GBuffer::setLightState(void) const
{
	ID3D11RenderTargetView *render[]
	{
		_pRenderTargetView[GBUF_LIGHT],
	};
	DeviceContext->OMSetRenderTargets(ARRAYSIZE(render), render, NULL);
	DeviceContext->OMSetDepthStencilState(_pLightDepthStencilState, 1);
	DeviceContext->OMSetBlendState(_pLightBlendState, NULL, 0xFFFFFFFF);
	DeviceContext->RSSetState(_pRasterizerState);
	DeviceContext->RSSetViewports(1, &_VP);
}

void Engine::GBuffer::setBackgroundState(void) const
{
	ID3D11RenderTargetView *render[]
	{
		_pRenderTargetView[GBUF_BACKGROUND],
	};
	DeviceContext->OMSetRenderTargets(ARRAYSIZE(render), render, _pDepthStencilView);
	DeviceContext->OMSetDepthStencilState(_pBackgroundDepthStencilState, 1);
	DeviceContext->OMSetBlendState(_pBackgroundBlendState, NULL, 0xFFFFFFFF);
	DeviceContext->RSSetState(_pRasterizerState);
	DeviceContext->RSSetViewports(1, &_VP);
}

void Engine::GBuffer::clear(void) const
{
	DeviceContext->ClearRenderTargetView(_pRenderTargetView[GBUF_NORMAL], Colors::Transparent);
	DeviceContext->ClearRenderTargetView(_pRenderTargetView[GBUF_MATERIAL], Colors::Transparent);
	DeviceContext->ClearRenderTargetView(_pRenderTargetView[GBUF_LIGHT], Colors::Transparent);
	DeviceContext->ClearRenderTargetView(_pRenderTargetView[GBUF_BACKGROUND], Colors::Transparent);
	DeviceContext->ClearDepthStencilView(_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Engine::GBuffer::clearLight(void) const
{
	DeviceContext->ClearRenderTargetView(_pRenderTargetView[GBUF_LIGHT], Colors::Transparent);
}