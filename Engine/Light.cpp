#include <Engine/Light.hpp>

engine::Light::Light(void)
{
	_pShadowMatrixBuffer = NULL;
	_pIVPMatrixBuffer = NULL;
	_pScreenBuffer = NULL;
	_pCameraBuffer = NULL;
	_pLightInfoBuffer = NULL;
	_pInputLayout = NULL;
	_pVertexBuffer = NULL;
	_VPMatrix = (XMMATRIX *)_aligned_malloc(sizeof *_VPMatrix, 16);
	_shadow = NULL;
}

engine::Light::~Light(void)
{
	if (_pVertexBuffer)
		_pVertexBuffer->Release();
	if (_pInputLayout)
		_pInputLayout->Release();
	if (_pLightInfoBuffer)
		_pLightInfoBuffer->Release();
	if (_pCameraBuffer)
		_pCameraBuffer->Release();
	if (_pScreenBuffer)
		_pScreenBuffer->Release();
	if (_pIVPMatrixBuffer)
		_pIVPMatrixBuffer->Release();
	if (_pShadowMatrixBuffer)
		_pShadowMatrixBuffer->Release();

	_aligned_free(_VPMatrix);
	if (_shadow != NULL)
		delete _shadow;
}

void engine::Light::configShadowMap(const UINT &width, const UINT &height, ShaderProgram *shadow, ID3D11Device *pd3dDevice, ID3D11DeviceContext *pContext)
{
	if (_shadow != NULL)
		delete _shadow;
	_shadow = new ShadowMap;
	_shadow->config(width, height, shadow, pd3dDevice, pContext);
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
	if (_shadow == NULL)
	{
		MessageBox(NULL, "Need to config the ShadowMap before clearing", "Light", MB_OK);
		exit(1);
	}
	_shadow->clear();
}