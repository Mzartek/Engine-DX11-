#ifndef PLAYER_CAM
#define PLAYER_CAM

#include "MovableCamera.hpp"

namespace engine
{
	class DLLAPI PlayerCam : public MovableCamera
	{
	public:
		PlayerCam(void);
		~PlayerCam(void);
		void keyboardMove(const BOOL &f, const BOOL &b, const BOOL &l, const BOOL &r);
	};
}

#endif
