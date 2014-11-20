#ifndef PARTICLESMANAGER_HEADER
#define PARTICLESMANAGER_HEADER

#include "Object.hpp"

namespace Engine
{
	class ShaderProgram;
	class Buffer;

	class DLLAPI ParticlesManager : public Object
	{
	protected:
		EngineDevice _EngineDevice;
		ShaderProgram *_program;
		ID3D11InputLayout *_pInputLayout;
		Buffer *_vertexBuffer;
		UINT _numElement;
	public:
		ParticlesManager(EngineDevice EngineDevice, ShaderProgram *program);
		~ParticlesManager(void);
	};
}

#endif