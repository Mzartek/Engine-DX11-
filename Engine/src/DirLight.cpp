#include <Engine/DirLight.hpp>
#include <Engine/Buffer.hpp>
#include <Engine/DepthMap.hpp>
#include <Engine/ShaderProgram.hpp>
#include <Engine/GBuffer.hpp>
#include <Engine/Camera.hpp>

Engine::DirLight::DirLight(ShaderProgram *program)
	: Light(program)
{
	_projectionMatrix = (XMMATRIX *)_aligned_malloc(CSM_NUM * sizeof *_projectionMatrix, 16);
	_viewMatrix = (XMMATRIX *)_aligned_malloc(CSM_NUM * sizeof *_viewMatrix, 16);
	_VPMatrix = (XMMATRIX *)_aligned_malloc(CSM_NUM * sizeof *_VPMatrix, 16);

	_lightInfoBuffer->createStore(D3D11_BIND_CONSTANT_BUFFER, NULL, sizeof _lightInfo, D3D11_USAGE_DYNAMIC);

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "IN_POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	Device->CreateInputLayout(layout, ARRAYSIZE(layout), _program->getEntryBufferPointer(), _program->getEntryBytecodeLength(), &_pInputLayout);
}

Engine::DirLight::~DirLight(void)
{
	_aligned_free(_projectionMatrix);
	_aligned_free(_viewMatrix);
	_aligned_free(_VPMatrix);

	_pInputLayout->Release();
}

void Engine::DirLight::setColor(const XMVECTOR &color)
{
	XMStoreFloat3(&_lightInfo.color, color);
}

void Engine::DirLight::setDirection(const XMVECTOR &dir)
{
	XMStoreFloat3(&_lightInfo.direction, dir);
}

XMMATRIX Engine::DirLight::getProjectionMatrix(const UINT &num) const
{
	return _projectionMatrix[num];
}

XMMATRIX Engine::DirLight::getViewMatrix(const UINT &num) const
{
	return _viewMatrix[num];
}

XMMATRIX Engine::DirLight::getVPMatrix(const UINT &num) const
{
	return _VPMatrix[num];
}

XMVECTOR Engine::DirLight::getColor(void) const
{
	return XMLoadFloat3(&_lightInfo.color);
}

XMVECTOR Engine::DirLight::getDirection(void) const
{
	return XMLoadFloat3(&_lightInfo.direction);
}

void Engine::DirLight::position(const XMVECTOR &pos, const FLOAT &dim0, const FLOAT &dim1, const FLOAT &dim2)
{
	FLOAT dim[3] = { dim0, dim1, dim2 };
	XMVECTOR dir = XMVectorSet(_lightInfo.direction.x, _lightInfo.direction.y, _lightInfo.direction.z, 0.0f);

	for (UINT i = 0; i < CSM_NUM; i++)
	{
		_projectionMatrix[i] = XMMatrixOrthographicOffCenterRH(-dim[i], dim[i], -dim[i], dim[i], -dim[i] / 2, dim[i] / 2);
		_viewMatrix[i] = XMMatrixLookAtRH(pos - dir, pos, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
		_VPMatrix[i] = _viewMatrix[i] * _projectionMatrix[i];
	}

	memcpy(_lightInfo.shadowMatrix, _VPMatrix, 3 * sizeof(XMMATRIX));

	/*XMFLOAT4X4 mattest;
	XMStoreFloat4x4(&mattest, _projectionMatrix[0]);
	std::string test =
	std::to_string(mattest._11) + " " + std::to_string(mattest._12) + " " + std::to_string(mattest._13) + " " + std::to_string(mattest._14) + "\n" +
	std::to_string(mattest._21) + " " + std::to_string(mattest._22) + " " + std::to_string(mattest._23) + " " + std::to_string(mattest._24) + "\n" +
	std::to_string(mattest._31) + " " + std::to_string(mattest._32) + " " + std::to_string(mattest._33) + " " + std::to_string(mattest._34) + "\n" +
	std::to_string(mattest._41) + " " + std::to_string(mattest._42) + " " + std::to_string(mattest._43) + " " + std::to_string(mattest._44) + "\n";
	MessageBoxA(NULL, test.c_str(), "DirectX", MB_OK);
	exit(0);*/
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

	_mainInfo.IVPMatrix = cam->getIVPMatrix();
	_mainInfo.screen = XMUINT2(gbuf->getWidth(), gbuf->getHeight());
	_mainInfo.camPosition = cam->getCameraPosition();
	_mainInfo.withShadowMapping = FALSE;

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

void Engine::DirLight::display(GBuffer *gbuf, DepthMap *dmaps, Camera *cam)
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
	ID3D11ShaderResourceView *shadowResourceView[] =
	{
		dmaps[0].getShaderResourceView(),
		dmaps[1].getShaderResourceView(),
		dmaps[2].getShaderResourceView(),
	};
	ID3D11SamplerState *shadowSampler = dmaps->getSamplerComparisonState();
	DeviceContext->PSSetShaderResources(ARRAYSIZE(gshr), ARRAYSIZE(shadowResourceView), shadowResourceView);
	DeviceContext->PSSetSamplers(0, 1, &shadowSampler);

	_mainInfo.IVPMatrix = cam->getIVPMatrix();
	_mainInfo.screen = XMUINT2(gbuf->getWidth(), gbuf->getHeight());
	_mainInfo.camPosition = cam->getCameraPosition();
	_mainInfo.withShadowMapping = TRUE;

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