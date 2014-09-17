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
		*_pcamera += *_vforward * _speed;
	if (b)
		*_pcamera -= *_vforward * _speed;
	if (l)
		*_pcamera += *_vleft * _speed;
	if (r)
		*_pcamera -= *_vleft * _speed;
	*_ptarget = *_pcamera + *_vforward;
}
