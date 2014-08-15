#ifndef CAMERA
#define CAMERA

#include "ShaderProgram.hpp"

namespace engine
{
	class DLLAPI Camera : public Object
	{
	protected:
		DirectX::XMFLOAT3 *_pcamera;
		DirectX::XMFLOAT3 *_ptarget;
		DirectX::XMMATRIX *_projectionMatrix;
		DirectX::XMMATRIX *_VPMatrix;
	public:
		Camera(void);
		Camera(const FLOAT &x, const FLOAT &y, const FLOAT &z);
		virtual ~Camera(void);
		void setPositionCamera(const FLOAT &x, const FLOAT &y, const FLOAT &z);
		void setPositionTarget(const FLOAT &x, const FLOAT &y, const FLOAT &z);
		void setPerspective(const FLOAT &fov, const UINT &width, const UINT &height, const FLOAT &n, const FLOAT &f);
		DirectX::XMFLOAT3 getPositionCamera(void) const;
		DirectX::XMFLOAT3 getPositionTarget(void) const;
		DirectX::XMMATRIX getProjectionMatrix(void) const;
		DirectX::XMMATRIX getVPMatrix(void) const;
		void position(void);
	};
}

#endif
