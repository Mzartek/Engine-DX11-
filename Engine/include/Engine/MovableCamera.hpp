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
	public:
		MovableCamera(void);
		~MovableCamera(void);
		void setSpeed(const FLOAT &v);
		void mouseMove(const INT &xrel, const INT &yrel);
		virtual void keyboardMove(const BOOL &f, const BOOL &b, const BOOL &l, const BOOL &r) = 0;
	};
}

#endif
