#ifndef DIRLIGHT_HEADER
#define DIRLIGHT_HEADER

#include "Light.hpp"

#define CSM_NUM 3

namespace Engine
{
	class DLLAPI DirLight : public Light
	{
	private:
		struct
		{
			XMFLOAT3 __declspec(align(16)) color;
			XMFLOAT3 __declspec(align(16)) direction;
			XMMATRIX shadowMatrix[CSM_NUM];
		} _lightInfo;
	public:
		DirLight(ShaderProgram *program);
		~DirLight(void);
		void setColor(const XMVECTOR &color);
		void setDirection(const XMVECTOR &dir);
		XMMATRIX getProjectionMatrix(const UINT &num) const;
		XMMATRIX getViewMatrix(const UINT &num) const;
		XMMATRIX getVPMatrix(const UINT &num) const;
		XMVECTOR getColor(void) const;
		XMVECTOR getDirection(void) const;
		void position(const XMVECTOR &pos, const FLOAT &dim0, const FLOAT &dim1, const FLOAT &dim2);
		void display(GBuffer *gbuf, Camera *cam);
		void display(GBuffer *gbuf, DepthMap *dmaps, Camera *cam);
	};
}

#endif
