#include <Engine/SkyBox.hpp>
#include <Engine/Texture.hpp>
#include <Engine/Buffer.hpp>
#include <Engine/ShaderProgram.hpp>
#include <Engine/GBuffer.hpp>
#include <Engine/Camera.hpp>

Engine::SkyBox::SkyBox(const EngineDevice &EngineDevice, ShaderProgram *program)
{
	_EngineDevice = EngineDevice;
	_cubeTexture = new Texture(_EngineDevice);
	_vertexBuffer = new Buffer(_EngineDevice);
	_indexBuffer = new Buffer(_EngineDevice);
	_MVPMatrixBuffer = new Buffer(_EngineDevice);
	_rotateMatrix = (XMMATRIX *)_aligned_malloc(sizeof *_rotateMatrix, 16);

	FLOAT vertexArray[] =
	{
		+SKYBOX_DIM, -SKYBOX_DIM, -SKYBOX_DIM,
		-SKYBOX_DIM, -SKYBOX_DIM, -SKYBOX_DIM,
		-SKYBOX_DIM, -SKYBOX_DIM, +SKYBOX_DIM,
		+SKYBOX_DIM, -SKYBOX_DIM, +SKYBOX_DIM,
		+SKYBOX_DIM, +SKYBOX_DIM, -SKYBOX_DIM,
		-SKYBOX_DIM, +SKYBOX_DIM, -SKYBOX_DIM,
		-SKYBOX_DIM, +SKYBOX_DIM, +SKYBOX_DIM,
		+SKYBOX_DIM, +SKYBOX_DIM, +SKYBOX_DIM
	};
	UINT indexArray[] =
	{
		0, 1, 2, 0, 2, 3,
		4, 7, 6, 4, 6, 5,
		0, 4, 5, 0, 5, 1,
		3, 2, 6, 3, 6, 7,
		1, 5, 6, 1, 6, 2,
		0, 3, 7, 0, 7, 4
	};
	_numElement = sizeof indexArray / sizeof(UINT);
	_vertexBuffer->createStore(D3D11_BIND_VERTEX_BUFFER, vertexArray, sizeof vertexArray, D3D11_USAGE_IMMUTABLE);
	_indexBuffer->createStore(D3D11_BIND_INDEX_BUFFER, indexArray, sizeof indexArray, D3D11_USAGE_IMMUTABLE);
	_MVPMatrixBuffer->createStore(D3D11_BIND_CONSTANT_BUFFER, NULL, sizeof XMMATRIX, D3D11_USAGE_DYNAMIC);
	*_rotateMatrix = XMMatrixIdentity();
	
	_program = program;

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "IN_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	_EngineDevice.Device->CreateInputLayout(layout, ARRAYSIZE(layout), _program->getEntryBufferPointer(), _program->getEntryBytecodeLength(), &_pInputLayout);
}

Engine::SkyBox::~SkyBox(void)
{
	delete _cubeTexture;
	delete _vertexBuffer;
	delete _indexBuffer;
	delete _MVPMatrixBuffer;
	_aligned_free(_rotateMatrix);
	_pInputLayout->Release();
}

void Engine::SkyBox::load(const CHAR *posx, const CHAR *negx,
	const CHAR *posy, const CHAR *negy,
	const CHAR *posz, const CHAR *negz)
{
	_cubeTexture->loadCubeTextureFromFiles(posx, negx, posy, negy, posz, negz);
}

void Engine::SkyBox::rotate(const FLOAT &angle, const FLOAT &x, const FLOAT &y, const FLOAT &z)
{
	*_rotateMatrix = XMMatrixRotationAxis(XMVectorSet(x, y, z, 1.0f), angle * ((FLOAT)XM_PI / 180)) * *_rotateMatrix;
}

void Engine::SkyBox::display(GBuffer *gbuf, Camera *cam)
{
	XMMATRIX pos = XMMatrixTranslation(cam->getPositionCamera().x, cam->getPositionCamera().y, cam->getPositionCamera().z);
	pos = *_rotateMatrix * pos;
	pos *= cam->getVPMatrix();

	gbuf->setSkyboxState();

	_EngineDevice.DeviceContext->VSSetShader(_program->getVertexShader(), NULL, 0);
	_EngineDevice.DeviceContext->HSSetShader(_program->getHullShader(), NULL, 0);
	_EngineDevice.DeviceContext->DSSetShader(_program->getDomainShader(), NULL, 0);
	_EngineDevice.DeviceContext->GSSetShader(_program->getGeometryShader(), NULL, 0);
	_EngineDevice.DeviceContext->PSSetShader(_program->getPixelShader(), NULL, 0);

	_MVPMatrixBuffer->updateStoreMap(&pos);

	ID3D11Buffer *buf[] =
	{
		_MVPMatrixBuffer->getBuffer(),
	};
	_EngineDevice.DeviceContext->VSSetConstantBuffers(0, ARRAYSIZE(buf), buf);

	ID3D11ShaderResourceView *pshr[] =
	{
		_cubeTexture->getShaderResourceView(),
	};
	_EngineDevice.DeviceContext->PSSetShaderResources(0, ARRAYSIZE(pshr), pshr);
	ID3D11SamplerState *psam[] =
	{
		_cubeTexture->getSamplerState(),
	};
	_EngineDevice.DeviceContext->PSSetSamplers(0, ARRAYSIZE(psam), psam);

	// Vertex And Index Buffer
	UINT stride = 3 * sizeof(FLOAT), offset = 0;
	ID3D11Buffer *drawBuf[] =
	{
		_vertexBuffer->getBuffer(),
		_indexBuffer->getBuffer(),
	};
	_EngineDevice.DeviceContext->IASetVertexBuffers(0, 1, &drawBuf[0], &stride, &offset);
	_EngineDevice.DeviceContext->IASetIndexBuffer(drawBuf[1], DXGI_FORMAT_R32_UINT, offset);
	_EngineDevice.DeviceContext->IASetInputLayout(_pInputLayout);
	_EngineDevice.DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	_EngineDevice.DeviceContext->DrawIndexed(_numElement, 0, 0);
}
