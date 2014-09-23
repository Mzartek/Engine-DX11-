#include <Engine/SpotLight.hpp>
#include <Engine/Buffer.hpp>
#include <Engine/ShaderProgram.hpp>
#include <Engine/ShadowMap.hpp>
#include <Engine/GBuffer.hpp>
#include <Engine/Camera.hpp>

extern ID3D11Device *Device;
extern ID3D11DeviceContext *DeviceContext;

engine::SpotLight::SpotLight(void)
{
}

engine::SpotLight::~SpotLight(void)
{
}

void engine::SpotLight::config(ShaderProgram *program)
{
	HRESULT hr;

	_program = program;

	if (_pInputLayout) _pInputLayout->Release();

	// Create and set the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "IN_POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	hr = Device->CreateInputLayout(layout, ARRAYSIZE(layout),
		program->getEntryBufferPointer(), program->getEntryBytecodeLength(),
		&_pInputLayout);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Failed to create Input Layout", TEXT(__FILE__), MB_OK);
		exit(1);
	}

	// Create Vertex Buffer
	FLOAT vertex[] = {
		-1, -1,
		1, -1,
		-1, 1,
		1, 1,
	};
	_vertexBuffer->createStore(D3D11_BIND_VERTEX_BUFFER, vertex, sizeof vertex, D3D11_USAGE_IMMUTABLE);

	// ShadowMatrix Buffer
	_shadowMatrixBuffer->createStore(D3D11_BIND_CONSTANT_BUFFER, NULL, sizeof XMMATRIX, D3D11_USAGE_DYNAMIC);

	// IVPMatrix Buffer
	_IVPMatrixBuffer->createStore(D3D11_BIND_CONSTANT_BUFFER, NULL, sizeof XMMATRIX, D3D11_USAGE_DYNAMIC);

	// Screen Buffer
	_screenBuffer->createStore(D3D11_BIND_CONSTANT_BUFFER, NULL, 16, D3D11_USAGE_DYNAMIC);

	// Camera Buffer
	_cameraBuffer->createStore(D3D11_BIND_CONSTANT_BUFFER, NULL, 16, D3D11_USAGE_DYNAMIC);

	// LightInfo Buffer
	_lightInfoBuffer->createStore(D3D11_BIND_CONSTANT_BUFFER, NULL, sizeof _lightInfo, D3D11_USAGE_DYNAMIC);
}

void engine::SpotLight::setColor(const XMFLOAT3 &color)
{
	_lightInfo.color = color;
}

void engine::SpotLight::setPosition(const XMFLOAT3 &position)
{
	_lightInfo.position = position;
}

void engine::SpotLight::setDirection(const XMFLOAT3 &dir)
{
	_lightInfo.direction = dir;
}

void engine::SpotLight::setSpotCutOff(const FLOAT &spot)
{
	_lightInfo.spotCutOff = spot;
}

void engine::SpotLight::setShadowMapping(const BOOL &shadow)
{
	_lightInfo.withShadowMapping = shadow;
}

XMFLOAT3 engine::SpotLight::getColor(void) const
{
	return _lightInfo.color;
}

XMFLOAT3 engine::SpotLight::getPosition(void) const
{
	return _lightInfo.position;
}

XMFLOAT3 engine::SpotLight::getDirection(void) const
{
	return _lightInfo.direction;
}

FLOAT engine::SpotLight::getSpotCutOff(void) const
{
	return _lightInfo.spotCutOff;
}

void engine::SpotLight::position(void)
{
	XMVECTOR pos = XMVectorSet(_lightInfo.position.x, _lightInfo.position.y, _lightInfo.position.z, 0.0f);
	XMVECTOR dir = XMVectorSet(_lightInfo.direction.x, _lightInfo.direction.y, _lightInfo.direction.z, 0.0f);

	*_VPMatrix = XMMatrixLookAtRH(pos, pos + dir, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)) *
		XMMatrixPerspectiveFovRH(_lightInfo.spotCutOff * 2 * ((FLOAT)XM_PI / 180), (FLOAT)_shadow->getWidth() / _shadow->getHeight(), 0.1f, 1000.0f);
}

void engine::SpotLight::display(GBuffer *gbuf, Camera *cam)
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

		_shadowMatrixBuffer->updateStoreMap(_VPMatrix);
	}

	XMMATRIX matrix = XMMatrixInverse(NULL, cam->getVPMatrix());
	XMUINT2 screen(gbuf->getWidth(), gbuf->getHeight());
	XMFLOAT3 pos = cam->getPositionCamera();

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