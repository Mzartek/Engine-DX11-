#ifndef SPOT_LIGHT
#define SPOT_LIGHT

#include "Light.hpp"

namespace engine
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
		SpotLight(void);
		~SpotLight(void);
		void config(ShaderProgram *program, ID3D11Device *pd3dDevice, ID3D11DeviceContext *pContext);
		void setColor(const XMFLOAT3 &color);
		void setPosition(const XMFLOAT3 &position);
		void setDirection(const XMFLOAT3 &dir);
		void setSpotCutOff(const FLOAT &spot);
		XMFLOAT3 getColor(void) const;
		XMFLOAT3 getPosition(void) const;
		XMFLOAT3 getDirection(void) const;
		FLOAT getSpotCutOff(void) const;
		void activateShadowMapping(const BOOL &shadow);
		void position(void);
		void display(LBuffer *lbuf, GBuffer *gbuf, Camera *cam);
	};
}

#endif