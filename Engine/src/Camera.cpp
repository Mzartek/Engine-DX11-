#include <Engine/Camera.hpp>

Engine::Camera::Camera(void)
{
	_pcamera = (XMVECTOR *)_aligned_malloc(sizeof *_pcamera, 16);
	_ptarget = (XMVECTOR *)_aligned_malloc(sizeof *_ptarget, 16);
	_projectionMatrix = (XMMATRIX *)_aligned_malloc(sizeof *_projectionMatrix, 16);
	_viewMatrix = (XMMATRIX *)_aligned_malloc(sizeof *_viewMatrix, 16);
	_VPMatrix = (XMMATRIX *)_aligned_malloc(sizeof *_VPMatrix, 16);
}

Engine::Camera::~Camera(void)
{
	_aligned_free(_pcamera);
	_aligned_free(_ptarget);
	_aligned_free(_projectionMatrix);
	_aligned_free(_viewMatrix);
	_aligned_free(_VPMatrix);
}

void Engine::Camera::setPositionCamera(const XMVECTOR &pos)
{
	*_pcamera = pos;
}

void Engine::Camera::setPositionTarget(const XMVECTOR &pos)
{
	*_ptarget = pos;
}

void Engine::Camera::setPerspective(const FLOAT &fov, const UINT &width, const UINT &height, const FLOAT &n, const FLOAT &f)
{
	*_projectionMatrix = XMMatrixPerspectiveFovRH(fov, (FLOAT)width / height, n, f);
}

XMVECTOR Engine::Camera::getPositionCamera(void) const
{
	return *_pcamera;
}

XMVECTOR Engine::Camera::getPositionTarget(void) const
{
	return *_ptarget;
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

void Engine::Camera::position(void)
{
	*_viewMatrix = XMMatrixLookAtRH(*_pcamera, *_ptarget, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	*_VPMatrix = *_viewMatrix * *_projectionMatrix;
}
