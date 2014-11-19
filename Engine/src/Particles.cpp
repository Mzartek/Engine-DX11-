#include <Engine/Particles.hpp>
#include <Engine/ShaderProgram.hpp>

Engine::Particles::Particles(EngineDevice EngineDevice, ShaderProgram *program)
	: _numElement(0)
{
	_EngineDevice = EngineDevice;

	_program = program;

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "IN_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "IN_LIFE", 0, DXGI_FORMAT_R32_FLOAT, 0, 3 * sizeof(FLOAT), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	_EngineDevice.Device->CreateInputLayout(layout, ARRAYSIZE(layout), _program->getEntryBufferPointer(), _program->getEntryBytecodeLength(), &_pInputLayout);
}

Engine::Particles::~Particles(void)
{
	_pInputLayout->Release();
}