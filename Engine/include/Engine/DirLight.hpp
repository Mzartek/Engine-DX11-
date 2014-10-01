#ifndef DIR_LIGHT
#define DIR_LIGHT

#include "Light.hpp"

namespace Engine
{
	class DLLAPI DirLight : public Light
	{
	private:
		struct lightInfo
		{
			XMFLOAT3 __declspec(align(16)) color;
			XMFLOAT3 __declspec(align(16)) direction;
			INT withShadowMapping;
		} _lightInfo;
	public:
		DirLight(ShaderProgram *program);
		~DirLight(void);
		void setColor(const XMFLOAT3 &color);
		void setDirection(const XMFLOAT3 &dir);
		void setShadowMapping(const BOOL &shadow);
		XMFLOAT3 getColor(void) const;
		XMFLOAT3 getDirection(void) const;
		void position(const XMFLOAT3 &position, const FLOAT &dim);
		void display(GBuffer *gbuf, Camera *cam);
	};
}

#endif
