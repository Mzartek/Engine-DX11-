#ifndef ORTHOCAMERA_HEADER
#define ORTHOCAMERA_HEADER

#include <Engine/Camera.hpp>

namespace Engine
{
	class DLLAPI OrthoCamera : public Camera
	{
	protected:
		XMVECTOR *_position;
		XMVECTOR *_direction;
		FLOAT _left;
		FLOAT _right;
		FLOAT _bottom;
		FLOAT _top;
		FLOAT _zNear;
		FLOAT _zFar;
	public:
		OrthoCamera(void);
		~OrthoCamera(void);
		void setPosition(const XMVECTOR &pos) const;
		void setDirection(const XMVECTOR &dir) const;
		void setOrthogonal(const FLOAT &left, const FLOAT &right, const FLOAT &bottom, const FLOAT &top, const FLOAT &zNear, const FLOAT &zFar);
		void position(void) const;
	};
}

#endif