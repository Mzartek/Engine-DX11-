#include <Engine/Light.hpp>
#include <Engine/Buffer.hpp>
#include <Engine/ShadowMap.hpp>

engine::Light::Light(void)
	: _pInputLayout(NULL)
{
	_vertexBuffer = new Buffer;
	_shadowMatrixBuffer = new Buffer;
	_IVPMatrixBuffer = new Buffer;
	_screenBuffer = new Buffer;
	_cameraBuffer = new Buffer;
	_lightInfoBuffer = new Buffer;
	_VPMatrix = (XMMATRIX *)_aligned_malloc(sizeof *_VPMatrix, 16);
	_shadow = new ShadowMap;
}

engine::Light::~Light(void)
{
	if (_pInputLayout) _pInputLayout->Release();
	delete _vertexBuffer;
	delete _shadowMatrixBuffer;
	delete _IVPMatrixBuffer;
	delete _screenBuffer;
	delete _cameraBuffer;
	delete _lightInfoBuffer;
	_aligned_free(_VPMatrix);
	delete _shadow;
}

void engine::Light::configShadowMap(const UINT &width, const UINT &height)
{
	_shadow->config(width, height);
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