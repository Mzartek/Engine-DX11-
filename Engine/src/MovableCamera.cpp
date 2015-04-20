#include <Engine/MovableCamera.hpp>

Engine::MovableCamera::MovableCamera(void)
	: _speed(1), _atheta(0), _aphi(0)
{
}

Engine::MovableCamera::~MovableCamera(void)
{
}

void Engine::MovableCamera::setSpeed(const FLOAT &v)
{
	_speed = v;
}

void Engine::MovableCamera::mouseMove(const INT &xrel, const INT &yrel)
{
	_atheta -= (FLOAT)xrel * XM_PI / 180;
	_aphi -= (FLOAT)yrel * XM_PI / 180;

	if (_aphi >= XM_PI / 2) _aphi = 89.0f * XM_PI / 180;
	else if (_aphi <= -XM_PI / 2) _aphi = -89.0f * XM_PI / 180;

	this->setTargetPosition(_atheta, _aphi);
}