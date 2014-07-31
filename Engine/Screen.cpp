#include <Engine/Screen.hpp>

engine::Screen::Screen()
{
	_color = (struct screenColor *)_aligned_malloc(sizeof *_color, 16);
	_program = NULL;
}

engine::Screen::~Screen()
{
	_aligned_free(_color);

	if (_pInputLayout)
		_pInputLayout->Release();
	if (_pVertexBuffer)
		_pVertexBuffer->Release();
	if (_pScreenColorBuffer)
		_pScreenColorBuffer->Release();
}

#define BUFFER_OFFSET(i) ((GLbyte *)NULL + i)

HRESULT engine::Screen::config(ShaderProgram *program, ID3D11Device *pd3dDevice)
{
	HRESULT hr;
	
	_program = program;

	// Create Constant Buffer
	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA data;
	bd.ByteWidth = sizeof(*_color) + (((sizeof(*_color) % 16) == 0) ? 0 : (16 - (sizeof(*_color) % 16)));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	hr = pd3dDevice->CreateBuffer(&bd, NULL, &_pScreenColorBuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create Constant Buffer 0", "Screen", MB_OK);
		return hr;
	}

	// Create Vertex Buffer
	FLOAT vertex[] = {
		-1, -1,
		1, -1,
		-1, 1,
		1, 1,
	};

	bd.ByteWidth = ARRAYSIZE(vertex) * sizeof(FLOAT);
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	data.pSysMem = vertex;
	hr = pd3dDevice->CreateBuffer(&bd, &data, &_pVertexBuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create Vertex Buffer", "Screen", MB_OK);
		return hr;
	}

	// Create Input Layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "IN_POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	hr = pd3dDevice->CreateInputLayout(layout, ARRAYSIZE(layout),
		_program->getEntryBufferPointer(), _program->getEntryBytecodeLength(),
		&_pInputLayout);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create Input Layout", "D3DObject", MB_OK);
		return hr;
	}

	return S_OK;
}

#undef BUFFER_OFFSET

void engine::Screen::display(Window *win, GBuffer *gbuf, const FLOAT &r, const FLOAT &g, const FLOAT &b, const FLOAT &a)
{
	if (_program == NULL)
	{
		MessageBox(NULL, "You need to configure before!", "SCreen", MB_OK);
		exit(1);
	}

	// Shader
	win->getImmediateContext()->VSSetShader(_program->getVertexShader(), NULL, 0);
	win->getImmediateContext()->GSSetShader(_program->getGeometryShader(), NULL, 0);
	win->getImmediateContext()->PSSetShader(_program->getPixelShader(), NULL, 0);

	// Texture
	ID3D11ShaderResourceView *pshr[] =
	{
		gbuf->getShaderResourceView(GBUF_MATERIAL),
	};
	win->getImmediateContext()->PSSetShaderResources(0, ARRAYSIZE(pshr), pshr);

	// Constant Buffer
	_color->color[0] = r;
	_color->color[1] = g;
	_color->color[2] = b;
	_color->color[3] = a;
	win->getImmediateContext()->UpdateSubresource(_pScreenColorBuffer, 0, NULL, _color, 0, 0);
	ID3D11Buffer *buf[] =
	{
		_pScreenColorBuffer,
	};
	win->getImmediateContext()->PSSetConstantBuffers(0, ARRAYSIZE(buf), buf);

	// Vertex Buffer
	UINT stride = 2 * sizeof(FLOAT);
	UINT offset = 0;
	win->getImmediateContext()->IASetVertexBuffers(0, 1, &_pVertexBuffer, &stride, &offset);

	// Input Layout
	win->getImmediateContext()->IASetInputLayout(_pInputLayout);

	// Topology
	win->getImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// Draw
	win->getImmediateContext()->Draw(4, 0);
}
