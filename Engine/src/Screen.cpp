#include <Engine/Screen.hpp>
#include <Engine/Buffer.hpp>
#include <Engine/ShaderProgram.hpp>
#include <Engine/GBuffer.hpp>
#include <Engine/Renderer.hpp>

extern ID3D11Device1 *Device;
extern ID3D11DeviceContext1 *DeviceContext;

Engine::Screen::Screen(void)
	: _pInputLayout(NULL)
{
	_vertexBuffer = new Buffer;
	_screenColorBuffer = new Buffer;
}

Engine::Screen::~Screen(void)
{
	if (_pInputLayout) _pInputLayout->Release();
	delete _vertexBuffer;
	delete _screenColorBuffer;
}

void Engine::Screen::config(ShaderProgram *backgroundProgram, ShaderProgram *directProgram)
{
	_backgroundProgram = backgroundProgram;
	_directProgram = directProgram;

	if (_pInputLayout) _pInputLayout->Release();

	// Create Input Layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "IN_POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	Device->CreateInputLayout(layout, ARRAYSIZE(layout), _directProgram->getEntryBufferPointer(), _directProgram->getEntryBytecodeLength(), &_pInputLayout);

	// Create Vertex Buffer
	FLOAT vertex[] = {
		-1, -1,
		1, -1,
		-1, 1,
		1, 1,
	};
	_vertexBuffer->createStore(D3D11_BIND_VERTEX_BUFFER, vertex, sizeof vertex, D3D11_USAGE_IMMUTABLE);

	// Create Color Buffer
	_screenColorBuffer->createStore(D3D11_BIND_CONSTANT_BUFFER, NULL, sizeof XMFLOAT4, D3D11_USAGE_DYNAMIC);
}

void Engine::Screen::background(GBuffer *gbuf)
{
	gbuf->setBackgroundState();

	// Shader
	DeviceContext->VSSetShader(_backgroundProgram->getVertexShader(), NULL, 0);
	DeviceContext->HSSetShader(_backgroundProgram->getHullShader(), NULL, 0);
	DeviceContext->DSSetShader(_backgroundProgram->getDomainShader(), NULL, 0);
	DeviceContext->GSSetShader(_backgroundProgram->getGeometryShader(), NULL, 0);
	DeviceContext->PSSetShader(_backgroundProgram->getPixelShader(), NULL, 0);

	// Texture
	ID3D11ShaderResourceView *pshr[] =
	{
		gbuf->getShaderResourceView(GBUF_MATERIAL),
		gbuf->getShaderResourceView(GBUF_LIGHT),
	};
	DeviceContext->PSSetShaderResources(0, ARRAYSIZE(pshr), pshr);

	// Vertex Buffer
	UINT stride = 2 * sizeof(FLOAT), offset = 0;
	ID3D11Buffer *drawBuf[] =
	{
		_vertexBuffer->getBuffer(),
	};
	DeviceContext->IASetVertexBuffers(0, ARRAYSIZE(drawBuf), drawBuf, &stride, &offset);
	DeviceContext->IASetInputLayout(_pInputLayout);
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	DeviceContext->Draw(4, 0);

	gbuf->clearLight();
}

void Engine::Screen::display(Renderer *renderer, GBuffer *gbuf, const FLOAT &r, const FLOAT &g, const FLOAT &b, const FLOAT &a)
{
	renderer->setState();

	// Shader
	DeviceContext->VSSetShader(_directProgram->getVertexShader(), NULL, 0);
	DeviceContext->HSSetShader(_directProgram->getHullShader(), NULL, 0);
	DeviceContext->DSSetShader(_directProgram->getDomainShader(), NULL, 0);
	DeviceContext->GSSetShader(_directProgram->getGeometryShader(), NULL, 0);
	DeviceContext->PSSetShader(_directProgram->getPixelShader(), NULL, 0);

	// Texture
	ID3D11ShaderResourceView *pshr[] =
	{
		gbuf->getShaderResourceView(GBUF_BACKGROUND),
	};
	DeviceContext->PSSetShaderResources(0, ARRAYSIZE(pshr), pshr);

	XMFLOAT4 color(r, g, b, a);
	_screenColorBuffer->updateStoreMap(&color);

	ID3D11Buffer *buf[] =
	{
		_screenColorBuffer->getBuffer(),
	};
	DeviceContext->PSSetConstantBuffers(0, ARRAYSIZE(buf), buf);

	// Vertex Buffer
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
