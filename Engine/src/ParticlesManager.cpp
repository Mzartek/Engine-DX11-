#include <Engine/ParticlesManager.hpp>
#include <Engine/ShaderProgram.hpp>

Engine::ParticlesManager::ParticlesManager(ShaderProgram *program)
	: _program(program), _numElement(0)
{
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "IN_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "IN_LIFE", 0, DXGI_FORMAT_R32_FLOAT, 0, 3 * sizeof(FLOAT), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	Device->CreateInputLayout(layout, ARRAYSIZE(layout), _program->getEntryBufferPointer(), _program->getEntryBytecodeLength(), &_pInputLayout);
}

Engine::ParticlesManager::~ParticlesManager(void)
{
	_pInputLayout->Release();
}