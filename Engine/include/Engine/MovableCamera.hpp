#ifndef MOVABLE_CAMERA
#define MOVABLE_CAMERA

#include "Camera.hpp"

namespace engine
{
	class DLLAPI MovableCamera : public Camera
	{
	private:
		FLOAT _atheta;
		FLOAT _aphi;
	protected:
		XMVECTOR *_vforward;
		XMVECTOR *_vleft;
		FLOAT _speed;
	public:
		MovableCamera(void);
		~MovableCamera(void);
		void setInitialAngle(const FLOAT &t, const FLOAT &p);
		void setSpeed(const FLOAT &v);
		void mouseMove(const INT &xrel, const INT &yrel);
		XMFLOAT3 getForward(void) const;
		XMFLOAT3 getLeft(void) const;
		virtual void keyboardMove(const BOOL &f, const BOOL &b, const BOOL &l, const BOOL &r) = 0;
	};
}

#endif
