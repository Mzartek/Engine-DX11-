#ifndef PERSPCAMERA_HEADER
#define PERSPCAMERA_HEADER

#include <Engine/Camera.hpp>

namespace Engine
{
	class DLLAPI PerspCamera : public Camera
	{
	protected:
		XMVECTOR *_pcamera;
		XMVECTOR *_ptarget;
		XMVECTOR *_vview;
		FLOAT _near;
		FLOAT _far;
		FLOAT _fov;
		FLOAT _frusSphereDistance;
		FLOAT _frusSphereRadius;
		XMVECTOR *_frusSpherePosition;
	public:
		PerspCamera(void);
		~PerspCamera(void);
		void setCameraPosition(const XMVECTOR &pos) const;
		void setTargetPosition(const XMVECTOR &pos) const;
		void setPerspective(const FLOAT &fov, const UINT &width, const UINT &height, const FLOAT &n, const FLOAT &f);
		XMVECTOR getCameraPosition(void) const;
		XMVECTOR getTargetPosition(void) const;
		XMVECTOR getViewVector(void) const;
		FLOAT getNear(void) const;
		FLOAT getFar(void) const;
		FLOAT getFOV(void) const;
		FLOAT getFrusSphereDistance(void) const;
		FLOAT getFrusSphereRadius(void) const;
		XMVECTOR getFrusSpherePosition(void) const;
		void position(void) const;
	};
}

#endif