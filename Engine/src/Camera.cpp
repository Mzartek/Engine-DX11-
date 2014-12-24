#include <Engine/Camera.hpp>

Engine::Camera::Camera(void)
{
	_pcamera = (XMVECTOR *)_aligned_malloc(sizeof *_pcamera, 16);
	_ptarget = (XMVECTOR *)_aligned_malloc(sizeof *_ptarget, 16);
	_vview = (XMVECTOR *)_aligned_malloc(sizeof *_vview, 16);
	_projectionMatrix = (XMMATRIX *)_aligned_malloc(sizeof *_projectionMatrix, 16);
	_viewMatrix = (XMMATRIX *)_aligned_malloc(sizeof *_viewMatrix, 16);
	_VPMatrix = (XMMATRIX *)_aligned_malloc(sizeof *_VPMatrix, 16);
	_IVPMatrix = (XMMATRIX *)_aligned_malloc(sizeof *_IVPMatrix, 16);
}

Engine::Camera::~Camera(void)
{
	_aligned_free(_pcamera);
	_aligned_free(_ptarget);
	_aligned_free(_vview);
	_aligned_free(_projectionMatrix);
	_aligned_free(_viewMatrix);
	_aligned_free(_VPMatrix);
	_aligned_free(_IVPMatrix);
}

void Engine::Camera::setCameraPosition(const XMVECTOR &pos)
{
	*_pcamera = pos;
}

void Engine::Camera::setTargetPosition(const XMVECTOR &pos)
{
	*_ptarget = pos;
}

void Engine::Camera::setPerspective(const FLOAT &fov, const UINT &width, const UINT &height, const FLOAT &n, const FLOAT &f)
{
	*_projectionMatrix = XMMatrixPerspectiveFovRH(fov, (FLOAT)width / height, n, f);

	_fov = fov;
	_viewLen = f - n;
	_frusSphereRadian = XMVectorGetX(XMVector3Length(XMVectorSet(0.0f, 0.0f, n + _viewLen * 0.5f, 0.0f) - XMVectorSet((FLOAT)width, (FLOAT)height, _viewLen, 0.0f)));
}

XMVECTOR Engine::Camera::getCameraPosition(void) const
{
	return *_pcamera;
}

XMVECTOR Engine::Camera::getTargetPosition(void) const
{
	return *_ptarget;
}

XMVECTOR Engine::Camera::getViewVector(void) const
{
	return *_vview;
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

FLOAT Engine::Camera::getFOV(void) const
{
	return _fov;
}

FLOAT Engine::Camera::getFrusSphereRadian(void) const
{
	return _frusSphereRadian;
}

void Engine::Camera::position(void)
{
	*_vview = XMVector3Normalize(*_ptarget - *_pcamera);
	*_viewMatrix = XMMatrixLookAtRH(*_pcamera, *_ptarget, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	*_VPMatrix = *_viewMatrix * *_projectionMatrix;
	*_IVPMatrix = XMMatrixInverse(NULL, *_VPMatrix);
}
