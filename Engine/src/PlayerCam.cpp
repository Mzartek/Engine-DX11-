#include <Engine/PlayerCam.hpp>

engine::PlayerCam::PlayerCam(void)
{
}

engine::PlayerCam::~PlayerCam(void)
{
}

void engine::PlayerCam::keyboardMove(const BOOL &f, const BOOL &b, const BOOL &l, const BOOL &r)
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
	*_ptarget = *_pcamera + *_vforward;
}
