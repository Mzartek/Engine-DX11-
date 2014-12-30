#ifndef CAMERA_HEADER
#define CAMERA_HEADER

#include "Object.hpp"

namespace Engine
{
	class DLLAPI Camera : public Object
	{
	protected:
		XMVECTOR *_pcamera;
		XMVECTOR *_ptarget;
		XMVECTOR *_vview;
		XMMATRIX *_projectionMatrix;
		XMMATRIX *_viewMatrix;
		XMMATRIX *_VPMatrix;
		XMMATRIX *_IVPMatrix;
		FLOAT _fov;
		FLOAT _distance;
		FLOAT _frusSphereRadian;
		XMVECTOR *_frusSpherePosition;
	public:
		Camera(void);
		~Camera(void);
		void setCameraPosition(const XMVECTOR &pos);
		void setTargetPosition(const XMVECTOR &pos);
		void setPerspective(const FLOAT &fov, const UINT &width, const UINT &height, const FLOAT &n, const FLOAT &f);
		XMVECTOR getCameraPosition(void) const;
		XMVECTOR getTargetPosition(void) const;
		XMVECTOR getViewVector(void) const;
		XMMATRIX getProjectionMatrix(void) const;
		XMMATRIX getViewMatrix(void) const;
		XMMATRIX getVPMatrix(void) const;
		XMMATRIX getIVPMatrix(void) const;
		FLOAT getFOV(void) const;
		FLOAT getFrusSphereRadian(void) const;
		XMVECTOR getFrusSpherePosition(void) const;
		void position(void);
	};
}

#endif
