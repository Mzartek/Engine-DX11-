#include <Engine/SkyBox.hpp>
#include <Engine/Texture.hpp>
#include <Engine/Buffer.hpp>
#include <Engine/ShaderProgram.hpp>
#include <Engine/GBuffer.hpp>
#include <Engine/Camera.hpp>

Engine::SkyBox::SkyBox(ShaderProgram *program)
	: _program(program)
{
	_cubeTexture = new Texture;
	_vertexBuffer = new Buffer;
	_indexBuffer = new Buffer;
	_MVPMatrixBuffer = new Buffer;

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

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "IN_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	Device->CreateInputLayout(layout, ARRAYSIZE(layout), _program->getEntryBufferPointer(), _program->getEntryBytecodeLength(), &_pInputLayout);
}

Engine::SkyBox::~SkyBox(void)
{
	delete _cubeTexture;
	delete _vertexBuffer;
	delete _indexBuffer;
	delete _MVPMatrixBuffer;
	_pInputLayout->Release();
}

void Engine::SkyBox::load(const CHAR *posx, const CHAR *negx,
	const CHAR *posy, const CHAR *negy,
	const CHAR *posz, const CHAR *negz)
{
	_cubeTexture->loadCubeTextureFromFiles(posx, negx, posy, negy, posz, negz);
}

Engine::Texture *Engine::SkyBox::getTexture(void) const
{
	return _cubeTexture;
}

void Engine::SkyBox::display(GBuffer *gbuf, Camera *cam)
{
	XMMATRIX pos = XMMatrixTranslation(XMVectorGetX(cam->getCameraPosition()), XMVectorGetY(cam->getCameraPosition()), XMVectorGetZ(cam->getCameraPosition())) * cam->getVPMatrix();

	gbuf->setSkyboxState();

	DeviceContext->VSSetShader(_program->getVertexShader(), NULL, 0);
	DeviceContext->HSSetShader(_program->getHullShader(), NULL, 0);
	DeviceContext->DSSetShader(_program->getDomainShader(), NULL, 0);
	DeviceContext->GSSetShader(_program->getGeometryShader(), NULL, 0);
	DeviceContext->PSSetShader(_program->getPixelShader(), NULL, 0);

	_MVPMatrixBuffer->updateStoreMap(&pos);

	ID3D11Buffer *buf[] =
	{
		_MVPMatrixBuffer->getBuffer(),
	};
	DeviceContext->VSSetConstantBuffers(0, ARRAYSIZE(buf), buf);

	ID3D11ShaderResourceView *pshr[] =
	{
		_cubeTexture->getShaderResourceView(),
	};
	DeviceContext->PSSetShaderResources(0, ARRAYSIZE(pshr), pshr);
	ID3D11SamplerState *psam[] =
	{
		_cubeTexture->getSamplerState(),
	};
	DeviceContext->PSSetSamplers(0, ARRAYSIZE(psam), psam);

	// Vertex And Index Buffer
	UINT stride = 3 * sizeof(FLOAT), offset = 0;
	ID3D11Buffer *drawBuf[] =
	{
		_vertexBuffer->getBuffer(),
		_indexBuffer->getBuffer(),
	};
	DeviceContext->IASetVertexBuffers(0, 1, &drawBuf[0], &stride, &offset);
	DeviceContext->IASetIndexBuffer(drawBuf[1], DXGI_FORMAT_R32_UINT, offset);
	DeviceContext->IASetInputLayout(_pInputLayout);
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DeviceContext->DrawIndexed(_numElement, 0, 0);
}
