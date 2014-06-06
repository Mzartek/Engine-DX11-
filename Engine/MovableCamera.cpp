#include <Engine/MovableCamera.hpp>

engine::MovableCamera::MovableCamera(void)
{
	_atheta = _aphi = 0;
	_vforward = new DirectX::XMFLOAT3;
	_vleft = new DirectX::XMFLOAT3;
	_speed = 1;
}

engine::MovableCamera::~MovableCamera(void)
{
	delete _vforward;
	delete _vleft;
}

void engine::MovableCamera::setSpeed(const FLOAT &v)
{
	_speed = v;
}

void engine::MovableCamera::mouseMove(const INT &xrel, const INT &yrel)
{
	_atheta -= (FLOAT)xrel;
	_aphi -= (FLOAT)yrel;

	if(_atheta > 360)
		_atheta -= 360;
	else if(_atheta < -360)
		_atheta += 360;
	if (_aphi > 89)
		_aphi = 89;
	else if (_aphi < -89)
		_aphi = -89;
  
	FLOAT tmp = (FLOAT)cos(_aphi*DirectX::XM_PI/180);
	_vforward->y = (FLOAT)sin(_aphi*DirectX::XM_PI / 180);
	_vforward->z = tmp*(FLOAT)cos(_atheta*DirectX::XM_PI / 180);
	_vforward->x = tmp*(FLOAT)sin(_atheta*DirectX::XM_PI / 180);

	_vleft->x = 1*_vforward->z;
	_vleft->z = -(1*_vforward->x);

	tmp = (FLOAT)sqrt(_vleft->x*_vleft->x + _vleft->y*_vleft->y + _vleft->z*_vleft->z);
	_vleft->x /= tmp;
	_vleft->y /= tmp;
	_vleft->y /= tmp;
  
	_ptarget->x = _pcamera->x + _vforward->x;
	_ptarget->y = _pcamera->y + _vforward->y;
	_ptarget->z = _pcamera->z + _vforward->z;
}

DirectX::XMFLOAT3 *engine::MovableCamera::getForward(void) const
{
	return _vforward;
}
