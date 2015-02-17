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
			XMMATRIX shadowMatrix[3];
			XMFLOAT3 __declspec(align(16)) color;
			XMFLOAT3 __declspec(align(16)) direction;
			INT withShadowMapping;
		} _lightInfo;
	public:
		DirLight(ShaderProgram *program);
		~DirLight(void);
		void setColor(const XMVECTOR &color);
		void setDirection(const XMVECTOR &dir);
		void setShadowMapping(const BOOL &shadow);
		void configShadowMap(const UINT &width, const UINT &height) const;
		ShadowMap *getShadowMap(const UINT &num) const;
		XMMATRIX getProjectionMatrix(const UINT &num) const;
		XMMATRIX getViewMatrix(const UINT &num) const;
		XMMATRIX getVPMatrix(const UINT &num) const;
		XMVECTOR getColor(void) const;
		XMVECTOR getDirection(void) const;
		void position(const XMVECTOR &pos, const FLOAT &dim0, const FLOAT &dim1, const FLOAT &dim2) const;
		void clear(void) const;
		void display(GBuffer *gbuf, Camera *cam);
	};
}

#endif
