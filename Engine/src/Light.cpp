#include <Engine/Light.hpp>
#include <Engine/ShadowMap.hpp>

engine::Light::Light(void)
	: _pInputLayout(NULL), _pVertexBuffer(NULL), _pShadowMatrixBuffer(NULL), _pIVPMatrixBuffer(NULL), _pScreenBuffer(NULL), _pCameraBuffer(NULL), _pLightInfoBuffer(NULL)
{
	_VPMatrix = (XMMATRIX *)_aligned_malloc(sizeof *_VPMatrix, 16);
	_shadow = new ShadowMap;
}

engine::Light::~Light(void)
{
	if (_pInputLayout) _pInputLayout->Release();
	if (_pVertexBuffer) _pVertexBuffer->Release();
	if (_pShadowMatrixBuffer) _pShadowMatrixBuffer->Release();
	if (_pIVPMatrixBuffer) _pIVPMatrixBuffer->Release();
	if (_pScreenBuffer) _pScreenBuffer->Release();
	if (_pCameraBuffer) _pCameraBuffer->Release();
	if (_pLightInfoBuffer) _pLightInfoBuffer->Release();
	_aligned_free(_VPMatrix);
	delete _shadow;
}

void engine::Light::configShadowMap(const UINT &width, const UINT &height, ID3D11Device *pd3dDevice, ID3D11DeviceContext *pContext)
{
	_shadow->config(width, height, pd3dDevice, pContext);
}

XMMATRIX engine::Light::getVPMatrix(void) const
{
	return *_VPMatrix;
}

engine::ShadowMap *engine::Light::getShadowMap(void)
{
	return _shadow;
}

void engine::Light::clear(void) const
{
	_shadow->clear();
}