#include <Engine/SpotLight.hpp>
#include <Engine/Buffer.hpp>
#include <Engine/ShaderProgram.hpp>
#include <Engine/ShadowMap.hpp>
#include <Engine/GBuffer.hpp>
#include <Engine/Camera.hpp>

Engine::SpotLight::SpotLight(ShaderProgram *program)
	: Light(program)
{
	_shadow = new ShadowMap;
	_projectionMatrix = (XMMATRIX *)_aligned_malloc(sizeof *_projectionMatrix, 16);
	_viewMatrix = (XMMATRIX *)_aligned_malloc(sizeof *_viewMatrix, 16);
	_VPMatrix = (XMMATRIX *)_aligned_malloc(sizeof *_VPMatrix, 16);

	_lightInfoBuffer->createStore(D3D11_BIND_CONSTANT_BUFFER, NULL, sizeof _lightInfo, D3D11_USAGE_DYNAMIC);

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "IN_POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	Device->CreateInputLayout(layout, ARRAYSIZE(layout), _program->getEntryBufferPointer(), _program->getEntryBytecodeLength(), &_pInputLayout);
}

Engine::SpotLight::~SpotLight(void)
{
	delete _shadow;
	_aligned_free(_projectionMatrix);
	_aligned_free(_viewMatrix);
	_aligned_free(_VPMatrix);

	_pInputLayout->Release();
}

void Engine::SpotLight::setColor(const XMVECTOR &color)
{
	 XMStoreFloat3(&_lightInfo.color, color);
}

void Engine::SpotLight::setPosition(const XMVECTOR &position)
{
	XMStoreFloat3(&_lightInfo.position, position);
}

void Engine::SpotLight::setDirection(const XMVECTOR &dir)
{
	XMStoreFloat3(&_lightInfo.direction, dir);
}

void Engine::SpotLight::setSpotCutOff(const FLOAT &spot)
{
	_lightInfo.spotCutOff = spot;
}

void Engine::SpotLight::setMaxDistance(const FLOAT &maxDistance)
{
	_lightInfo.maxDistance = maxDistance;
}

void Engine::SpotLight::setShadowMapping(const BOOL &shadow)
{
	_lightInfo.withShadowMapping = shadow;
}

void Engine::SpotLight::configShadowMap(const UINT &width, const UINT &height) const
{
	_shadow->config(width, height);
}

Engine::ShadowMap *Engine::SpotLight::getShadowMap(void) const
{
	return _shadow;
}

XMMATRIX Engine::SpotLight::getProjectionMatrix(void) const
{
	return *_projectionMatrix;
}

XMMATRIX Engine::SpotLight::getViewMatrix(void) const
{
	return *_viewMatrix;
}

XMMATRIX Engine::SpotLight::getVPMatrix(void) const
{
	return *_VPMatrix;
}

XMVECTOR Engine::SpotLight::getColor(void) const
{
	return XMLoadFloat3(&_lightInfo.color);
}

XMVECTOR Engine::SpotLight::getPosition(void) const
{
	return XMLoadFloat3(&_lightInfo.position);
}

XMVECTOR Engine::SpotLight::getDirection(void) const
{
	return XMLoadFloat3(&_lightInfo.direction);
}

FLOAT Engine::SpotLight::getSpotCutOff(void) const
{
	return _lightInfo.spotCutOff;
}

FLOAT Engine::SpotLight::getMaxDistance(void) const
{
	return _lightInfo.maxDistance;
}

void Engine::SpotLight::position(void) const
{
	XMVECTOR pos = XMVectorSet(_lightInfo.position.x, _lightInfo.position.y, _lightInfo.position.z, 0.0f);
	XMVECTOR dir = XMVectorSet(_lightInfo.direction.x, _lightInfo.direction.y, _lightInfo.direction.z, 0.0f);

	*_projectionMatrix = XMMatrixPerspectiveFovRH(_lightInfo.spotCutOff * 2, (FLOAT)_shadow->getWidth() / _shadow->getHeight(), 0.1f, 1000.0f);
	*_viewMatrix = XMMatrixLookAtRH(pos, pos + dir, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	*_VPMatrix = *_viewMatrix * *_projectionMatrix;
}

void Engine::SpotLight::clear(void) const
{
	_shadow->clear();
}

void Engine::SpotLight::display(GBuffer *gbuf, Camera *cam)
{
	gbuf->setLightState();

	DeviceContext->VSSetShader(_program->getVertexShader(), NULL, 0);
	DeviceContext->HSSetShader(_program->getHullShader(), NULL, 0);
	DeviceContext->DSSetShader(_program->getDomainShader(), NULL, 0);
	DeviceContext->GSSetShader(_program->getGeometryShader(), NULL, 0);
	DeviceContext->PSSetShader(_program->getPixelShader(), NULL, 0);

	ID3D11ShaderResourceView *gshr[]
	{
		gbuf->getShaderResourceView(GBUF_NORMAL),
		gbuf->getShaderResourceView(GBUF_MATERIAL),
		gbuf->getShaderResourceView(GBUF_DEPTH),
		gbuf->getShaderResourceView(GBUF_STENCIL),
	};
	DeviceContext->PSSetShaderResources(0, ARRAYSIZE(gshr), gshr);

	// ShadowMap
	if (_lightInfo.withShadowMapping == TRUE)
	{
		ID3D11ShaderResourceView *shadowResourceView = _shadow->getShaderResourceView();
		ID3D11SamplerState *shadowSampler = _shadow->getSamplerComparisonState();
		DeviceContext->PSSetShaderResources(ARRAYSIZE(gshr), 1, &shadowResourceView);
		DeviceContext->PSSetSamplers(0, 1, &shadowSampler);

		_lightInfo.shadowMatrix = *_VPMatrix;
	}
	_mainInfo.IVPMatrix = cam->getIVPMatrix();
	_mainInfo.screen = XMUINT2(gbuf->getWidth(), gbuf->getHeight());
	_mainInfo.camPosition = cam->getCameraPosition();

	_mainInfoBuffer->updateStoreMap(&_mainInfo);
	_lightInfoBuffer->updateStoreMap(&_lightInfo);

	ID3D11Buffer *buf[] =
	{
		_mainInfoBuffer->getBuffer(),
		_lightInfoBuffer->getBuffer(),
	};
	DeviceContext->PSSetConstantBuffers(0, ARRAYSIZE(buf), buf);

	UINT stride = 2 * sizeof(FLOAT), offset = 0;
	ID3D11Buffer *drawBuf[] =
	{
		_vertexBuffer->getBuffer(),
	};
	DeviceContext->IASetVertexBuffers(0, 1, &drawBuf[0], &stride, &offset);
	DeviceContext->IASetInputLayout(_pInputLayout);
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	DeviceContext->Draw(4, 0);
}