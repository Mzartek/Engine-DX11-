#ifndef SCREEN
#define SCREEN

#include "Object.hpp"

namespace engine
{
	class ShaderProgram;
	class GBuffer;
	class Renderer;

	class DLLAPI Screen : public Object
	{
	private:
		ID3D11InputLayout *_pInputLayout;
		ID3D11Buffer *_pVertexBuffer;
		ID3D11Buffer *_pScreenColorBuffer;
		// Do not delete it
		ShaderProgram *_backgroundProgram;
		ShaderProgram *_directProgram;
	public:
		Screen();
		~Screen();
		void config(ShaderProgram *backgroundProgram, ShaderProgram *directProgram, ID3D11Device *pd3dDevice);
		void background(GBuffer *gbuf);
		void display(Renderer *renderer, GBuffer *gbuf, const FLOAT &r, const FLOAT &g, const FLOAT &b, const FLOAT &a);
	};
}

#endif
