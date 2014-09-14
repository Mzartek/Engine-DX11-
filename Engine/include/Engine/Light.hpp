#ifndef LIGHT
#define LIGHT

#include "Object.hpp"

namespace engine
{
	class ShadowMap;
	class ShaderProgram;
	class GBuffer;
	class Camera;

	class DLLAPI Light : public Object
	{
	protected:
		ID3D11Buffer *_pShadowMatrixBuffer;
		ID3D11Buffer *_pIVPMatrixBuffer;
		ID3D11Buffer *_pScreenBuffer;
		ID3D11Buffer *_pCameraBuffer;
		ID3D11Buffer *_pLightInfoBuffer;
		ID3D11InputLayout *_pInputLayout;
		ID3D11Buffer *_pVertexBuffer;
		XMMATRIX *_VPMatrix;
		ShadowMap *_shadow;
		ShaderProgram *_program;
		ID3D11DeviceContext *_pContext;
	public:
		Light(void);
		~Light(void);
		void configShadowMap(const UINT &width, const UINT &height, ID3D11Device *pd3dDevice, ID3D11DeviceContext *pContext);
		XMMATRIX getVPMatrix(void) const;
		ShadowMap *getShadowMap(void);
		void clear(void) const;
		virtual void display(GBuffer *gbuf, Camera *cam) = 0;
	};
}

#endif