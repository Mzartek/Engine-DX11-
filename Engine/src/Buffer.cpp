#include "Engine/Buffer.hpp"

Engine::Buffer::Buffer(const EngineDevice &EngineDevice)
	: _target(D3D11_BIND_VERTEX_BUFFER), _pBuffer(NULL), _size(0)
{
	_EngineDevice = EngineDevice;
}

Engine::Buffer::~Buffer(void)
{
	if (_pBuffer) _pBuffer->Release();
}

ID3D11Buffer *Engine::Buffer::getBuffer(void)
{
	return _pBuffer;
}

void Engine::Buffer::createStore(const D3D11_BIND_FLAG &target, const void *data, const UINT &size, const D3D11_USAGE &usage)
{
	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA dt;

	_target = target;
	_size = size;

	if (_pBuffer) _pBuffer->Release();

	bd.ByteWidth = _size;
	bd.Usage = usage;
	bd.BindFlags = _target;
	switch (usage)
	{
	case D3D11_USAGE_DYNAMIC:
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		break;
	default:
		bd.CPUAccessFlags = 0;
		break;
	}
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	if (data != NULL)
	{
		dt.pSysMem = data;
		dt.SysMemPitch = 0;
		dt.SysMemSlicePitch = 0;
		_EngineDevice.Device->CreateBuffer(&bd, &dt, &_pBuffer);
	}
	else
		_EngineDevice.Device->CreateBuffer(&bd, NULL, &_pBuffer);
}

void Engine::Buffer::updateStoreSub(const void *data)
{
	_EngineDevice.DeviceContext->UpdateSubresource(_pBuffer, 0, NULL, data, 0, 0);
}

void Engine::Buffer::updateStoreMap(const void *data)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	ZeroMemory(&mappedResource, sizeof mappedResource);
	_EngineDevice.DeviceContext->Map(_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	memcpy(mappedResource.pData, data, _size);
	_EngineDevice.DeviceContext->Unmap(_pBuffer, 0);
}