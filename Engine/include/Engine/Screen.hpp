#ifndef SCREEN
#define SCREEN

#include "ShaderProgram.hpp"
#include "GBuffer.hpp"
#include "Window.hpp"

namespace engine
{
	class DLLAPI Screen : public Object
	{
	private:
		struct uniform0
		{
			FLOAT screen[2];
		};
		struct uniform1
		{
			FLOAT color[4];
		};
		ID3D11Buffer *_pConstantBuffer0;
		ID3D11Buffer *_pConstantBuffer1;
		ID3D11Buffer *_pVertexBuffer;
		ID3D11InputLayout *_pInputLayout;
		struct uniform0 *_screen;
		struct uniform1 *_color;
		ShaderProgram *_program;
	public:
		Screen();
		~Screen();
		HRESULT config(ShaderProgram *program, ID3D11Device *pd3dDevice);
		void display(Window *win, GBuffer *gbuf, const FLOAT &r, const FLOAT &g, const FLOAT &b, const FLOAT &a);
	};
}

#endif
