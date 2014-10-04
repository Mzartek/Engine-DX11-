#include <Engine/FrameBuffer.hpp>

Engine::FrameBuffer::FrameBuffer(const EngineDevice &EngineDevice)
{
	_EngineDevice = EngineDevice;
}

Engine::FrameBuffer::~FrameBuffer(void)
{
}

void Engine::FrameBuffer::config(const UINT &width, const UINT &height)
{
	_width = width;
	_height = height;
}

UINT Engine::FrameBuffer::getWidth(void) const
{
	return _width;
}

UINT Engine::FrameBuffer::getHeight(void) const
{
	return _height;
}