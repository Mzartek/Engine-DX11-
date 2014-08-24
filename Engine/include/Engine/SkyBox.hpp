#ifndef SKYBOX
#define SKYBOX

#include "ShaderProgram.hpp"
#include "GBuffer.hpp"
#include "Camera.hpp"

namespace engine
{
	class DLLAPI SkyBox : public Object
	{
	private:
		UINT _numElement;
		ID3D11Resource *_pResource;
		ID3D11ShaderResourceView *_pShaderResourceView;
		ID3D11SamplerState *_pSamplerState;
		ID3D11Buffer *_pVertexBuffer;
		ID3D11Buffer *_pIndexBuffer;
		ID3D11InputLayout *_pInputLayout;
		ID3D11Buffer *_pMVPMatrixBuffer;
		XMMATRIX *_rotateMatrix;
		ShaderProgram *_program;
		ID3D11Device *_pd3dDevice;
		ID3D11DeviceContext *_pContext;
	public:
		SkyBox(void);
		~SkyBox(void);
		HRESULT load(const TCHAR *posx, const TCHAR *negx,
			const TCHAR *posy, const TCHAR *negy,
			const TCHAR *posz, const TCHAR *negz,
			FLOAT dim, ShaderProgram *program,
			ID3D11Device *pd3dDevice, ID3D11DeviceContext *pContext);
		void rotate(const FLOAT &angle, const FLOAT &x, const FLOAT &y, const FLOAT &z);
		void display(GBuffer *g, Camera *cam);
	};
}


#endif
