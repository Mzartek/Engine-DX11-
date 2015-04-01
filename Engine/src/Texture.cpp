#include <Engine/Texture.hpp>

Engine::Texture::Texture(void)
	: _pShaderResourceView(NULL), _pSamplerState(NULL)
{
}

Engine::Texture::~Texture(void)
{
	if (_pShaderResourceView) _pShaderResourceView->Release();
	if (_pSamplerState) _pSamplerState->Release();
}

ID3D11ShaderResourceView *Engine::Texture::getShaderResourceView(void)
{
	return _pShaderResourceView;
}

ID3D11SamplerState *Engine::Texture::getSamplerState(void)
{
	return _pSamplerState;
}