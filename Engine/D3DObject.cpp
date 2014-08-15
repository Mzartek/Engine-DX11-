#include <Engine/D3DObject.hpp>

engine::D3DObject::D3DObject(void)
{
	UINT i;
	_pResource = NULL;
	_pShaderResourceView = NULL;
	_pSamplerState = NULL;
	_pMaterialBuffer = NULL;
	_pVertexBuffer = NULL;
	_pIndexBuffer = NULL;
	_pInputLayout = NULL;
	_material = (struct material *)_aligned_malloc(sizeof *_material, 16);
	_program = NULL;

	for (i = 0; i<4; i++)
	{
		_material->ambient[i] = 1.0;
		_material->diffuse[i] = 1.0;
		_material->specular[i] = 1.0;
	}
	_material->shininess[0] = 1.0;
}

engine::D3DObject::~D3DObject(void)
{
	_aligned_free(_material);

	if (_pInputLayout)
		_pInputLayout->Release();
	if (_pIndexBuffer)
		_pIndexBuffer->Release();
	if (_pVertexBuffer)
		_pVertexBuffer->Release();
	if (_pMaterialBuffer)
		_pMaterialBuffer->Release();
	if (_pSamplerState)
		_pSamplerState->Release();
	if (_pShaderResourceView)
		_pShaderResourceView->Release();
	if (_pResource)
		_pResource->Release();
}

HRESULT engine::D3DObject::config(ShaderProgram *program, ID3D11Device *pd3dDevice)
{
	HRESULT hr;

	_program = program;

	// Create uniform
	D3D11_BUFFER_DESC bd;
	bd.ByteWidth = sizeof(*_material) + (((sizeof(*_material) % 16) == 0) ? 0 : (16 - (sizeof(*_material) % 16)));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	hr = pd3dDevice->CreateBuffer(&bd, NULL, &_pMaterialBuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create Constant Buffer", "D3DObject", MB_OK);
		return hr;
	}

	// Create and set the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "IN_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "IN_TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "IN_NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 }
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

void engine::D3DObject::setTexture(ID3D11ShaderResourceView *pShaderResourceView, ID3D11SamplerState *pSamplerState)
{
	pShaderResourceView->GetResource(&_pResource);
	_pShaderResourceView = pShaderResourceView;
	_pSamplerState = pSamplerState;
}

void engine::D3DObject::setAmbient(const FLOAT &x, const FLOAT &y, const FLOAT &z, const FLOAT &w)
{
	_material->ambient[0] = x;
	_material->ambient[1] = y;
	_material->ambient[2] = z;
	_material->ambient[3] = w;
}

void engine::D3DObject::setDiffuse(const FLOAT &x, const FLOAT &y, const FLOAT &z, const FLOAT &w)
{
	_material->diffuse[0] = x;
	_material->diffuse[1] = y;
	_material->diffuse[2] = z;
	_material->diffuse[3] = w;
}

void engine::D3DObject::setSpecular(const FLOAT &x, const FLOAT &y, const FLOAT &z, const FLOAT &w)
{
	_material->specular[0] = x;
	_material->specular[1] = y;
	_material->specular[2] = z;
	_material->specular[3] = w;
}

void engine::D3DObject::setShininess(const FLOAT &x)
{
	_material->shininess[0] = x;
}

FLOAT engine::D3DObject::getTransparency(void)
{
	return _material->diffuse[3];
}

#define BUFFER_OFFSET(i) ((CHAR *)NULL + i)

HRESULT engine::D3DObject::load(const UINT &sizeVertexArray, const FLOAT *vertexArray, 
	const UINT &sizeIndexArray, const UINT *indexArray,
	ID3D11Device *pd3dDevice)
{
	HRESULT hr;
	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA data;

	_numElement = sizeIndexArray / (UINT)sizeof(UINT);

	// Create vertex buffer
	bd.ByteWidth = sizeVertexArray;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	data.pSysMem = vertexArray;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;
	hr = pd3dDevice->CreateBuffer(&bd, &data, &_pVertexBuffer);
	if (FAILED(hr))
		return hr;

	// Create index buffer
	bd.ByteWidth = sizeIndexArray;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	data.pSysMem = indexArray;
	hr = pd3dDevice->CreateBuffer(&bd, &data, &_pIndexBuffer);

	return hr;
}

#undef BUFFER_OFFSET

void engine::D3DObject::display(GBuffer *g) const
{
	if(_program == NULL)
	{
		MessageBox(NULL, "You need to configure before!", "D3DObject", MB_OK);
		exit(1);
	}

	// Shader
	g->getContext()->VSSetShader(_program->getVertexShader(), NULL, 0);
	g->getContext()->GSSetShader(_program->getGeometryShader(), NULL, 0);
	g->getContext()->PSSetShader(_program->getPixelShader(), NULL, 0);

	// Texture
	ID3D11ShaderResourceView *pshr[] =
	{
		_pShaderResourceView,
		g->getShaderResourceView(GBUF_NORMAL),
		g->getShaderResourceView(GBUF_MATERIAL),
		g->getShaderResourceView(GBUF_DEPTH),
	};
	g->getContext()->PSSetShaderResources(0, ARRAYSIZE(pshr), pshr);
	ID3D11SamplerState *psam[] =
	{
		_pSamplerState,
	};
	g->getContext()->PSSetSamplers(0, ARRAYSIZE(psam), psam);

	// Uniform
	g->getContext()->UpdateSubresource(_pMaterialBuffer, 0, NULL, _material, 0, 0);
	g->getContext()->PSSetConstantBuffers(0, 1, &_pMaterialBuffer);

	// Vertex And Index Buffer
	UINT stride = 8 * sizeof(FLOAT);
	UINT offset = 0;
	g->getContext()->IASetVertexBuffers(0, 1, &_pVertexBuffer, &stride, &offset);
	g->getContext()->IASetIndexBuffer(_pIndexBuffer, DXGI_FORMAT_R32_UINT, offset);

	// Input Layout
	g->getContext()->IASetInputLayout(_pInputLayout);

	// Topology
	g->getContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Draw
	g->getContext()->DrawIndexed(_numElement, 0, 0);
}

int engine::comparD3DObject(const void *p1, const void *p2)
{
	D3DObject **obj1 = (engine::D3DObject **)p1;
	D3DObject **obj2 = (engine::D3DObject **)p2;

	if ((*obj1)->_material->diffuse[3] < (*obj2)->_material->diffuse[3])
		return 1;
	if ((*obj1)->_material->diffuse[3] > (*obj2)->_material->diffuse[3])
		return -1;
	return 0;
}
