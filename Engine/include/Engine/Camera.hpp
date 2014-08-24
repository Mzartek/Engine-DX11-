#ifndef CAMERA
#define CAMERA

#include "ShaderProgram.hpp"

namespace engine
{
	class DLLAPI Camera : public Object
	{
	protected:
		XMVECTOR *_pcamera;
		XMVECTOR *_ptarget;
		XMMATRIX *_projectionMatrix;
		XMMATRIX *_VPMatrix;
	public:
		Camera(void);
		Camera(const FLOAT &x, const FLOAT &y, const FLOAT &z);
		~Camera(void);
		void setPositionCamera(const FLOAT &x, const FLOAT &y, const FLOAT &z);
		void setPositionTarget(const FLOAT &x, const FLOAT &y, const FLOAT &z);
		void setPerspective(const FLOAT &fov, const UINT &width, const UINT &height, const FLOAT &n, const FLOAT &f);
		XMVECTOR getPositionCamera(void) const;
		XMVECTOR getPositionTarget(void) const;
		XMMATRIX getProjectionMatrix(void) const;
		XMMATRIX getVPMatrix(void) const;
		void position(void);
	};
}

#endif
