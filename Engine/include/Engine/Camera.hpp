#ifndef CAMERA_HEADER
#define CAMERA_HEADER

#include "Object.hpp"

namespace Engine
{
	class DLLAPI Camera : public Object
	{
	protected:
		XMMATRIX *_projectionMatrix;
		XMMATRIX *_viewMatrix;
		XMMATRIX *_VPMatrix;
		XMMATRIX *_IVPMatrix;
	public:
		Camera(void);
		~Camera(void);
		XMMATRIX getProjectionMatrix(void) const;
		XMMATRIX getViewMatrix(void) const;
		XMMATRIX getVPMatrix(void) const;
		XMMATRIX getIVPMatrix(void) const;
		virtual void position(void) const = 0;
	};
}

#endif
