#ifndef PARTICLES_HEADER
#define PARTICLES_HEADER

#include "Object.hpp"

namespace Engine
{
	class ShaderProgram;
	class Buffer;

	class DLLAPI Particles : public Object
	{
	protected:
		EngineDevice _EngineDevice;
		ShaderProgram *_program;
		ID3D11InputLayout *_pInputLayout;
		Buffer *_vertexBuffer;
		UINT _numElement;
	public:
		Particles(EngineDevice EngineDevice, ShaderProgram *program);
		~Particles(void);
	};
}

#endif