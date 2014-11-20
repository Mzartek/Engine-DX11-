#include <Engine/Light.hpp>
#include <Engine/Buffer.hpp>
#include <Engine/ShadowMap.hpp>
#include <Engine/ShaderProgram.hpp>

Engine::Light::Light(const EngineDevice &EngineDevice, ShaderProgram *program)
	: _EngineDevice(EngineDevice), _program(program)
{
	_vertexBuffer = new Buffer(_EngineDevice);
	_mainInfoBuffer = new Buffer(_EngineDevice);
	_lightInfoBuffer = new Buffer(_EngineDevice);
	_shadow = new ShadowMap(_EngineDevice);
	_projectionMatrix = (XMMATRIX *)_aligned_malloc(sizeof *_projectionMatrix, 16);
	_viewMatrix = (XMMATRIX *)_aligned_malloc(sizeof *_viewMatrix, 16);
	_VPMatrix = (XMMATRIX *)_aligned_malloc(sizeof *_VPMatrix, 16);

	FLOAT vertex[] = 
	{
		-1, -1,
		1, -1,
		-1, 1,
		1, 1,
	};
	_vertexBuffer->createStore(D3D11_BIND_VERTEX_BUFFER, vertex, sizeof vertex, D3D11_USAGE_IMMUTABLE);
	_mainInfoBuffer->createStore(D3D11_BIND_CONSTANT_BUFFER, NULL, 160, D3D11_USAGE_DYNAMIC);
}

Engine::Light::~Light(void)
{
	delete _vertexBuffer;
	delete _mainInfoBuffer;
	delete _lightInfoBuffer;
	delete _shadow;
	_aligned_free(_projectionMatrix);
	_aligned_free(_viewMatrix);
	_aligned_free(_VPMatrix);
}

void Engine::Light::configShadowMap(const UINT &width, const UINT &height)
{
	_shadow->config(width, height);
}

XMMATRIX Engine::Light::getProjectionMatrix(void) const
{
	return *_projectionMatrix;
}

XMMATRIX Engine::Light::getViewMatrix(void) const
{
	return *_viewMatrix;
}

XMMATRIX Engine::Light::getVPMatrix(void) const
{
	return *_VPMatrix;
}

Engine::ShadowMap *Engine::Light::getShadowMap(void)
{
	return _shadow;
}

void Engine::Light::clear(void) const
{
	_shadow->clear();
}