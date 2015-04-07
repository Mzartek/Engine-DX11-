#include <Engine/Mesh.hpp>
#include <Engine/Texture2D.hpp>
#include <Engine/TextureCube.hpp>
#include <Engine/Buffer.hpp>
#include <Engine/Material.hpp>

Engine::Mesh::Mesh(void)
	: _numElement(0), _materia(NULL)
{
	_colorTexture = new Texture2D;
	_NMTexture = new Texture2D;
	_vertexBuffer = new Buffer;
	_indexBuffer = new Buffer;
	_materialBuffer = new Buffer;

	_materialBuffer->createStore(D3D11_BIND_CONSTANT_BUFFER, NULL, sizeof _material, D3D11_USAGE_DYNAMIC);

	ZeroMemory(_pshr, sizeof _pshr);
	ZeroMemory(_psam, sizeof _psam);
}

Engine::Mesh::~Mesh(void)
{
	delete _colorTexture;
	delete _NMTexture;
	delete _vertexBuffer;
	delete _indexBuffer;
	delete _materialBuffer;
}

void Engine::Mesh::loadColorTexture(const CHAR *path)
{
	_colorTexture->loadFromFile(path);
}

void Engine::Mesh::loadNMTexture(const CHAR *path)
{
	_NMTexture->loadFromFile(path);
}

void Engine::Mesh::setAmbient(const XMVECTOR &ambient)
{
	XMStoreFloat4(&_material.ambient, ambient);
}

void Engine::Mesh::setDiffuse(const XMVECTOR &diffuse)
{
	XMStoreFloat4(&_material.diffuse, diffuse);
}

void Engine::Mesh::setSpecular(const XMVECTOR &specular)
{
	XMStoreFloat4(&_material.specular, specular);
}

void Engine::Mesh::setShininess(const FLOAT &shininess)
{
	_material.shininess = shininess;
}

void Engine::Mesh::setMaterial(Material *material)
{
	ZeroMemory(_pshr, sizeof _pshr);
	ZeroMemory(_psam, sizeof _psam);

	_materia = material;

	if (_materia->hasDiffuseTexture())
	{
		_pshr[0] = _materia->getDiffuseTexture()->getShaderResourceView();
		_psam[0] = _materia->getDiffuseTexture()->getSamplerState();
	}

	if (_materia->hasSpecularTexture())
	{
		_pshr[1] = _materia->getSpecularTexture()->getShaderResourceView();
		_psam[1] = _materia->getSpecularTexture()->getSamplerState();
	}

	if (_materia->hasAmbientTexture())
	{
		_pshr[2] = _materia->getAmbientTexture()->getShaderResourceView();
		_psam[2] = _materia->getAmbientTexture()->getSamplerState();
	}

	if (_materia->hasEmissiveTexture())
	{
		_pshr[3] = _materia->getEmissiveTexture()->getShaderResourceView();
		_psam[3] = _materia->getEmissiveTexture()->getSamplerState();
	}

	if (_materia->hasShininessTexture())
	{
		_pshr[4] = _materia->getShininessTexture()->getShaderResourceView();
		_psam[4] = _materia->getShininessTexture()->getSamplerState();
	}

	if (_materia->hasOpacityTexture())
	{
		_pshr[5] = _materia->getOpacityTexture()->getShaderResourceView();
		_psam[5] = _materia->getOpacityTexture()->getSamplerState();
	}

	if (_materia->hasBumpMap())
	{
		_pshr[6] = _materia->getBumpMap()->getShaderResourceView();
		_psam[6] = _materia->getBumpMap()->getSamplerState();
	}

	if (_materia->hasNormalMap())
	{
		_pshr[7] = _materia->getNormalMap()->getShaderResourceView();
		_psam[7] = _materia->getNormalMap()->getSamplerState();
	}

	if (_materia->hasDisplacementMap())
	{
		_pshr[8] = _materia->getDisplacementMap()->getShaderResourceView();
		_psam[8] = _materia->getDisplacementMap()->getSamplerState();
	}

	if (_materia->hasLightMap())
	{
		_pshr[9] = _materia->getLightMap()->getShaderResourceView();
		_psam[9] = _materia->getLightMap()->getSamplerState();
	}
}

FLOAT Engine::Mesh::getTransparency(void)
{
	return _material.diffuse.w;
}

void Engine::Mesh::load(const UINT &numVertex, const Vertex *vertexArray,
	const UINT &numIndex, const UINT *indexArray)
{
	_numElement = numIndex;

	// Create vertex buffer
	_vertexBuffer->createStore(D3D11_BIND_VERTEX_BUFFER, vertexArray, numVertex * sizeof *vertexArray, D3D11_USAGE_IMMUTABLE);

	// Create index buffer
	_indexBuffer->createStore(D3D11_BIND_INDEX_BUFFER, indexArray, numIndex * sizeof *indexArray, D3D11_USAGE_IMMUTABLE);
}

void Engine::Mesh::display(void) const
{
	ID3D11ShaderResourceView *pshr[] =
	{
		_colorTexture->getShaderResourceView(),
		_NMTexture->getShaderResourceView(),
	};
	DeviceContext->PSSetShaderResources(0, ARRAYSIZE(pshr), pshr);
	ID3D11SamplerState *psam[] =
	{
		_colorTexture->getSamplerState(),
		_NMTexture->getSamplerState(),
	};
	DeviceContext->PSSetSamplers(0, ARRAYSIZE(psam), psam);

	_materialBuffer->updateStoreMap(&_material);
	ID3D11Buffer *buf[] =
	{
		_materialBuffer->getBuffer(),
	};
	DeviceContext->PSSetConstantBuffers(0, ARRAYSIZE(buf), buf);

	UINT stride = 11 * sizeof(FLOAT), offset = 0;
	ID3D11Buffer *drawBuf[] =
	{
		_vertexBuffer->getBuffer(),
		_indexBuffer->getBuffer(),
	};
	DeviceContext->IASetVertexBuffers(0, 1, &drawBuf[0], &stride, &offset);
	DeviceContext->IASetIndexBuffer(drawBuf[1], DXGI_FORMAT_R32_UINT, offset);
	DeviceContext->DrawIndexed(_numElement, 0, 0);
}

void Engine::Mesh::display(TextureCube *cubeTex) const
{
	ID3D11ShaderResourceView *pshr[] =
	{
		_colorTexture->getShaderResourceView(),
		_NMTexture->getShaderResourceView(),
		cubeTex->getShaderResourceView(),
	};
	DeviceContext->PSSetShaderResources(0, ARRAYSIZE(pshr), pshr);
	ID3D11SamplerState *psam[] =
	{
		_colorTexture->getSamplerState(),
		_NMTexture->getSamplerState(),
		cubeTex->getSamplerState(),
	};
	DeviceContext->PSSetSamplers(0, ARRAYSIZE(psam), psam);

	_materialBuffer->updateStoreMap(&_material);
	ID3D11Buffer *buf[] =
	{
		_materialBuffer->getBuffer(),
	};
	DeviceContext->PSSetConstantBuffers(0, ARRAYSIZE(buf), buf);

	UINT stride = 11 * sizeof(FLOAT), offset = 0;
	ID3D11Buffer *drawBuf[] =
	{
		_vertexBuffer->getBuffer(),
		_indexBuffer->getBuffer(),
	};
	DeviceContext->IASetVertexBuffers(0, 1, &drawBuf[0], &stride, &offset);
	DeviceContext->IASetIndexBuffer(drawBuf[1], DXGI_FORMAT_R32_UINT, offset);
	DeviceContext->DrawIndexed(_numElement, 0, 0);
}

void Engine::Mesh::displayShadow(void) const
{
	ID3D11ShaderResourceView *pshr[] =
	{
		_colorTexture->getShaderResourceView(),
	};
	DeviceContext->PSSetShaderResources(0, ARRAYSIZE(pshr), pshr);
	ID3D11SamplerState *psam[] =
	{
		_colorTexture->getSamplerState(),
	};
	DeviceContext->PSSetSamplers(0, ARRAYSIZE(psam), psam);

	UINT stride = 11 * sizeof(FLOAT), offset = 0;
	ID3D11Buffer *drawBuf[] =
	{
		_vertexBuffer->getBuffer(),
		_indexBuffer->getBuffer(),
	};
	DeviceContext->IASetVertexBuffers(0, 1, &drawBuf[0], &stride, &offset);
	DeviceContext->IASetIndexBuffer(drawBuf[1], DXGI_FORMAT_R32_UINT, offset);
	DeviceContext->DrawIndexed(_numElement, 0, 0);
}

bool Engine::CompareMesh::operator()(const Mesh *first, const Mesh *second)
{
	if (first->_material.diffuse.z > second->_material.diffuse.z)
		return true;
	return false;
}
