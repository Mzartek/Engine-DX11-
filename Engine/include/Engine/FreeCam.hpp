#ifndef FREECAM_HEADER
#define FREECAM_HEADER

#include "MovableCamera.hpp"

namespace Engine
{
	class DLLAPI FreeCam : public MovableCamera
	{
	public:
		FreeCam(void);
		~FreeCam(void);
		void keyboardMove(const BOOL &f, const BOOL &b, const BOOL &l, const BOOL &r);
	};
}

#endif
