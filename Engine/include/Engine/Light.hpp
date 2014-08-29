#ifndef LIGHT
#define LIGHT

#include "ShadowMap.hpp"
#include "Camera.hpp"
#include "GBuffer.hpp"

namespace engine
{
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
		void configShadowMap(const UINT &width, const UINT &height, ShaderProgram *shadow, ID3D11Device *pd3dDevice, ID3D11DeviceContext *pContext);
		XMMATRIX getVPMatrix(void) const;
		ShadowMap *getShadowMap(void);
		void clear(void) const;
		virtual void display(GBuffer *g, Camera *cam) = 0;
	};
}

#endif