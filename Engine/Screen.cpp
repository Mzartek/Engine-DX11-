#include <Engine/Screen.hpp>

engine::Screen::Screen()
{
	_pScreenColorBuffer = NULL;
	_pInputLayout = NULL;
	_pVertexBuffer = NULL;
	_color = new XMFLOAT4;
	_program = NULL;
}

engine::Screen::~Screen()
{
	delete _color;
	if (_pVertexBuffer)
		_pVertexBuffer->Release();
	if (_pInputLayout)
		_pInputLayout->Release();
	if (_pScreenColorBuffer)
		_pScreenColorBuffer->Release();
}

#define BUFFER_OFFSET(i) ((GLbyte *)NULL + i)

void engine::Screen::config(ShaderProgram *program, ID3D11Device *pd3dDevice)
{
	HRESULT hr;
	
	_program = program;

	// Create Screen Color Buffer
	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA data;
	bd.ByteWidth = sizeof *_color;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	hr = pd3dDevice->CreateBuffer(&bd, NULL, &_pScreenColorBuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create Screen Buffer", "Screen", MB_OK);
		exit(1);
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
		MessageBox(NULL, "Failed to create Input Layout", "Screen", MB_OK);
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
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	data.pSysMem = vertex;
	hr = pd3dDevice->CreateBuffer(&bd, &data, &_pVertexBuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create Vertex Buffer", "Screen", MB_OK);
		exit(1);
	}
}

#undef BUFFER_OFFSET

void engine::Screen::display(Renderer *renderer, GBuffer *gbuf, const FLOAT &r, const FLOAT &g, const FLOAT &b, const FLOAT &a)
{
	if (_program == NULL)
	{
		MessageBox(NULL, "You need to configure before!", "Screen", MB_OK);
		exit(1);
	}

	// Shader
	renderer->getContext()->VSSetShader(_program->getVertexShader(), NULL, 0);
	renderer->getContext()->GSSetShader(_program->getGeometryShader(), NULL, 0);
	renderer->getContext()->PSSetShader(_program->getPixelShader(), NULL, 0);
	renderer->getContext()->IASetInputLayout(_pInputLayout);
	renderer->getContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// Texture
	ID3D11ShaderResourceView *pshr[] =
	{
		gbuf->getShaderResourceView(GBUF_MATERIAL),
	};
	renderer->getContext()->PSSetShaderResources(0, ARRAYSIZE(pshr), pshr);

	// Constant Buffer
	_color->x = r;
	_color->y = g;
	_color->z = b;
	_color->w = a;
	renderer->getContext()->UpdateSubresource(_pScreenColorBuffer, 0, NULL, _color, 0, 0);
	renderer->getContext()->PSSetConstantBuffers(0, 1, &_pScreenColorBuffer);

	// Vertex Buffer
	UINT stride = 2 * sizeof(FLOAT), offset = 0;
	renderer->getContext()->IASetVertexBuffers(0, 1, &_pVertexBuffer, &stride, &offset);

	// Draw
	renderer->getContext()->Draw(4, 0);
}
