#include <Engine/FrameBuffer.hpp>

engine::FrameBuffer::FrameBuffer(void)
{
}

engine::FrameBuffer::~FrameBuffer(void)
{
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