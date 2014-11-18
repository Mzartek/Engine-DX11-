#ifndef SPOTLIGHT_HEADER
#define SPOTLIGHT_HEADER

#include "Light.hpp"

namespace Engine
{
	class DLLAPI SpotLight : public Light
	{
	private:
		struct
		{
			XMFLOAT3 __declspec(align(16)) color;
			XMFLOAT3 __declspec(align(16)) position;
			XMFLOAT3 __declspec(align(16)) direction;
			FLOAT spotCutOff;
			INT __declspec(align(16)) withShadowMapping;
		} _lightInfo;
	public:
		SpotLight(const EngineDevice &EngineDevice, ShaderProgram *program);
		~SpotLight(void);
		void setColor(const XMVECTOR &color);
		void setPosition(const XMVECTOR &position);
		void setDirection(const XMVECTOR &dir);
		void setSpotCutOff(const FLOAT &spot);
		void setShadowMapping(const BOOL &shadow);
		XMVECTOR getColor(void) const;
		XMVECTOR getPosition(void) const;
		XMVECTOR getDirection(void) const;
		FLOAT getSpotCutOff(void) const;
		void position(void);
		void display(GBuffer *gbuf, Camera *cam);
	};
}

#endif