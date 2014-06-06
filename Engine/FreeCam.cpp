#include <Engine/FreeCam.hpp>

engine::FreeCam::FreeCam(void)
{
}

engine::FreeCam::~FreeCam(void)
{
}

void engine::FreeCam::keyboardMove(const BOOL &f, const BOOL &b, const BOOL &l, const BOOL &r)
{
	if (f)
	{
		_pcamera->x += _vforward->x * _speed;
		_pcamera->y += _vforward->y * _speed;
		_pcamera->z += _vforward->z * _speed;
	}
	if (b)
	{
		_pcamera->x -= _vforward->x * _speed;
		_pcamera->y -= _vforward->y * _speed;
		_pcamera->z -= _vforward->z * _speed;
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
