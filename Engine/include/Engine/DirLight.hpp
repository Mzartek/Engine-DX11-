#ifndef DIR_LIGHT
#define DIR_LIGHT

#include "Light.hpp"

namespace Engine
{
	class DLLAPI DirLight : public Light
	{
	private:
		struct
		{
			XMFLOAT3 __declspec(align(16)) color;
			XMFLOAT3 __declspec(align(16)) direction;
			INT withShadowMapping;
		} _lightInfo;
	public:
		DirLight(const EngineDevice &EngineDevice, ShaderProgram *program);
		~DirLight(void);
		void setColor(const XMVECTOR &color);
		void setDirection(const XMVECTOR &dir);
		void setShadowMapping(const BOOL &shadow);
		XMVECTOR getColor(void) const;
		XMVECTOR getDirection(void) const;
		void position(const XMVECTOR &position, const FLOAT &dim);
		void display(GBuffer *gbuf, Camera *cam);
	};
}

#endif
