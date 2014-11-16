#include <Engine/MovableCamera.hpp>

Engine::MovableCamera::MovableCamera(void)
	: _speed(1), _atheta(0), _aphi(0)
{
	_vforward = (XMVECTOR *)_aligned_malloc(sizeof *_vforward, 16);
	_vleft = (XMVECTOR *)_aligned_malloc(sizeof *_vleft, 16);
}

Engine::MovableCamera::~MovableCamera(void)
{
	_aligned_free(_vforward);
	_aligned_free(_vleft);
}

void Engine::MovableCamera::setInitialAngle(const FLOAT &t, const FLOAT &p)
{
	_atheta = t;
	_aphi = p;

	if (_aphi >= XM_PI / 2) _aphi = 89.0f * XM_PI / 180;
	else if (_aphi <= -XM_PI / 2) _aphi = -89.0f * XM_PI / 180;

	FLOAT tmp = cos(_aphi);
	*_vforward = XMVectorSetX(*_vforward, tmp * sin(_atheta));
	*_vforward = XMVectorSetY(*_vforward, sin(_aphi));
	*_vforward = XMVectorSetZ(*_vforward, tmp * cos(_atheta));

	*_vleft = XMVectorSetX(*_vleft, 1 * XMVectorGetZ(*_vforward));
	*_vleft = XMVectorSetY(*_vleft, 0);
	*_vleft = XMVectorSetZ(*_vleft, -(1 * XMVectorGetX(*_vforward)));
	*_vleft = XMVector3Normalize(*_vleft);

	*_ptarget = *_pcamera + *_vforward;
}

void Engine::MovableCamera::setSpeed(const FLOAT &v)
{
	_speed = v;
}

XMVECTOR Engine::MovableCamera::getVectorForward(void) const
{
	return *_vforward;
}

XMVECTOR Engine::MovableCamera::getVectorLeft(void) const
{
	return *_vleft;
}

void Engine::MovableCamera::mouseMove(const INT &xrel, const INT &yrel)
{
	_atheta -= (FLOAT)xrel * XM_PI / 180;
	_aphi -= (FLOAT)yrel * XM_PI / 180;

	if (_aphi >= XM_PI / 2) _aphi = 89.0f * XM_PI / 180;
	else if (_aphi <= -XM_PI / 2) _aphi = -89.0f * XM_PI / 180;

	FLOAT tmp = cos(_aphi);
	*_vforward = XMVectorSetX(*_vforward, tmp * sin(_atheta));
	*_vforward = XMVectorSetY(*_vforward, sin(_aphi));
	*_vforward = XMVectorSetZ(*_vforward, tmp * cos(_atheta));

	*_vleft = XMVectorSetX(*_vleft, 1 * XMVectorGetZ(*_vforward));
	*_vleft = XMVectorSetY(*_vleft, 0);
	*_vleft = XMVectorSetZ(*_vleft, -(1 * XMVectorGetX(*_vforward)));
	*_vleft = XMVector3Normalize(*_vleft);

	*_ptarget = *_pcamera + *_vforward;
}