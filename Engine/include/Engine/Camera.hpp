#ifndef CAMERA
#define CAMERA

#include "ShaderProgram.hpp"

namespace Engine
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
		~Camera(void);
		void setPositionCamera(const XMVECTOR &pos);
		void setPositionTarget(const XMVECTOR &pos);
		void setPerspective(const FLOAT &fov, const UINT &width, const UINT &height, const FLOAT &n, const FLOAT &f);
		XMVECTOR getPositionCamera(void) const;
		XMVECTOR getPositionTarget(void) const;
		XMMATRIX getProjectionMatrix(void) const;
		XMMATRIX getVPMatrix(void) const;
		void position(void);
	};
}

#endif
