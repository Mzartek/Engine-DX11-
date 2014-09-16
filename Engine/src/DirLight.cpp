#include <Engine/DirLight.hpp>
#include <Engine/ShadowMap.hpp>
#include <Engine/ShaderProgram.hpp>
#include <Engine/GBuffer.hpp>
#include <Engine/Camera.hpp>

engine::DirLight::DirLight(void)
{
}

engine::DirLight::~DirLight(void)
{
}

void engine::DirLight::config(ShaderProgram *program, ID3D11Device *pd3dDevice, ID3D11DeviceContext *pContext)
{
	HRESULT hr;
	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA data;

	if (_pInputLayout) _pInputLayout->Release();
	if (_pVertexBuffer) _pVertexBuffer->Release();
	if (_pShadowMatrixBuffer) _pShadowMatrixBuffer->Release();
	if (_pIVPMatrixBuffer) _pIVPMatrixBuffer->Release();
	if (_pScreenBuffer) _pScreenBuffer->Release();
	if (_pCameraBuffer) _pCameraBuffer->Release();
	if (_pLightInfoBuffer) _pLightInfoBuffer->Release();

	_program = program;
	_pContext = pContext;

	// Create and set the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "IN_POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	hr = pd3dDevice->CreateInputLayout(layout, ARRAYSIZE(layout),
		program->getEntryBufferPointer(), program->getEntryBytecodeLength(),
		&_pInputLayout);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create Input Layout", "DirLight", MB_OK);
		exit(1);
	}

	// Create Vertex Buffer
	FLOAT vertex[] = {
		-1, -1,
		1, -1,
		-1, 1,
		1, 1,
	};
	bd.ByteWidth = sizeof vertex;
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	data.pSysMem = vertex;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;
	hr = pd3dDevice->CreateBuffer(&bd, &data, &_pVertexBuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create Vertex Buffer", "DirLight", MB_OK);
		exit(1);
	}

	// ShadowMatrix Buffer
	bd.ByteWidth = sizeof XMMATRIX;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	hr = pd3dDevice->CreateBuffer(&bd, NULL, &_pShadowMatrixBuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create ShadowMatrix Buffer", "DirLight", MB_OK);
		exit(1);
	}

	// IVPMatrix Buffer
	bd.ByteWidth = sizeof XMMATRIX;
	hr = pd3dDevice->CreateBuffer(&bd, NULL, &_pIVPMatrixBuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create IVPMatrix Buffer", "DirLight", MB_OK);
		exit(1);
	}

	// Screen Buffer
	bd.ByteWidth = 16;
	hr = pd3dDevice->CreateBuffer(&bd, NULL, &_pScreenBuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create Screen Buffer", "DirLight", MB_OK);
		exit(1);
	}

	// Camera Buffer
	bd.ByteWidth = 16;
	hr = pd3dDevice->CreateBuffer(&bd, NULL, &_pCameraBuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create Camera Buffer", "DirLight", MB_OK);
		exit(1);
	}

	// LightInfo Buffer
	bd.ByteWidth = sizeof _lightInfo;
	hr = pd3dDevice->CreateBuffer(&bd, NULL, &_pLightInfoBuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create LightInfo Buffer", "DirLight", MB_OK);
		exit(1);
	}
}

void engine::DirLight::setColor(const XMFLOAT3 &color)
{
	_lightInfo.color = color;
}

void engine::DirLight::setDirection(const XMFLOAT3 &dir)
{
	_lightInfo.direction = dir;
}

void engine::DirLight::setShadowMapping(const BOOL &shadow)
{
	_lightInfo.withShadowMapping = shadow;
}

XMFLOAT3 engine::DirLight::getColor(void) const
{
	return _lightInfo.color;
}

XMFLOAT3 engine::DirLight::getDirection(void) const
{
	return _lightInfo.direction;
}

void engine::DirLight::position(const XMFLOAT3 &position, const FLOAT &dim)
{
	XMVECTOR pos = XMVectorSet(position.x, position.y, position.z, 0.0f);
	XMVECTOR dir = XMVectorSet(_lightInfo.direction.x, _lightInfo.direction.y, _lightInfo.direction.z, 0.0f);

	*_VPMatrix = XMMatrixLookAtRH(pos - dir, pos, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)) *
		XMMatrixOrthographicOffCenterRH(-dim, dim, -dim, dim, -dim, dim);
}

void engine::DirLight::display(GBuffer *gbuf, Camera *cam)
{	gbuf->setLightState();

	gbuf->getContext()->VSSetShader(_program->getVertexShader(), NULL, 0);
	gbuf->getContext()->GSSetShader(_program->getGeometryShader(), NULL, 0);
	gbuf->getContext()->PSSetShader(_program->getPixelShader(), NULL, 0);

	ID3D11ShaderResourceView *gshr[]
	{
		gbuf->getShaderResourceView(GBUF_NORMAL),
		gbuf->getShaderResourceView(GBUF_MATERIAL),
		gbuf->getShaderResourceView(GBUF_DEPTH),
		gbuf->getShaderResourceView(GBUF_STENCIL),
	};
	gbuf->getContext()->PSSetShaderResources(0, ARRAYSIZE(gshr), gshr);

	// ShadowMap
	if (_lightInfo.withShadowMapping == TRUE)
	{
		ID3D11ShaderResourceView *shadowResourceView = _shadow->getShaderResourceView();
		ID3D11SamplerState *shadowSampler = _shadow->getSamplerComparisonState();
		gbuf->getContext()->PSSetShaderResources(ARRAYSIZE(gshr), 1, &shadowResourceView);
		gbuf->getContext()->PSSetSamplers(0, 1, &shadowSampler);

		updateDynamicBuffer(_pShadowMatrixBuffer, _VPMatrix, sizeof *_VPMatrix, gbuf->getContext());
		gbuf->getContext()->PSSetConstantBuffers(0, 1, &_pShadowMatrixBuffer);
	}

	XMMATRIX matrix = XMMatrixInverse(NULL, cam->getVPMatrix());
	updateDynamicBuffer(_pIVPMatrixBuffer, &matrix, sizeof matrix, gbuf->getContext());
	gbuf->getContext()->PSSetConstantBuffers(1, 1, &_pIVPMatrixBuffer);

	XMUINT2 screen(gbuf->getWidth(), gbuf->getHeight());
	updateDynamicBuffer(_pScreenBuffer, &screen, sizeof screen, gbuf->getContext());
	gbuf->getContext()->PSSetConstantBuffers(2, 1, &_pScreenBuffer);

	XMFLOAT3 pos = cam->getPositionCamera();
	updateDynamicBuffer(_pCameraBuffer, &pos, sizeof pos, gbuf->getContext());
	gbuf->getContext()->PSSetConstantBuffers(3, 1, &_pCameraBuffer);

	updateDynamicBuffer(_pLightInfoBuffer, &_lightInfo, sizeof _lightInfo, _pContext);
	gbuf->getContext()->PSSetConstantBuffers(4, 1, &_pLightInfoBuffer);

	UINT stride = 2 * sizeof(FLOAT), offset = 0;
	gbuf->getContext()->IASetVertexBuffers(0, 1, &_pVertexBuffer, &stride, &offset);
	gbuf->getContext()->IASetInputLayout(_pInputLayout);
	gbuf->getContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	gbuf->getContext()->Draw(4, 0);
}