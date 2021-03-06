#include <Engine/FreeCam.hpp>

Engine::FreeCam::FreeCam(void)
{
}

Engine::FreeCam::FreeCam(const FLOAT &atheta, const FLOAT &aphi)
	: MovableCamera(atheta, aphi)
{
}

Engine::FreeCam::~FreeCam(void)
{
}

void Engine::FreeCam::keyboardMove(const BOOL &f, const BOOL &b, const BOOL &l, const BOOL &r)
{
	if (f)
		*_pcamera += *_vforward * _speed;
	if (b)
		*_pcamera -= *_vforward * _speed;
	if (l)
		*_pcamera += *_vleft * _speed;
	if (r)
		*_pcamera -= *_vleft * _speed;
}
