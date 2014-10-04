#include <Engine/Screen.hpp>
#include <Engine/Buffer.hpp>
#include <Engine/ShaderProgram.hpp>
#include <Engine/GBuffer.hpp>
#include <Engine/Renderer.hpp>

Engine::Screen::Screen(const EngineDevice &EngineDevice, ShaderProgram *backgroundProgram, ShaderProgram *directProgram)
{
	_EngineDevice = EngineDevice;
	_vertexBuffer = new Buffer(_EngineDevice);
	_screenColorBuffer = new Buffer(_EngineDevice);

	FLOAT vertex[] = {
		-1, -1,
		1, -1,
		-1, 1,
		1, 1,
	};
	_vertexBuffer->createStore(D3D11_BIND_VERTEX_BUFFER, vertex, sizeof vertex, D3D11_USAGE_IMMUTABLE);
	_screenColorBuffer->createStore(D3D11_BIND_CONSTANT_BUFFER, NULL, sizeof XMFLOAT4, D3D11_USAGE_DYNAMIC);

	_backgroundProgram = backgroundProgram;
	_directProgram = directProgram;

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "IN_POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	_EngineDevice.Device->CreateInputLayout(layout, ARRAYSIZE(layout), _directProgram->getEntryBufferPointer(), _directProgram->getEntryBytecodeLength(), &_pInputLayout);
}

Engine::Screen::~Screen(void)
{
	delete _vertexBuffer;
	delete _screenColorBuffer;
	_pInputLayout->Release();
}

void Engine::Screen::background(GBuffer *gbuf)
{
	gbuf->setBackgroundState();

	// Shader
	_EngineDevice.DeviceContext->VSSetShader(_backgroundProgram->getVertexShader(), NULL, 0);
	_EngineDevice.DeviceContext->HSSetShader(_backgroundProgram->getHullShader(), NULL, 0);
	_EngineDevice.DeviceContext->DSSetShader(_backgroundProgram->getDomainShader(), NULL, 0);
	_EngineDevice.DeviceContext->GSSetShader(_backgroundProgram->getGeometryShader(), NULL, 0);
	_EngineDevice.DeviceContext->PSSetShader(_backgroundProgram->getPixelShader(), NULL, 0);

	// Texture
	ID3D11ShaderResourceView *pshr[] =
	{
		gbuf->getShaderResourceView(GBUF_MATERIAL),
		gbuf->getShaderResourceView(GBUF_LIGHT),
	};
	_EngineDevice.DeviceContext->PSSetShaderResources(0, ARRAYSIZE(pshr), pshr);

	// Vertex Buffer
	UINT stride = 2 * sizeof(FLOAT), offset = 0;
	ID3D11Buffer *drawBuf[] =
	{
		_vertexBuffer->getBuffer(),
	};
	_EngineDevice.DeviceContext->IASetVertexBuffers(0, ARRAYSIZE(drawBuf), drawBuf, &stride, &offset);
	_EngineDevice.DeviceContext->IASetInputLayout(_pInputLayout);
	_EngineDevice.DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	_EngineDevice.DeviceContext->Draw(4, 0);

	gbuf->clearLight();
}

void Engine::Screen::display(Renderer *renderer, GBuffer *gbuf, const FLOAT &r, const FLOAT &g, const FLOAT &b, const FLOAT &a)
{
	renderer->setState();

	// Shader
	_EngineDevice.DeviceContext->VSSetShader(_directProgram->getVertexShader(), NULL, 0);
	_EngineDevice.DeviceContext->HSSetShader(_directProgram->getHullShader(), NULL, 0);
	_EngineDevice.DeviceContext->DSSetShader(_directProgram->getDomainShader(), NULL, 0);
	_EngineDevice.DeviceContext->GSSetShader(_directProgram->getGeometryShader(), NULL, 0);
	_EngineDevice.DeviceContext->PSSetShader(_directProgram->getPixelShader(), NULL, 0);

	// Texture
	ID3D11ShaderResourceView *pshr[] =
	{
		gbuf->getShaderResourceView(GBUF_BACKGROUND),
	};
	_EngineDevice.DeviceContext->PSSetShaderResources(0, ARRAYSIZE(pshr), pshr);

	XMFLOAT4 color(r, g, b, a);
	_screenColorBuffer->updateStoreMap(&color);

	ID3D11Buffer *buf[] =
	{
		_screenColorBuffer->getBuffer(),
	};
	_EngineDevice.DeviceContext->PSSetConstantBuffers(0, ARRAYSIZE(buf), buf);

	// Vertex Buffer
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
