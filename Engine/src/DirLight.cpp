#include <Engine/DirLight.hpp>
#include <Engine/Buffer.hpp>
#include <Engine/ShadowMap.hpp>
#include <Engine/ShaderProgram.hpp>
#include <Engine/GBuffer.hpp>
#include <Engine/Camera.hpp>

extern ID3D11Device1 *Device;
extern ID3D11DeviceContext1 *DeviceContext;

Engine::DirLight::DirLight(void)
{
}

Engine::DirLight::~DirLight(void)
{
}

void Engine::DirLight::config(ShaderProgram *program)
{
	_program = program;

	if (_pInputLayout) _pInputLayout->Release();

	// Create and set the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "IN_POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	Device->CreateInputLayout(layout, ARRAYSIZE(layout), program->getEntryBufferPointer(), program->getEntryBytecodeLength(), &_pInputLayout);

	// Create Vertex Buffer
	FLOAT vertex[] = {
		-1, -1,
		1, -1,
		-1, 1,
		1, 1,
	};
	_vertexBuffer->createStore(D3D11_BIND_VERTEX_BUFFER, vertex, sizeof vertex, D3D11_USAGE_IMMUTABLE);
	_shadowMatrixBuffer->createStore(D3D11_BIND_CONSTANT_BUFFER, NULL, sizeof XMMATRIX, D3D11_USAGE_DYNAMIC);
	_IVPMatrixBuffer->createStore(D3D11_BIND_CONSTANT_BUFFER, NULL, sizeof XMMATRIX, D3D11_USAGE_DYNAMIC);
	_screenBuffer->createStore(D3D11_BIND_CONSTANT_BUFFER, NULL, 16, D3D11_USAGE_DYNAMIC);
	_cameraBuffer->createStore(D3D11_BIND_CONSTANT_BUFFER, NULL, 16, D3D11_USAGE_DYNAMIC);
	_lightInfoBuffer->createStore(D3D11_BIND_CONSTANT_BUFFER, NULL, sizeof _lightInfo, D3D11_USAGE_DYNAMIC);
}

void Engine::DirLight::setColor(const XMFLOAT3 &color)
{
	_lightInfo.color = color;
}

void Engine::DirLight::setDirection(const XMFLOAT3 &dir)
{
	_lightInfo.direction = dir;
}

void Engine::DirLight::setShadowMapping(const BOOL &shadow)
{
	_lightInfo.withShadowMapping = shadow;
}

XMFLOAT3 Engine::DirLight::getColor(void) const
{
	return _lightInfo.color;
}

XMFLOAT3 Engine::DirLight::getDirection(void) const
{
	return _lightInfo.direction;
}

void Engine::DirLight::position(const XMFLOAT3 &position, const FLOAT &dim)
{
	XMVECTOR pos = XMVectorSet(position.x, position.y, position.z, 0.0f);
	XMVECTOR dir = XMVectorSet(_lightInfo.direction.x, _lightInfo.direction.y, _lightInfo.direction.z, 0.0f);

	*_VPMatrix = XMMatrixLookAtRH(pos - dir, pos, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)) *
		XMMatrixOrthographicOffCenterRH(-dim, dim, -dim, dim, -dim, dim);
}

void Engine::DirLight::display(GBuffer *gbuf, Camera *cam)
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