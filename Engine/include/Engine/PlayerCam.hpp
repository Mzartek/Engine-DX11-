#ifndef PLAYERCAM_HEADER
#define PLAYERCAM_HEADER

#include "MovableCamera.hpp"

namespace Engine
{
	class DLLAPI PlayerCam : public MovableCamera
	{
	public:
		PlayerCam(void);
		PlayerCam(const FLOAT &atheta, const FLOAT &aphi);
		~PlayerCam(void);
		void keyboardMove(const BOOL &f, const BOOL &b, const BOOL &l, const BOOL &r);
	};
}

#endif
