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
		~Camera(void);
		void setPositionCamera(const XMFLOAT3 &pos);
		void setPositionTarget(const XMFLOAT3 &pos);
		void setPerspective(const FLOAT &fov, const UINT &width, const UINT &height, const FLOAT &n, const FLOAT &f);
		XMFLOAT3 getPositionCamera(void) const;
		XMFLOAT3 getPositionTarget(void) const;
		XMMATRIX getProjectionMatrix(void) const;
		XMMATRIX getVPMatrix(void) const;
		void position(void);
	};
}

#endif
