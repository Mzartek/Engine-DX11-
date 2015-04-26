#include <Engine/PlayerCam.hpp>

Engine::PlayerCam::PlayerCam(void)
{
}

Engine::PlayerCam::PlayerCam(const FLOAT &atheta, const FLOAT &aphi)
	: MovableCamera(atheta, aphi)
{
}

Engine::PlayerCam::~PlayerCam(void)
{
}

void Engine::PlayerCam::keyboardMove(const BOOL &f, const BOOL &b, const BOOL &l, const BOOL &r)
{
	if (f)
	{
		*_pcamera = XMVectorSetX(*_pcamera, XMVectorGetX(*_pcamera) - (XMVectorGetZ(*_vleft) * _speed));
		*_pcamera = XMVectorSetZ(*_pcamera, XMVectorGetZ(*_pcamera) + (XMVectorGetX(*_vleft) * _speed));
	}
	if (b)
	{
		*_pcamera = XMVectorSetX(*_pcamera, XMVectorGetX(*_pcamera) + (XMVectorGetZ(*_vleft) * _speed));
		*_pcamera = XMVectorSetZ(*_pcamera, XMVectorGetZ(*_pcamera) - (XMVectorGetX(*_vleft) * _speed));
	}
	if (l)
		*_pcamera += *_vleft * _speed;
	if (r)
		*_pcamera -= *_vleft * _speed;
}
