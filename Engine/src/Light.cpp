#include <Engine/Light.hpp>
#include <Engine/Buffer.hpp>
#include <Engine/ShadowMap.hpp>
#include <Engine/ShaderProgram.hpp>

Engine::Light::Light(const EngineDevice &EngineDevice, ShaderProgram *program)
{
	_EngineDevice = EngineDevice;
	_program = program;

	_vertexBuffer = new Buffer(_EngineDevice);
	_shadowMatrixBuffer = new Buffer(_EngineDevice);
	_IVPMatrixBuffer = new Buffer(_EngineDevice);
	_screenBuffer = new Buffer(_EngineDevice);
	_cameraBuffer = new Buffer(_EngineDevice);
	_lightInfoBuffer = new Buffer(_EngineDevice);
	_shadow = new ShadowMap(_EngineDevice);
	_VPMatrix = (XMMATRIX *)_aligned_malloc(sizeof *_VPMatrix, 16);

	FLOAT vertex[] = 
	{
		-1, -1,
		1, -1,
		-1, 1,
		1, 1,
	};
	_vertexBuffer->createStore(D3D11_BIND_VERTEX_BUFFER, vertex, sizeof vertex, D3D11_USAGE_IMMUTABLE);
	_shadowMatrixBuffer->createStore(D3D11_BIND_CONSTANT_BUFFER, NULL, sizeof XMMATRIX, D3D11_USAGE_DYNAMIC);
	_IVPMatrixBuffer->createStore(D3D11_BIND_CONSTANT_BUFFER, NULL, sizeof XMMATRIX, D3D11_USAGE_DYNAMIC);
	_screenBuffer->createStore(D3D11_BIND_CONSTANT_BUFFER, NULL, 16, D3D11_USAGE_DYNAMIC);
	_cameraBuffer->createStore(D3D11_BIND_CONSTANT_BUFFER, NULL, 16, D3D11_USAGE_DYNAMIC);
}

Engine::Light::~Light(void)
{
	delete _vertexBuffer;
	delete _shadowMatrixBuffer;
	delete _IVPMatrixBuffer;
	delete _screenBuffer;
	delete _cameraBuffer;
	delete _lightInfoBuffer;
	delete _shadow;
	_aligned_free(_VPMatrix);
}

void Engine::Light::configShadowMap(const UINT &width, const UINT &height)
{
	_shadow->config(width, height);
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