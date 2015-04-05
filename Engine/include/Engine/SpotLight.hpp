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
			FLOAT __declspec(align(16)) maxDistance;
			XMMATRIX shadowMatrix;
		} _lightInfo;
	public:
		SpotLight(ShaderProgram *program);
		~SpotLight(void);
		void setColor(const XMVECTOR &color);
		void setPosition(const XMVECTOR &position);
		void setDirection(const XMVECTOR &dir);
		void setSpotCutOff(const FLOAT &spot);
		void setMaxDistance(const FLOAT &maxDistance);
		XMMATRIX getProjectionMatrix(void) const;
		XMMATRIX getViewMatrix(void) const;
		XMMATRIX getVPMatrix(void) const;
		XMVECTOR getColor(void) const;
		XMVECTOR getPosition(void) const;
		XMVECTOR getDirection(void) const;
		FLOAT getSpotCutOff(void) const;
		FLOAT getMaxDistance(void) const;
		void position(DepthMap *dmap);
		void display(GBuffer *gbuf, PerspCamera *cam);
		void display(GBuffer *gbuf, DepthMap *dmap, PerspCamera *cam);
	};
}

#endif