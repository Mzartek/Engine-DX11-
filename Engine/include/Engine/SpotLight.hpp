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
			XMMATRIX shadowMatrix;
			XMFLOAT3 __declspec(align(16)) color;
			XMFLOAT3 __declspec(align(16)) position;
			XMFLOAT3 __declspec(align(16)) direction;
			FLOAT spotCutOff;
			FLOAT __declspec(align(16)) maxDistance;
			INT withShadowMapping;
		} _lightInfo;
	public:
		SpotLight(ShaderProgram *program);
		~SpotLight(void);
		void setColor(const XMVECTOR &color);
		void setPosition(const XMVECTOR &position);
		void setDirection(const XMVECTOR &dir);
		void setSpotCutOff(const FLOAT &spot);
		void setMaxDistance(const FLOAT &maxDistance);
		void setShadowMapping(const BOOL &shadow);
		void configShadowMap(const UINT &width, const UINT &height) const;
		ShadowMap *getShadowMap(void) const;
		XMMATRIX getProjectionMatrix(void) const;
		XMMATRIX getViewMatrix(void) const;
		XMMATRIX getVPMatrix(void) const;
		XMVECTOR getColor(void) const;
		XMVECTOR getPosition(void) const;
		XMVECTOR getDirection(void) const;
		FLOAT getSpotCutOff(void) const;
		FLOAT getMaxDistance(void) const;
		void position(void) const;
		void clear(void) const;
		void display(GBuffer *gbuf, Camera *cam);
	};
}

#endif