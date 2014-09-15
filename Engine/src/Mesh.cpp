#include <Engine/Mesh.hpp>

engine::Mesh::Mesh(ID3D11Device *pd3dDevice)
	: _pColorTex(NULL), _pNMTex(NULL), 
	_pColorTexSHR(NULL), _pNMTexSHR(NULL),
	_pTexSamplerState(NULL),
	_pVertexBuffer(NULL), _pIndexBuffer(NULL)
{
	D3D11_BUFFER_DESC bd;
	bd.ByteWidth = sizeof _material;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	pd3dDevice->CreateBuffer(&bd, NULL, &_pMaterialBuffer);
}

engine::Mesh::~Mesh(void)
{
	if (_pColorTex) _pColorTex->Release();
	if (_pNMTex) _pNMTex->Release();
	if (_pColorTexSHR) _pColorTexSHR->Release();
	if (_pNMTexSHR) _pNMTexSHR->Release();
	if (_pTexSamplerState) _pTexSamplerState->Release();
	if (_pVertexBuffer) _pVertexBuffer->Release();
	if (_pIndexBuffer) _pIndexBuffer->Release();
	_pMaterialBuffer->Release();
}

void engine::Mesh::setColorTexture(ID3D11Texture2D *ptex, ID3D11ShaderResourceView *pShaderResourceView, ID3D11SamplerState *pSamplerState)
{
	if (_pColorTex) _pColorTex->Release();
	if (_pColorTexSHR) _pColorTexSHR->Release();
	if (_pTexSamplerState) _pTexSamplerState->Release();
	
	_pColorTex = ptex;
	_pColorTexSHR = pShaderResourceView;
	_pTexSamplerState = pSamplerState;
}

void engine::Mesh::setNMTexture(ID3D11Texture2D *ptex, ID3D11ShaderResourceView *pShaderResourceView)
{
	if (_pNMTex) _pNMTex->Release();
	if (_pNMTexSHR) _pNMTexSHR->Release();

	_pNMTex = ptex;
	_pNMTexSHR = pShaderResourceView;
}

void engine::Mesh::setAmbient(const XMFLOAT4 &ambient, ID3D11DeviceContext *pContext)
{
	_material.ambient = ambient;
	pContext->UpdateSubresource(_pMaterialBuffer, 0, NULL, &_material, 0, 0);
}

void engine::Mesh::setDiffuse(const XMFLOAT4 &diffuse, ID3D11DeviceContext *pContext)
{
	_material.diffuse = diffuse;
	pContext->UpdateSubresource(_pMaterialBuffer, 0, NULL, &_material, 0, 0);
}

void engine::Mesh::setSpecular(const XMFLOAT4 &specular, ID3D11DeviceContext *pContext)
{
	_material.specular = specular;
	pContext->UpdateSubresource(_pMaterialBuffer, 0, NULL, &_material, 0, 0);
}

void engine::Mesh::setShininess(const FLOAT &shininess, ID3D11DeviceContext *pContext)
{
	_material.shininess = shininess;
	pContext->UpdateSubresource(_pMaterialBuffer, 0, NULL, &_material, 0, 0);
}

FLOAT engine::Mesh::getTransparency(void)
{
	return _material.diffuse.w;
}

void engine::Mesh::load(const UINT &sizeVertexArray, const FLOAT *vertexArray,
	const UINT &sizeIndexArray, const UINT *indexArray,
	ID3D11Device *pd3dDevice)
{
	HRESULT hr;
	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA data;

	if (_pVertexBuffer) _pVertexBuffer->Release();
	if (_pIndexBuffer) _pIndexBuffer->Release();

	_numElement = sizeIndexArray / (UINT)sizeof(UINT);

	// Create vertex buffer
	bd.ByteWidth = sizeVertexArray;
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	bd.CPUAccessFlags = 0;
	data.pSysMem = vertexArray;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;
	hr = pd3dDevice->CreateBuffer(&bd, &data, &_pVertexBuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create Vertex Buffer", "Mesh", MB_OK);
		exit(1);
	}

	// Create index buffer
	bd.ByteWidth = sizeIndexArray;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	data.pSysMem = indexArray;
	hr = pd3dDevice->CreateBuffer(&bd, &data, &_pIndexBuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create Index Buffer", "Mesh", MB_OK);
		exit(1);
	}
}

void engine::Mesh::display(ID3D11DeviceContext *pContext) const
{
	pContext->PSSetShaderResources(0, 1, &_pColorTexSHR);
	pContext->PSSetShaderResources(1, 1, &_pNMTexSHR);
	pContext->PSSetSamplers(0, 1, &_pTexSamplerState);

	pContext->PSSetConstantBuffers(0, 1, &_pMaterialBuffer);

	UINT stride = 11 * sizeof(FLOAT), offset = 0;
	pContext->IASetVertexBuffers(0, 1, &_pVertexBuffer, &stride, &offset);
	pContext->IASetIndexBuffer(_pIndexBuffer, DXGI_FORMAT_R32_UINT, offset);
	pContext->DrawIndexed(_numElement, 0, 0);
}

void engine::Mesh::displayShadow(ID3D11DeviceContext *pContext) const
{
	pContext->PSSetShaderResources(0, 1, &_pColorTexSHR);
	pContext->PSSetSamplers(0, 1, &_pTexSamplerState);

	UINT stride = 11 * sizeof(FLOAT), offset = 0;
	pContext->IASetVertexBuffers(0, 1, &_pVertexBuffer, &stride, &offset);
	pContext->IASetIndexBuffer(_pIndexBuffer, DXGI_FORMAT_R32_UINT, offset);
	pContext->DrawIndexed(_numElement, 0, 0);
}

int engine::comparMesh(const void *p1, const void *p2)
{
	Mesh **obj1 = (engine::Mesh **)p1;
	Mesh **obj2 = (engine::Mesh **)p2;

	if ((*obj1)->_material.diffuse.z < (*obj2)->_material.diffuse.z)
		return 1;
	if ((*obj1)->_material.diffuse.z > (*obj2)->_material.diffuse.z)
		return -1;
	return 0;
}
