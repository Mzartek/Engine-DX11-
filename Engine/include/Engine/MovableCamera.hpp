#ifndef MOVABLECAMERA_HEADER
#define MOVABLECAMERA_HEADER

#include "PerspCamera.hpp"

namespace Engine
{
	class DLLAPI MovableCamera : public PerspCamera
	{
	protected:
		FLOAT _speed;
		FLOAT _atheta;
		FLOAT _aphi;
		XMVECTOR *_vforward;
		XMVECTOR *_vleft;
	public:
		MovableCamera(void);
		~MovableCamera(void);
		void setInitialAngle(const FLOAT &t, const FLOAT &p);
		void setSpeed(const FLOAT &v);
		XMVECTOR getForwardVector(void) const;
		XMVECTOR getLeftVector(void) const;
		void mouseMove(const INT &xrel, const INT &yrel);
		virtual void keyboardMove(const BOOL &f, const BOOL &b, const BOOL &l, const BOOL &r) = 0;
	};
}

#endif
