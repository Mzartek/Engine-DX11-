#include <Engine/SpotLight.hpp>
#include <Engine/Buffer.hpp>
#include <Engine/ShaderProgram.hpp>
#include <Engine/ShadowMap.hpp>
#include <Engine/GBuffer.hpp>
#include <Engine/Camera.hpp>

Engine::SpotLight::SpotLight(const EngineDevice &EngineDevice, ShaderProgram *program)
	: Light(EngineDevice, program)
{
	_lightInfoBuffer->createStore(D3D11_BIND_CONSTANT_BUFFER, NULL, sizeof _lightInfo, D3D11_USAGE_DYNAMIC);

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "IN_POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	_EngineDevice.Device->CreateInputLayout(layout, ARRAYSIZE(layout), _program->getEntryBufferPointer(), _program->getEntryBytecodeLength(), &_pInputLayout);
}

Engine::SpotLight::~SpotLight(void)
{
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

void Engine::SpotLight::setShadowMapping(const BOOL &shadow)
{
	_lightInfo.withShadowMapping = shadow;
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

void Engine::SpotLight::position(void)
{
	XMVECTOR pos = XMVectorSet(_lightInfo.position.x, _lightInfo.position.y, _lightInfo.position.z, 0.0f);
	XMVECTOR dir = XMVectorSet(_lightInfo.direction.x, _lightInfo.direction.y, _lightInfo.direction.z, 0.0f);

	*_VPMatrix = XMMatrixLookAtRH(pos, pos + dir, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)) *
		XMMatrixPerspectiveFovRH(_lightInfo.spotCutOff * 2 * ((FLOAT)XM_PI / 180), (FLOAT)_shadow->getWidth() / _shadow->getHeight(), 0.1f, 1000.0f);
}

void Engine::SpotLight::display(GBuffer *gbuf, Camera *cam)
{
	gbuf->setLightState();

	_EngineDevice.DeviceContext->VSSetShader(_program->getVertexShader(), NULL, 0);
	_EngineDevice.DeviceContext->HSSetShader(_program->getHullShader(), NULL, 0);
	_EngineDevice.DeviceContext->DSSetShader(_program->getDomainShader(), NULL, 0);
	_EngineDevice.DeviceContext->GSSetShader(_program->getGeometryShader(), NULL, 0);
	_EngineDevice.DeviceContext->PSSetShader(_program->getPixelShader(), NULL, 0);

	ID3D11ShaderResourceView *gshr[]
	{
		gbuf->getShaderResourceView(GBUF_NORMAL),
		gbuf->getShaderResourceView(GBUF_MATERIAL),
		gbuf->getShaderResourceView(GBUF_DEPTH),
		gbuf->getShaderResourceView(GBUF_STENCIL),
	};
	_EngineDevice.DeviceContext->PSSetShaderResources(0, ARRAYSIZE(gshr), gshr);

	// ShadowMap
	if (_lightInfo.withShadowMapping == TRUE)
	{
		ID3D11ShaderResourceView *shadowResourceView = _shadow->getShaderResourceView();
		ID3D11SamplerState *shadowSampler = _shadow->getSamplerComparisonState();
		_EngineDevice.DeviceContext->PSSetShaderResources(ARRAYSIZE(gshr), 1, &shadowResourceView);
		_EngineDevice.DeviceContext->PSSetSamplers(0, 1, &shadowSampler);

		_shadowMatrixBuffer->updateStoreMap(_VPMatrix);
	}

	XMMATRIX matrix = XMMatrixInverse(NULL, cam->getVPMatrix());
	XMUINT2 screen(gbuf->getWidth(), gbuf->getHeight());
	XMVECTOR pos = cam->getPositionCamera();

	_IVPMatrixBuffer->updateStoreMap(&matrix);
	_screenBuffer->updateStoreMap(&screen);
	_cameraBuffer->updateStoreMap(&pos);
	_lightInfoBuffer->updateStoreMap(&_lightInfo);

	ID3D11Buffer *buf[] =
	{
		_shadowMatrixBuffer->getBuffer(),
		_IVPMatrixBuffer->getBuffer(),
		_screenBuffer->getBuffer(),
		_cameraBuffer->getBuffer(),
		_lightInfoBuffer->getBuffer(),
	};
	_EngineDevice.DeviceContext->PSSetConstantBuffers(0, ARRAYSIZE(buf), buf);

	UINT stride = 2 * sizeof(FLOAT), offset = 0;
	ID3D11Buffer *drawBuf[] =
	{
		_vertexBuffer->getBuffer(),
	};
	_EngineDevice.DeviceContext->IASetVertexBuffers(0, 1, &drawBuf[0], &stride, &offset);
	_EngineDevice.DeviceContext->IASetInputLayout(_pInputLayout);
	_EngineDevice.DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	_EngineDevice.DeviceContext->Draw(4, 0);
}