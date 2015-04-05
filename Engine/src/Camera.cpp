#include <Engine/Camera.hpp>

Engine::Camera::Camera(void)
{
	_projectionMatrix = (XMMATRIX *)_aligned_malloc(sizeof *_projectionMatrix, 16);
	_viewMatrix = (XMMATRIX *)_aligned_malloc(sizeof *_viewMatrix, 16);
	_VPMatrix = (XMMATRIX *)_aligned_malloc(sizeof *_VPMatrix, 16);
	_IVPMatrix = (XMMATRIX *)_aligned_malloc(sizeof *_IVPMatrix, 16);
}

Engine::Camera::~Camera(void)
{
	_aligned_free(_projectionMatrix);
	_aligned_free(_viewMatrix);
	_aligned_free(_VPMatrix);
	_aligned_free(_IVPMatrix);
}

XMMATRIX Engine::Camera::getProjectionMatrix(void) const
{
	return *_projectionMatrix;
}

XMMATRIX Engine::Camera::getViewMatrix(void) const
{
	return *_viewMatrix;
}

XMMATRIX Engine::Camera::getVPMatrix(void) const
{
	return *_VPMatrix;
}

XMMATRIX Engine::Camera::getIVPMatrix(void) const
{
	return *_IVPMatrix;
}
