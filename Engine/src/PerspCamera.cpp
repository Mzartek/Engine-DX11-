#include <Engine/PerspCamera.hpp>

Engine::PerspCamera::PerspCamera(void)
{
	_pcamera = (XMVECTOR *)_aligned_malloc(sizeof *_pcamera, 16);
	_vforward = (XMVECTOR *)_aligned_malloc(sizeof *_vforward, 16);
	_vleft = (XMVECTOR *)_aligned_malloc(sizeof *_vleft, 16);
	_vup = (XMVECTOR *)_aligned_malloc(sizeof *_vup, 16);

	_frusSpherePosition = (XMVECTOR *)_aligned_malloc(sizeof *_frusSpherePosition, 16);
}

Engine::PerspCamera::~PerspCamera(void)
{
	_aligned_free(_pcamera);
	_aligned_free(_vforward);
	_aligned_free(_vleft);
	_aligned_free(_vup);

	_aligned_free(_frusSpherePosition);
}

void Engine::PerspCamera::setCameraPosition(const XMVECTOR &pos) const
{
	*_pcamera = pos;
}

void Engine::PerspCamera::setTargetPosition(const FLOAT &atheta, const FLOAT &aphi) const
{
	FLOAT tmp = cosf(aphi);
	*_vforward = XMVectorSet(tmp * sinf(atheta), sinf(aphi), tmp * cosf(atheta), 0.0f);
	*_vleft = XMVector3Normalize(XMVectorSet(XMVectorGetZ(*_vforward), 0.0f, -XMVectorGetX(*_vforward), 0.0f));
	*_vup = XMVector3Cross(*_vforward, *_vleft);
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

XMVECTOR Engine::PerspCamera::getForwardVector(void) const
{
	return *_vforward;
}

XMVECTOR Engine::PerspCamera::getLeftVector(void) const
{
	return *_vleft;
}

XMVECTOR Engine::PerspCamera::getUpVector(void) const
{
	return *_vup;
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
	*_frusSpherePosition = *_vforward * _frusSphereDistance;

	*_viewMatrix = XMMatrixLookAtRH(*_pcamera, *_pcamera + * _vforward, *_vup);
	*_VPMatrix = *_viewMatrix * *_projectionMatrix;
	*_IVPMatrix = XMMatrixInverse(NULL, *_VPMatrix);
}