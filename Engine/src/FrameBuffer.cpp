#include <Engine/FrameBuffer.hpp>

Engine::FrameBuffer::FrameBuffer(void)
{
}

Engine::FrameBuffer::~FrameBuffer(void)
{
}

UINT Engine::FrameBuffer::getWidth(void) const
{
	return _width;
}

UINT Engine::FrameBuffer::getHeight(void) const
{
	return _height;
}