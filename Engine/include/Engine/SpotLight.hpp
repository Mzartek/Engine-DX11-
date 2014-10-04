#ifndef SPOT_LIGHT
#define SPOT_LIGHT

#include "Light.hpp"

namespace Engine
{
	class DLLAPI SpotLight : public Light
	{
	private:
		struct lightInfo
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
		void setColor(const XMFLOAT3 &color);
		void setPosition(const XMFLOAT3 &position);
		void setDirection(const XMFLOAT3 &dir);
		void setSpotCutOff(const FLOAT &spot);
		void setShadowMapping(const BOOL &shadow);
		XMFLOAT3 getColor(void) const;
		XMFLOAT3 getPosition(void) const;
		XMFLOAT3 getDirection(void) const;
		FLOAT getSpotCutOff(void) const;
		void position(void);
		void display(GBuffer *gbuf, Camera *cam);
	};
}

#endif