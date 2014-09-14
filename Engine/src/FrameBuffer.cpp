#include <Engine/FrameBuffer.hpp>

engine::FrameBuffer::FrameBuffer(void)
{
	_pVP = new D3D11_VIEWPORT;
	_pd3dDevice = NULL;
	_pContext = NULL;
}

engine::FrameBuffer::~FrameBuffer(void)
{
	delete _pVP;
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
	return _pContext;
}