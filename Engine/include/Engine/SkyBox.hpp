#ifndef SKYBOX
#define SKYBOX

#include "Object.hpp"

namespace engine
{
	class ShaderProgram;
	class GBuffer;
	class Camera;

	class DLLAPI SkyBox : public Object
	{
	private:
		ID3D11Texture2D *_pTexture;
		ID3D11ShaderResourceView *_pShaderResourceView;
		ID3D11SamplerState *_pSamplerState;
		ID3D11InputLayout *_pInputLayout;
		ID3D11Buffer *_pVertexBuffer;
		ID3D11Buffer *_pIndexBuffer;
		ID3D11Buffer *_pMVPMatrixBuffer;
		XMMATRIX *_rotateMatrix;
		ShaderProgram *_program;
		UINT _numElement;
	public:
		SkyBox(void);
		~SkyBox(void);
		void load(const TCHAR *posx, const TCHAR *negx,
			const TCHAR *posy, const TCHAR *negy,
			const TCHAR *posz, const TCHAR *negz,
			FLOAT dim, ShaderProgram *program, ID3D11Device *pd3dDevice);
		void rotate(const FLOAT &angle, const FLOAT &x, const FLOAT &y, const FLOAT &z);
		void display(GBuffer *gbuf, Camera *cam);
	};
}


#endif
