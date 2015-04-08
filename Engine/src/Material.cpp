#include <Engine/Material.hpp>
#include <Engine/Texture2D.hpp>
#include <Engine/Buffer.hpp>

Engine::Material::Material(void)
	: _diffuseTexture(NULL), _specularTexture(NULL), _ambientTexture(NULL), _emissiveTexture(NULL), _shininessTexture(NULL), 
	_opacityTexture(NULL), 	_bumpMap(NULL), _normalMap(NULL), _displacementMap(NULL), _lightMap(NULL)
{
	_matBuffer = new Buffer;
	_stateBuffer = new Buffer;

	_matBuffer->createStore(D3D11_BIND_CONSTANT_BUFFER, NULL, sizeof _mat, D3D11_USAGE_DEFAULT);
	_stateBuffer->createStore(D3D11_BIND_CONSTANT_BUFFER, NULL, sizeof _state, D3D11_USAGE_DEFAULT);

	_state.hasDiffuseTexture = FALSE;
	_state.hasSpecularTexture = FALSE;
	_state.hasAmbientTexture = FALSE;
	_state.hasEmissiveTexture = FALSE;
	_state.hasShininessTexture = FALSE;
	_state.hasOpacityTexture = FALSE;
	_state.hasBumpMap = FALSE;
	_state.hasNormalMap = FALSE;
	_state.hasDisplacementMap = FALSE;
	_state.hasLightMap = FALSE;
}

Engine::Material::~Material(void)
{
	delete _matBuffer;
	delete _stateBuffer;
}

void Engine::Material::setDiffuse(const XMFLOAT3 &color)
{
	_mat.diffuse = color;
	_matBuffer->updateStoreSub(&_mat);
}

void Engine::Material::setSpecular(const XMFLOAT3 &color)
{
	_mat.specular = color;
	_matBuffer->updateStoreSub(&_mat);
}

void Engine::Material::setAmbient(const XMFLOAT3 &color)
{
	_mat.ambient = color;
	_matBuffer->updateStoreSub(&_mat);
}

void Engine::Material::setEmissive(const XMFLOAT3 &color)
{
	_mat.emissive = color;
	_matBuffer->updateStoreSub(&_mat);
}

void Engine::Material::setShininess(const FLOAT &shininess)
{
	_mat.shininess = shininess;
	_matBuffer->updateStoreSub(&_mat);
}

void Engine::Material::setOpacity(const FLOAT &opacity)
{
	_mat.opacity = opacity;
	_matBuffer->updateStoreSub(&_mat);
}

void Engine::Material::setDiffuseTexture(Texture2D *tex)
{
	_diffuseTexture = tex;

	if (_diffuseTexture)
		_state.hasDiffuseTexture = TRUE;
	else
		_state.hasDiffuseTexture = FALSE;

	_stateBuffer->updateStoreSub(&_state);
}

void Engine::Material::setSpecularTexture(Texture2D *tex)
{
	_specularTexture = tex;

	if (_diffuseTexture)
		_state.hasSpecularTexture = TRUE;
	else
		_state.hasSpecularTexture = FALSE;

	_stateBuffer->updateStoreSub(&_state);
}

void Engine::Material::setAmbientTexture(Texture2D *tex)
{
	_ambientTexture = tex;

	if (_diffuseTexture)
		_state.hasAmbientTexture = TRUE;
	else
		_state.hasAmbientTexture = FALSE;

	_stateBuffer->updateStoreSub(&_state);
}

void Engine::Material::setEmissiveTexture(Texture2D *tex)
{
	_emissiveTexture = tex;

	if (_diffuseTexture)
		_state.hasEmissiveTexture = TRUE;
	else
		_state.hasEmissiveTexture = FALSE;

	_stateBuffer->updateStoreSub(&_state);
}

void Engine::Material::setShininessTexture(Texture2D *tex)
{
	_shininessTexture = tex;

	if (_diffuseTexture)
		_state.hasShininessTexture = TRUE;
	else
		_state.hasShininessTexture = FALSE;

	_stateBuffer->updateStoreSub(&_state);
}

void Engine::Material::setOpacityTexture(Texture2D *tex)
{
	_opacityTexture = tex;

	if (_diffuseTexture)
		_state.hasOpacityTexture = TRUE;
	else
		_state.hasOpacityTexture = FALSE;

	_stateBuffer->updateStoreSub(&_state);
}

void Engine::Material::setBumpMap(Texture2D *map)
{
	_bumpMap = map;

	if (_diffuseTexture)
		_state.hasBumpMap = TRUE;
	else
		_state.hasBumpMap = FALSE;

	_stateBuffer->updateStoreSub(&_state);
}

void Engine::Material::setNormalMap(Texture2D *map)
{
	_normalMap = map;

	if (_diffuseTexture)
		_state.hasNormalMap = TRUE;
	else
		_state.hasNormalMap = FALSE;

	_stateBuffer->updateStoreSub(&_state);
}

void Engine::Material::setDisplacementMap(Texture2D *map)
{
	_displacementMap = map;

	if (_diffuseTexture)
		_state.hasDisplacementMap = TRUE;
	else
		_state.hasDisplacementMap = FALSE;

	_stateBuffer->updateStoreSub(&_state);
}

void Engine::Material::setLightMap(Texture2D *map)
{
	_lightMap = map;

	if (_diffuseTexture)
		_state.hasLightMap = TRUE;
	else
		_state.hasLightMap = FALSE;

	_stateBuffer->updateStoreSub(&_state);
}

XMFLOAT3 Engine::Material::getDiffuse(void) const
{
	return _mat.diffuse;
}

XMFLOAT3 Engine::Material::getSpecular(void) const
{
	return _mat.specular;
}

XMFLOAT3 Engine::Material::getAmbient(void) const
{
	return _mat.ambient;
}

XMFLOAT3 Engine::Material::getEmissive(void) const
{
	return _mat.emissive;
}

FLOAT Engine::Material::getShininess(void) const
{
	return _mat.shininess;
}

FLOAT Engine::Material::getOpacity(void) const
{
	return _mat.opacity;
}

Engine::Texture2D *Engine::Material::getDiffuseTexture(void) const
{
	return _diffuseTexture;
}

Engine::Texture2D *Engine::Material::getSpecularTexture(void) const
{
	return _specularTexture;
}

Engine::Texture2D *Engine::Material::getAmbientTexture(void) const
{
	return _ambientTexture;
}

Engine::Texture2D *Engine::Material::getEmissiveTexture(void) const
{
	return _emissiveTexture;
}

Engine::Texture2D *Engine::Material::getShininessTexture(void) const
{
	return _shininessTexture;
}

Engine::Texture2D *Engine::Material::getOpacityTexture(void) const
{
	return _opacityTexture;
}

Engine::Texture2D *Engine::Material::getBumpMap(void) const
{
	return _bumpMap;
}

Engine::Texture2D *Engine::Material::getNormalMap(void) const
{
	return _normalMap;
}

Engine::Texture2D *Engine::Material::getDisplacementMap(void) const
{
	return _displacementMap;
}

Engine::Texture2D *Engine::Material::getLightMap(void) const
{
	return _lightMap;
}

BOOL Engine::Material::hasDiffuseTexture(void) const
{
	return _state.hasDiffuseTexture;
}

BOOL Engine::Material::hasSpecularTexture(void) const
{
	return _state.hasSpecularTexture;
}

BOOL Engine::Material::hasAmbientTexture(void) const
{
	return _state.hasAmbientTexture;
}

BOOL Engine::Material::hasEmissiveTexture(void) const
{
	return _state.hasEmissiveTexture;
}

BOOL Engine::Material::hasShininessTexture(void) const
{
	return _state.hasShininessTexture;
}

BOOL Engine::Material::hasOpacityTexture(void) const
{
	return _state.hasOpacityTexture;
}

BOOL Engine::Material::hasBumpMap(void) const
{
	return _state.hasBumpMap;
}

BOOL Engine::Material::hasNormalMap(void) const
{
	return _state.hasNormalMap;
}

BOOL Engine::Material::hasDisplacementMap(void) const
{
	return _state.hasDisplacementMap;
}

BOOL Engine::Material::hasLightMap(void) const
{
	return _state.hasLightMap;
}

ID3D11Buffer *Engine::Material::getMatBuffer(void) const
{
	return _matBuffer->getBuffer();
}

ID3D11Buffer *Engine::Material::getStateBuffer(void) const
{
	return _stateBuffer->getBuffer();
}