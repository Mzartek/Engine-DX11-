#include <Engine/D3DObject.hpp>

engine::D3DObject::D3DObject(ID3D11Device *pd3dDevice)
{
	_pTex = NULL;
	_pShaderResourceView = NULL;
	_pSamplerState = NULL;
	_pVertexBuffer = NULL;
	_pIndexBuffer = NULL;

	D3D11_BUFFER_DESC bd;
	bd.ByteWidth = sizeof _material;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	pd3dDevice->CreateBuffer(&bd, NULL, &_pMaterialBuffer);
}

engine::D3DObject::~D3DObject(void)
{
	_pMaterialBuffer->Release();

	if (_pIndexBuffer)
		_pIndexBuffer->Release();
	if (_pVertexBuffer)
		_pVertexBuffer->Release();
	if (_pSamplerState)
		_pSamplerState->Release();
	if (_pShaderResourceView)
		_pShaderResourceView->Release();
	if (_pTex)
		_pTex->Release();
}

void engine::D3DObject::setTexture(ID3D11Texture2D *ptex, ID3D11ShaderResourceView *pShaderResourceView, ID3D11SamplerState *pSamplerState)
{
	_pTex = ptex;
	_pShaderResourceView = pShaderResourceView;
	_pSamplerState = pSamplerState;
}

void engine::D3DObject::setAmbient(const XMFLOAT4 &ambient, ID3D11DeviceContext *pContext)
{
	_material.ambient = ambient;
	pContext->UpdateSubresource(_pMaterialBuffer, 0, NULL, &_material, 0, 0);
}

void engine::D3DObject::setDiffuse(const XMFLOAT4 &diffuse, ID3D11DeviceContext *pContext)
{
	_material.diffuse = diffuse;
	pContext->UpdateSubresource(_pMaterialBuffer, 0, NULL, &_material, 0, 0);
}

void engine::D3DObject::setSpecular(const XMFLOAT4 &specular, ID3D11DeviceContext *pContext)
{
	_material.specular = specular;
	pContext->UpdateSubresource(_pMaterialBuffer, 0, NULL, &_material, 0, 0);
}

void engine::D3DObject::setShininess(const FLOAT &shininess, ID3D11DeviceContext *pContext)
{
	_material.shininess = shininess;
	pContext->UpdateSubresource(_pMaterialBuffer, 0, NULL, &_material, 0, 0);
}

FLOAT engine::D3DObject::getTransparency(void)
{
	return _material.diffuse.w;
}

#define BUFFER_OFFSET(i) ((CHAR *)NULL + i)

void engine::D3DObject::load(const UINT &sizeVertexArray, const FLOAT *vertexArray, 
	const UINT &sizeIndexArray, const UINT *indexArray,
	ID3D11Device *pd3dDevice)
{
	HRESULT hr;
	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA data;

	_numElement = sizeIndexArray / (UINT)sizeof(UINT);

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	// Create vertex buffer
	bd.ByteWidth = sizeVertexArray;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	data.pSysMem = vertexArray;
	hr = pd3dDevice->CreateBuffer(&bd, &data, &_pVertexBuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create Vertex Buffer", "D3DObject", MB_OK);
		exit(1);
	}

	// Create index buffer
	bd.ByteWidth = sizeIndexArray;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	data.pSysMem = indexArray;
	hr = pd3dDevice->CreateBuffer(&bd, &data, &_pIndexBuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create Index Buffer", "D3DObject", MB_OK);
		exit(1);
	}
}

#undef BUFFER_OFFSET

void engine::D3DObject::display(ID3D11DeviceContext *pContext) const
{
	pContext->PSSetShaderResources(0, 1, &_pShaderResourceView);
	pContext->PSSetSamplers(0, 1, &_pSamplerState);

	pContext->PSSetConstantBuffers(0, 1, &_pMaterialBuffer);

	UINT stride = 8 * sizeof(FLOAT), offset = 0;
	pContext->IASetVertexBuffers(0, 1, &_pVertexBuffer, &stride, &offset);
	pContext->IASetIndexBuffer(_pIndexBuffer, DXGI_FORMAT_R32_UINT, offset);
	pContext->DrawIndexed(_numElement, 0, 0);
}

int engine::comparD3DObject(const void *p1, const void *p2)
{
	D3DObject **obj1 = (engine::D3DObject **)p1;
	D3DObject **obj2 = (engine::D3DObject **)p2;

	if ((*obj1)->_material.diffuse.z < (*obj2)->_material.diffuse.z)
		return 1;
	if ((*obj1)->_material.diffuse.z > (*obj2)->_material.diffuse.z)
		return -1;
	return 0;
}
