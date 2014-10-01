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
		Buffer *_vertexBuffer;
		Buffer *_screenColorBuffer;
		ShaderProgram *_backgroundProgram;
		ShaderProgram *_directProgram;
		ID3D11InputLayout *_pInputLayout;
	public:
		Screen(ShaderProgram *backgroundProgram, ShaderProgram *directProgram);
		~Screen(void);
		void background(GBuffer *gbuf);
		void display(Renderer *renderer, GBuffer *gbuf, const FLOAT &r, const FLOAT &g, const FLOAT &b, const FLOAT &a);
	};
}

#endif
