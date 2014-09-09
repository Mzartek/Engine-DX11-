#ifndef DIR_LIGHT
#define DIR_LIGHT

#include "Light.hpp"

namespace engine
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
		DirLight(void);
		~DirLight(void);
		void config(ShaderProgram *program, ID3D11Device *pd3dDevice, ID3D11DeviceContext *pContext);
		void setColor(const XMFLOAT3 &color);
		void setDirection(const XMFLOAT3 &dir);
		XMFLOAT3 getColor(void) const;
		XMFLOAT3 getDirection(void) const;
		void activateShadowMapping(const BOOL &shadow);
		void position(const XMFLOAT3 &position, const FLOAT &dim);
		void display(GBuffer *gbuf, Camera *cam);
	};
}

#endif
