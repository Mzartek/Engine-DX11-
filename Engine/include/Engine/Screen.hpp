#ifndef SCREEN
#define SCREEN

#include "ShaderProgram.hpp"
#include "GBuffer.hpp"

namespace engine
{
	class DLLAPI Screen : public Object
	{
	private:
		ID3D11Buffer *_pScreenColorBuffer;
		ID3D11Buffer *_pVertexBuffer;
		ID3D11InputLayout *_pInputLayout;
		DirectX::XMFLOAT4 *_color;
		// Do not delete it
		ShaderProgram *_program;
		ID3D11Device *_pd3dDevice;
		ID3D11DeviceContext *_pContext;
	public:
		Screen();
		~Screen();
		HRESULT config(ShaderProgram *program, ID3D11Device *pd3dDevice, ID3D11DeviceContext *pContext);
		void display(GBuffer *gbuf, const FLOAT &r, const FLOAT &g, const FLOAT &b, const FLOAT &a);
	};
}

#endif
