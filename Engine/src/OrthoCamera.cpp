#include <Engine/OrthoCamera.hpp>

Engine::OrthoCamera::OrthoCamera(void)
{
	_position = (XMVECTOR *)_aligned_malloc(sizeof *_position, 16);
	_direction = (XMVECTOR *)_aligned_malloc(sizeof *_direction, 16);
}

Engine::OrthoCamera::~OrthoCamera(void)
{
	_aligned_free(_position);
	_aligned_free(_direction);
}

void Engine::OrthoCamera::setPosition(const XMVECTOR &pos) const
{
	*_position = pos;
}

void Engine::OrthoCamera::setDirection(const XMVECTOR &dir) const
{
	*_direction = dir;
}

void Engine::OrthoCamera::setOrthogonal(const FLOAT &left, const FLOAT &right, const FLOAT &bottom, const FLOAT &top, const FLOAT &zNear, const FLOAT &zFar)
{
	*_projectionMatrix = XMMatrixOrthographicOffCenterRH(left, right, bottom, top, zNear, zFar);

	_left = left;
	_right = right;
	_bottom = bottom;
	_top = top;
	_zNear = zNear;
	_zFar = zFar;
}

void Engine::OrthoCamera::position(void) const
{
	*_viewMatrix = XMMatrixLookAtRH(*_position - *_direction, *_position, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	*_VPMatrix = *_viewMatrix * *_projectionMatrix;
	*_IVPMatrix = XMMatrixInverse(NULL, *_VPMatrix);
}