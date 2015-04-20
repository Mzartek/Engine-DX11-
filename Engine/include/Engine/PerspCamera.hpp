#ifndef PERSPCAMERA_HEADER
#define PERSPCAMERA_HEADER

#include <Engine/Camera.hpp>

namespace Engine
{
	class DLLAPI PerspCamera : public Camera
	{
	protected:
		XMVECTOR *_pcamera;
		XMVECTOR *_vforward;
		XMVECTOR *_vleft;
		XMVECTOR *_vup;

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
		void setTargetPosition(const FLOAT &atheta, const FLOAT &aphi) const;
		void setPerspective(const FLOAT &fov, const UINT &width, const UINT &height, const FLOAT &n, const FLOAT &f);

		XMVECTOR getCameraPosition(void) const;
		XMVECTOR getForwardVector(void) const;
		XMVECTOR getLeftVector(void) const;
		XMVECTOR getUpVector(void) const;

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