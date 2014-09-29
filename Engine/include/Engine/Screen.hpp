#ifndef SCREEN
#define SCREEN

#include "Object.hpp"

namespace Engine
{
	class Buffer;
	class ShaderProgram;
	class GBuffer;
	class Renderer;

	class DLLAPI Screen : public Object
	{
	private:
		ID3D11InputLayout *_pInputLayout;
		Buffer *_vertexBuffer;
		Buffer *_screenColorBuffer;
		// Do not delete it
		ShaderProgram *_backgroundProgram;
		ShaderProgram *_directProgram;
	public:
		Screen(void);
		~Screen(void);
		void config(ShaderProgram *backgroundProgram, ShaderProgram *directProgram);
		void background(GBuffer *gbuf);
		void display(Renderer *renderer, GBuffer *gbuf, const FLOAT &r, const FLOAT &g, const FLOAT &b, const FLOAT &a);
	};
}

#endif
