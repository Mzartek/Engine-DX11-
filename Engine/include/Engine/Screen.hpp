#ifndef SCREEN
#define SCREEN

#include "ShaderProgram.hpp"
#include "GBuffer.hpp"
#include "Renderer.hpp"

namespace engine
{
	class DLLAPI Screen : public Object
	{
	private:
		struct screenColor
		{
			FLOAT color[4];
		};
		ID3D11Buffer *_pScreenColorBuffer;
		ID3D11Buffer *_pVertexBuffer;
		ID3D11InputLayout *_pInputLayout;
		struct screenColor *_color;
		ShaderProgram *_program;
	public:
		Screen();
		~Screen();
		HRESULT config(ShaderProgram *program, ID3D11Device *pd3dDevice);
		void display(Renderer *renderer, GBuffer *gbuf, const FLOAT &r, const FLOAT &g, const FLOAT &b, const FLOAT &a);
	};
}

#endif
