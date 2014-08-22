#include <Engine/FrameBuffer.hpp>

engine::FrameBuffer::FrameBuffer(void)
{
	// Device
	_pd3dDevice = NULL;
	// Context
	_pDefferedContext = NULL;
}

engine::FrameBuffer::~FrameBuffer(void)
{
	if (_pDefferedContext)
		_pDefferedContext->ClearState();
	if (_pDefferedContext)
		_pDefferedContext->Release();
}

UINT engine::FrameBuffer::getWidth(void) const
{
	return _width;
}

UINT engine::FrameBuffer::getHeight(void) const
{
	return _height;
}

ID3D11DeviceContext *engine::FrameBuffer::getContext(void) const
{
	return _pDefferedContext;
}