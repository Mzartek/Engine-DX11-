#include <Engine/PerspCamera.hpp>

Engine::PerspCamera::PerspCamera(void)
{
	_pcamera = (XMVECTOR *)_aligned_malloc(sizeof *_pcamera, 16);
	_ptarget = (XMVECTOR *)_aligned_malloc(sizeof *_ptarget, 16);
	_vview = (XMVECTOR *)_aligned_malloc(sizeof *_vview, 16);
	_frusSpherePosition = (XMVECTOR *)_aligned_malloc(sizeof *_frusSpherePosition, 16);
}

Engine::PerspCamera::~PerspCamera(void)
{
	_aligned_free(_pcamera);
	_aligned_free(_ptarget);
	_aligned_free(_vview);
	_aligned_free(_frusSpherePosition);
}

void Engine::PerspCamera::setCameraPosition(const XMVECTOR &pos) const
{
	*_pcamera = pos;
}

void Engine::PerspCamera::setTargetPosition(const XMVECTOR &pos) const
{
	*_ptarget = pos;
}

void Engine::PerspCamera::setPerspective(const FLOAT &fov, const UINT &width, const UINT &height, const FLOAT &n, const FLOAT &f)
{
	FLOAT ratio = (FLOAT)width / height;
	FLOAT yfar = tanf(fov * 0.5f) * f;
	FLOAT xfar = yfar * ratio;

	*_projectionMatrix = XMMatrixPerspectiveFovRH(fov, ratio, n, f);

	_near = n;
	_far = f;
	_fov = fov * ratio;
	_frusSphereDistance = n + (f - n) * 0.5f;
	_frusSphereRadius = XMVectorGetX(XMVector3Length(XMVectorSet(xfar, yfar, f, 0.0f) - XMVectorSet(0.0f, 0.0f, _frusSphereDistance, 0.0f)));
}

XMVECTOR Engine::PerspCamera::getCameraPosition(void) const
{
	return *_pcamera;
}

XMVECTOR Engine::PerspCamera::getTargetPosition(void) const
{
	return *_ptarget;
}

XMVECTOR Engine::PerspCamera::getViewVector(void) const
{
	return *_vview;
}

FLOAT Engine::PerspCamera::getNear(void) const
{
	return _near;
}

FLOAT Engine::PerspCamera::getFar(void) const
{
	return _far;
}

FLOAT Engine::PerspCamera::getFOV(void) const
{
	return _fov;
}

FLOAT Engine::PerspCamera::getFrusSphereDistance(void) const
{
	return _frusSphereDistance;
}

FLOAT Engine::PerspCamera::getFrusSphereRadius(void) const
{
	return _frusSphereRadius;
}

XMVECTOR Engine::PerspCamera::getFrusSpherePosition(void) const
{
	return *_frusSpherePosition;
}

void Engine::PerspCamera::position(void) const
{
	*_vview = XMVector3Normalize(*_ptarget - *_pcamera);
	*_frusSpherePosition = *_vview * _frusSphereDistance;
	*_viewMatrix = XMMatrixLookAtRH(*_pcamera, *_ptarget, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	*_VPMatrix = *_viewMatrix * *_projectionMatrix;
	*_IVPMatrix = XMMatrixInverse(NULL, *_VPMatrix);
}