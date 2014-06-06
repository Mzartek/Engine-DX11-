#include <Engine/PlayerCam.hpp>

engine::PlayerCam::PlayerCam(void)
{
}

engine::PlayerCam::~PlayerCam(void)
{
}

void engine::PlayerCam::keyboardMove(const BOOL &f, const BOOL &b, const BOOL &l, const BOOL &r)
{
	if(f)
	{
		_pcamera->x -= _vleft->z * _speed;
		_pcamera->z += _vleft->x * _speed;
	}
	if(b)
	{
		_pcamera->x += _vleft->z * _speed;
		_pcamera->z -= _vleft->x * _speed;
	}
	if (l)
	{
		_pcamera->x += _vleft->x * _speed;
		_pcamera->y += _vleft->y * _speed;
		_pcamera->z += _vleft->z * _speed;

	}
	if (r)
	{
		_pcamera->x -= _vleft->x * _speed;
		_pcamera->y -= _vleft->y * _speed;
		_pcamera->z -= _vleft->z * _speed;
	}
  
	_ptarget->x = _pcamera->x + _vforward->x;
	_ptarget->y = _pcamera->y + _vforward->y;
	_ptarget->z = _pcamera->z + _vforward->z;
}
