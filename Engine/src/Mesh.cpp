#include <Engine/Mesh.hpp>
#include <Engine/Texture.hpp>
#include <Engine/Buffer.hpp>

Engine::Mesh::Mesh(const EngineDevice &EngineDevice)
	: _EngineDevice(EngineDevice), _numElement(0)
{
	_colorTexture = new Texture(_EngineDevice);
	_NMTexture = new Texture(_EngineDevice);
	_vertexBuffer = new Buffer(_EngineDevice);
	_indexBuffer = new Buffer(_EngineDevice);
	_materialBuffer = new Buffer(_EngineDevice);

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

void Engine::Mesh::load(const UINT &sizeVertexArray, const FLOAT *vertexArray,
	const UINT &sizeIndexArray, const UINT *indexArray)
{
	_numElement = sizeIndexArray / (UINT)sizeof(UINT);

	// Create vertex buffer
	_vertexBuffer->createStore(D3D11_BIND_VERTEX_BUFFER, vertexArray, sizeVertexArray, D3D11_USAGE_IMMUTABLE);

	// Create index buffer
	_indexBuffer->createStore(D3D11_BIND_INDEX_BUFFER, indexArray, sizeIndexArray, D3D11_USAGE_IMMUTABLE);
}

void Engine::Mesh::display(void) const
{
	ID3D11ShaderResourceView *pshr[] =
	{
		_colorTexture->getShaderResourceView(),
		_NMTexture->getShaderResourceView(),
	};
	_EngineDevice.DeviceContext->PSSetShaderResources(0, ARRAYSIZE(pshr), pshr);
	ID3D11SamplerState *psam[] =
	{
		_colorTexture->getSamplerState(),
		_NMTexture->getSamplerState(),
	};
	_EngineDevice.DeviceContext->PSSetSamplers(0, ARRAYSIZE(psam), psam);

	_materialBuffer->updateStoreMap(&_material);
	ID3D11Buffer *buf[] =
	{
		_materialBuffer->getBuffer(),
	};
	_EngineDevice.DeviceContext->PSSetConstantBuffers(0, ARRAYSIZE(buf), buf);

	UINT stride = 11 * sizeof(FLOAT), offset = 0;
	ID3D11Buffer *drawBuf[] =
	{
		_vertexBuffer->getBuffer(),
		_indexBuffer->getBuffer(),
	};
	_EngineDevice.DeviceContext->IASetVertexBuffers(0, 1, &drawBuf[0], &stride, &offset);
	_EngineDevice.DeviceContext->IASetIndexBuffer(drawBuf[1], DXGI_FORMAT_R32_UINT, offset);
	_EngineDevice.DeviceContext->DrawIndexed(_numElement, 0, 0);
}

void Engine::Mesh::displayShadow(void) const
{
	ID3D11ShaderResourceView *pshr[] =
	{
		_colorTexture->getShaderResourceView(),
	};
	_EngineDevice.DeviceContext->PSSetShaderResources(0, ARRAYSIZE(pshr), pshr);
	ID3D11SamplerState *psam[] =
	{
		_colorTexture->getSamplerState(),
	};
	_EngineDevice.DeviceContext->PSSetSamplers(0, ARRAYSIZE(psam), psam);

	UINT stride = 11 * sizeof(FLOAT), offset = 0;
	ID3D11Buffer *drawBuf[] =
	{
		_vertexBuffer->getBuffer(),
		_indexBuffer->getBuffer(),
	};
	_EngineDevice.DeviceContext->IASetVertexBuffers(0, 1, &drawBuf[0], &stride, &offset);
	_EngineDevice.DeviceContext->IASetIndexBuffer(drawBuf[1], DXGI_FORMAT_R32_UINT, offset);
	_EngineDevice.DeviceContext->DrawIndexed(_numElement, 0, 0);
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
