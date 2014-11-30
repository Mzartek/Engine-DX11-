#include <Engine/GBuffer.hpp>

Engine::GBuffer::GBuffer(const EngineDevice &EngineDevice)
	: FrameBuffer(EngineDevice)
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
	_pDepthNoWriteState = NULL;
	_pDepthStencilState = NULL;
	_pNoDepthStencilState = NULL;
	_pStencilState = NULL;
	// Blend State
	_pNoBlendState = NULL;
	_pAdditiveBlendState = NULL;
	_pAlphaBlendState = NULL;
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
	if (_pDepthNoWriteState) _pDepthNoWriteState->Release();
	if (_pDepthStencilState) _pDepthStencilState->Release();
	if (_pNoDepthStencilState) _pNoDepthStencilState->Release();
	if (_pStencilState) _pStencilState->Release();
	// Blend State
	if (_pNoBlendState) _pNoBlendState->Release();
	if (_pAdditiveBlendState) _pAdditiveBlendState->Release();
	if (_pAlphaBlendState) _pAlphaBlendState->Release();
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
	if (_pDepthNoWriteState) _pDepthNoWriteState->Release();
	if (_pDepthStencilState) _pDepthStencilState->Release();
	if (_pNoDepthStencilState) _pNoDepthStencilState->Release();
	if (_pStencilState) _pStencilState->Release();
	// Blend State
	if (_pNoBlendState) _pNoBlendState->Release();
	if (_pAdditiveBlendState) _pAdditiveBlendState->Release();
	if (_pAlphaBlendState) _pAlphaBlendState->Release();
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
	_EngineDevice.Device->CreateTexture2D(&descTexture, NULL, &texture);
	descShaderResourceView.Format = descTexture.Format;
	_EngineDevice.Device->CreateShaderResourceView(texture, &descShaderResourceView, &_pShaderResourceView[GBUF_NORMAL]);
	descRenderTargetView.Format = descTexture.Format;
	_EngineDevice.Device->CreateRenderTargetView(texture, &descRenderTargetView, &_pRenderTargetView[GBUF_NORMAL]);
	texture->Release();

	// Material
	descTexture.Format = DXGI_FORMAT_R32G32B32A32_UINT;
	_EngineDevice.Device->CreateTexture2D(&descTexture, NULL, &texture);
	descShaderResourceView.Format = descTexture.Format;
	_EngineDevice.Device->CreateShaderResourceView(texture, &descShaderResourceView, &_pShaderResourceView[GBUF_MATERIAL]);
	descRenderTargetView.Format = descTexture.Format;
	_EngineDevice.Device->CreateRenderTargetView(texture, &descRenderTargetView, &_pRenderTargetView[GBUF_MATERIAL]);
	texture->Release();

	// Light
	descTexture.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	_EngineDevice.Device->CreateTexture2D(&descTexture, NULL, &texture);
	descShaderResourceView.Format = descTexture.Format;
	_EngineDevice.Device->CreateShaderResourceView(texture, &descShaderResourceView, &_pShaderResourceView[GBUF_LIGHT]);
	descRenderTargetView.Format = descTexture.Format;
	_EngineDevice.Device->CreateRenderTargetView(texture, &descRenderTargetView, &_pRenderTargetView[GBUF_LIGHT]);
	texture->Release();

	// Background
	descTexture.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	_EngineDevice.Device->CreateTexture2D(&descTexture, NULL, &texture);
	descShaderResourceView.Format = descTexture.Format;
	_EngineDevice.Device->CreateShaderResourceView(texture, &descShaderResourceView, &_pShaderResourceView[GBUF_BACKGROUND]);
	descRenderTargetView.Format = descTexture.Format;
	_EngineDevice.Device->CreateRenderTargetView(texture, &descRenderTargetView, &_pRenderTargetView[GBUF_BACKGROUND]);
	texture->Release();

	// Depth
	descTexture.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
	descTexture.Format = DXGI_FORMAT_R24G8_TYPELESS;
	_EngineDevice.Device->CreateTexture2D(&descTexture, NULL, &texture);
	descShaderResourceView.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	_EngineDevice.Device->CreateShaderResourceView(texture, &descShaderResourceView, &_pShaderResourceView[GBUF_DEPTH]);
	descShaderResourceView.Format = DXGI_FORMAT_X24_TYPELESS_G8_UINT;
	_EngineDevice.Device->CreateShaderResourceView(texture, &descShaderResourceView, &_pShaderResourceView[GBUF_STENCIL]);
	descDepthView.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	_EngineDevice.Device->CreateDepthStencilView(texture, &descDepthView, &_pDepthStencilView);
	texture->Release();

	// Depth Stencil State
	D3D11_DEPTH_STENCIL_DESC descDepth;
	// DepthNoWrite
	descDepth.DepthEnable = TRUE;
	descDepth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	descDepth.DepthFunc = D3D11_COMPARISON_LESS;
	descDepth.StencilEnable = FALSE;
	_EngineDevice.Device->CreateDepthStencilState(&descDepth, &_pDepthNoWriteState);
	// DepthStencil
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
	_EngineDevice.Device->CreateDepthStencilState(&descDepth, &_pDepthStencilState);
	// NoDepthStencil
	descDepth.DepthEnable = FALSE;
	descDepth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	descDepth.StencilEnable = FALSE;
	_EngineDevice.Device->CreateDepthStencilState(&descDepth, &_pNoDepthStencilState);
	// Stencil
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
	_EngineDevice.Device->CreateDepthStencilState(&descDepth, &_pStencilState);

	// Blending State
	D3D11_BLEND_DESC descBlend;
	descBlend.AlphaToCoverageEnable = FALSE;
	descBlend.IndependentBlendEnable = FALSE;
	// No Blending
	descBlend.RenderTarget[0].BlendEnable = FALSE;
	descBlend.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	_EngineDevice.Device->CreateBlendState(&descBlend, &_pNoBlendState);
	// Additive Blending
	descBlend.RenderTarget[0].BlendEnable = TRUE;
	descBlend.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	descBlend.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	descBlend.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	descBlend.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	descBlend.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	descBlend.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	descBlend.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	_EngineDevice.Device->CreateBlendState(&descBlend, &_pAdditiveBlendState);
	// Alpha Blending
	descBlend.RenderTarget[0].BlendEnable = TRUE;
	descBlend.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	descBlend.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	descBlend.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	descBlend.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	descBlend.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
	descBlend.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	descBlend.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	_EngineDevice.Device->CreateBlendState(&descBlend, &_pAlphaBlendState);
	
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
	_EngineDevice.Device->CreateRasterizerState(&descRasterizer, &_pRasterizerState);

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
	_EngineDevice.DeviceContext->OMSetRenderTargets(ARRAYSIZE(render), render, _pDepthStencilView);
	_EngineDevice.DeviceContext->OMSetDepthStencilState(_pDepthNoWriteState, 1);
	_EngineDevice.DeviceContext->OMSetBlendState(_pNoBlendState, NULL, 0xFFFFFFFF);
	_EngineDevice.DeviceContext->RSSetState(_pRasterizerState);
	_EngineDevice.DeviceContext->RSSetViewports(1, &_VP);
}

void Engine::GBuffer::setGeometryState(void) const
{
	ID3D11RenderTargetView *render[]
	{
		_pRenderTargetView[GBUF_NORMAL],
		_pRenderTargetView[GBUF_MATERIAL],
	};
	_EngineDevice.DeviceContext->OMSetRenderTargets(ARRAYSIZE(render), render, _pDepthStencilView);
	_EngineDevice.DeviceContext->OMSetDepthStencilState(_pDepthStencilState, 1);
	_EngineDevice.DeviceContext->OMSetBlendState(_pNoBlendState, NULL, 0xFFFFFFFF);
	_EngineDevice.DeviceContext->RSSetState(_pRasterizerState);
	_EngineDevice.DeviceContext->RSSetViewports(1, &_VP);
}

void Engine::GBuffer::setLightState(void) const
{
	ID3D11RenderTargetView *render[]
	{
		_pRenderTargetView[GBUF_LIGHT],
	};
	_EngineDevice.DeviceContext->OMSetRenderTargets(ARRAYSIZE(render), render, NULL);
	_EngineDevice.DeviceContext->OMSetDepthStencilState(_pNoDepthStencilState, 1);
	_EngineDevice.DeviceContext->OMSetBlendState(_pAdditiveBlendState, NULL, 0xFFFFFFFF);
	_EngineDevice.DeviceContext->RSSetState(_pRasterizerState);
	_EngineDevice.DeviceContext->RSSetViewports(1, &_VP);
}

void Engine::GBuffer::setParticlesState(void) const
{
	ID3D11RenderTargetView *render[]
	{
		_pRenderTargetView[GBUF_BACKGROUND],
	};
	_EngineDevice.DeviceContext->OMSetRenderTargets(ARRAYSIZE(render), render, _pDepthStencilView);
	_EngineDevice.DeviceContext->OMSetDepthStencilState(_pNoDepthStencilState, 1);
	_EngineDevice.DeviceContext->OMSetBlendState(_pAlphaBlendState, NULL, 0xFFFFFFFF);
	_EngineDevice.DeviceContext->RSSetState(_pRasterizerState);
	_EngineDevice.DeviceContext->RSSetViewports(1, &_VP);
}

void Engine::GBuffer::setBackgroundState(void) const
{
	ID3D11RenderTargetView *render[]
	{
		_pRenderTargetView[GBUF_BACKGROUND],
	};
	_EngineDevice.DeviceContext->OMSetRenderTargets(ARRAYSIZE(render), render, _pDepthStencilView);
	_EngineDevice.DeviceContext->OMSetDepthStencilState(_pStencilState, 1);
	_EngineDevice.DeviceContext->OMSetBlendState(_pAlphaBlendState, NULL, 0xFFFFFFFF);
	_EngineDevice.DeviceContext->RSSetState(_pRasterizerState);
	_EngineDevice.DeviceContext->RSSetViewports(1, &_VP);
}

void Engine::GBuffer::clear(void) const
{
	_EngineDevice.DeviceContext->ClearRenderTargetView(_pRenderTargetView[GBUF_NORMAL], Colors::Transparent);
	_EngineDevice.DeviceContext->ClearRenderTargetView(_pRenderTargetView[GBUF_MATERIAL], Colors::Transparent);
	_EngineDevice.DeviceContext->ClearRenderTargetView(_pRenderTargetView[GBUF_LIGHT], Colors::Transparent);
	_EngineDevice.DeviceContext->ClearRenderTargetView(_pRenderTargetView[GBUF_BACKGROUND], Colors::Transparent);
	_EngineDevice.DeviceContext->ClearDepthStencilView(_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Engine::GBuffer::clearLight(void) const
{
	_EngineDevice.DeviceContext->ClearRenderTargetView(_pRenderTargetView[GBUF_LIGHT], Colors::Transparent);
}