#ifndef SCREEN
#define SCREEN

#include "ShaderProgram.hpp"
#include "GBuffer.hpp"
#include "LBuffer.hpp"

namespace engine
{
	class DLLAPI Screen : public Object
	{
	private:
		ID3D11Buffer *_pScreenColorBuffer;
		ID3D11InputLayout *_pInputLayout;
		ID3D11Buffer *_pVertexBuffer;
		XMFLOAT4 *_color;
		// Do not delete it
		ShaderProgram *_program;
	public:
		Screen();
		~Screen();
		void config(ShaderProgram *program, ID3D11Device *pd3dDevice);
		void display(Renderer *renderer, GBuffer *gbuf, LBuffer *lbuf, const FLOAT &r, const FLOAT &g, const FLOAT &b, const FLOAT &a);
	};
}

#endif
