#include <Engine/Mesh.hpp>
#include <Engine/Texture.hpp>
#include <Engine/Buffer.hpp>

Engine::Mesh::Mesh(void)
	: _numElement(0)
{
	_colorTexture = new Texture;
	_NMTexture = new Texture;
	_vertexBuffer = new Buffer;
	_indexBuffer = new Buffer;
	_materialBuffer = new Buffer;

	_materialBuffer->createStore(D3D11_BIND_CONSTANT_BUFFER, NULL, sizeof _material, D3D11_USAGE_DYNAMIC);
}

Engine::Mesh::~Mesh(void)
{
	delete _colorTexture;
	delete _NMTexture;
	delete _vertexBuffer;
	delete _indexBuffer;
	delete _materialBuffer;
}

void Engine::Mesh::setColorTexture(const CHAR *path)
{
	_colorTexture->load2DTextureFromFile(path);
}

void Engine::Mesh::setNMTexture(const CHAR *path)
{
	_NMTexture->load2DTextureFromFile(path);
}

void Engine::Mesh::setAmbient(const XMVECTOR &ambient)
{
	XMStoreFloat4(&_material.ambient, ambient);
}

void Engine::Mesh::setDiffuse(const XMVECTOR &diffuse)
{
	XMStoreFloat4(&_material.diffuse, diffuse);
}

void Engine::Mesh::setSpecular(const XMVECTOR &specular)
{
	XMStoreFloat4(&_material.specular, specular);
}

void Engine::Mesh::setShininess(const FLOAT &shininess)
{
	_material.shininess = shininess;
}

FLOAT Engine::Mesh::getTransparency(void)
{
	return _material.diffuse.w;
}

void Engine::Mesh::load(const UINT &numVertex, const Vertex *vertexArray,
	const UINT &numIndex, const UINT *indexArray)
{
	_numElement = numIndex;

	// Create vertex buffer
	_vertexBuffer->createStore(D3D11_BIND_VERTEX_BUFFER, vertexArray, numVertex * sizeof *vertexArray, D3D11_USAGE_IMMUTABLE);

	// Create index buffer
	_indexBuffer->createStore(D3D11_BIND_INDEX_BUFFER, indexArray, numIndex * sizeof *indexArray, D3D11_USAGE_IMMUTABLE);
}

void Engine::Mesh::display(void) const
{
	ID3D11ShaderResourceView *pshr[] =
	{
		_colorTexture->getShaderResourceView(),
		_NMTexture->getShaderResourceView(),
	};
	DeviceContext->PSSetShaderResources(0, ARRAYSIZE(pshr), pshr);
	ID3D11SamplerState *psam[] =
	{
		_colorTexture->getSamplerState(),
		_NMTexture->getSamplerState(),
	};
	DeviceContext->PSSetSamplers(0, ARRAYSIZE(psam), psam);

	_materialBuffer->updateStoreMap(&_material);
	ID3D11Buffer *buf[] =
	{
		_materialBuffer->getBuffer(),
	};
	DeviceContext->PSSetConstantBuffers(0, ARRAYSIZE(buf), buf);

	UINT stride = 11 * sizeof(FLOAT), offset = 0;
	ID3D11Buffer *drawBuf[] =
	{
		_vertexBuffer->getBuffer(),
		_indexBuffer->getBuffer(),
	};
	DeviceContext->IASetVertexBuffers(0, 1, &drawBuf[0], &stride, &offset);
	DeviceContext->IASetIndexBuffer(drawBuf[1], DXGI_FORMAT_R32_UINT, offset);
	DeviceContext->DrawIndexed(_numElement, 0, 0);
}

void Engine::Mesh::displayShadow(void) const
{
	ID3D11ShaderResourceView *pshr[] =
	{
		_colorTexture->getShaderResourceView(),
	};
	DeviceContext->PSSetShaderResources(0, ARRAYSIZE(pshr), pshr);
	ID3D11SamplerState *psam[] =
	{
		_colorTexture->getSamplerState(),
	};
	DeviceContext->PSSetSamplers(0, ARRAYSIZE(psam), psam);

	UINT stride = 11 * sizeof(FLOAT), offset = 0;
	ID3D11Buffer *drawBuf[] =
	{
		_vertexBuffer->getBuffer(),
		_indexBuffer->getBuffer(),
	};
	DeviceContext->IASetVertexBuffers(0, 1, &drawBuf[0], &stride, &offset);
	DeviceContext->IASetIndexBuffer(drawBuf[1], DXGI_FORMAT_R32_UINT, offset);
	DeviceContext->DrawIndexed(_numElement, 0, 0);
}

int Engine::comparMesh(const void *p1, const void *p2)
{
	Mesh **obj1 = (Engine::Mesh **)p1;
	Mesh **obj2 = (Engine::Mesh **)p2;

	if ((*obj1)->_material.diffuse.z < (*obj2)->_material.diffuse.z)
		return 1;
	if ((*obj1)->_material.diffuse.z > (*obj2)->_material.diffuse.z)
		return -1;
	return 0;
}
