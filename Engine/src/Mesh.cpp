#include <Engine/Mesh.hpp>
#include <Engine/Texture2D.hpp>
#include <Engine/TextureCube.hpp>
#include <Engine/Buffer.hpp>
#include <Engine/Material.hpp>

Engine::Mesh::Mesh(void)
	: _numElement(0), _material(NULL)
{
	_vertexBuffer = new Buffer;
	_indexBuffer = new Buffer;

	ZeroMemory(_pshr, sizeof _pshr);
	ZeroMemory(_psam, sizeof _psam);
}

Engine::Mesh::~Mesh(void)
{
	delete _vertexBuffer;
	delete _indexBuffer;
}

void Engine::Mesh::setMaterial(Material *material)
{
	ZeroMemory(_pshr, sizeof _pshr);
	ZeroMemory(_psam, sizeof _psam);

	_material = material;

	if (_material->hasDiffuseTexture())
	{
		_pshr[0] = _material->getDiffuseTexture()->getShaderResourceView();
		_psam[0] = _material->getDiffuseTexture()->getSamplerState();
	}

	if (_material->hasSpecularTexture())
	{
		_pshr[1] = _material->getSpecularTexture()->getShaderResourceView();
		_psam[1] = _material->getSpecularTexture()->getSamplerState();
	}

	if (_material->hasAmbientTexture())
	{
		_pshr[2] = _material->getAmbientTexture()->getShaderResourceView();
		_psam[2] = _material->getAmbientTexture()->getSamplerState();
	}

	if (_material->hasEmissiveTexture())
	{
		_pshr[3] = _material->getEmissiveTexture()->getShaderResourceView();
		_psam[3] = _material->getEmissiveTexture()->getSamplerState();
	}

	if (_material->hasShininessTexture())
	{
		_pshr[4] = _material->getShininessTexture()->getShaderResourceView();
		_psam[4] = _material->getShininessTexture()->getSamplerState();
	}

	if (_material->hasOpacityTexture())
	{
		_pshr[5] = _material->getOpacityTexture()->getShaderResourceView();
		_psam[5] = _material->getOpacityTexture()->getSamplerState();
	}

	if (_material->hasBumpMap())
	{
		_pshr[6] = _material->getBumpMap()->getShaderResourceView();
		_psam[6] = _material->getBumpMap()->getSamplerState();
	}

	if (_material->hasNormalMap())
	{
		_pshr[7] = _material->getNormalMap()->getShaderResourceView();
		_psam[7] = _material->getNormalMap()->getSamplerState();
	}

	if (_material->hasDisplacementMap())
	{
		_pshr[8] = _material->getDisplacementMap()->getShaderResourceView();
		_psam[8] = _material->getDisplacementMap()->getSamplerState();
	}

	if (_material->hasLightMap())
	{
		_pshr[9] = _material->getLightMap()->getShaderResourceView();
		_psam[9] = _material->getLightMap()->getSamplerState();
	}
}

Engine::Material *Engine::Mesh::getMaterial(void) const
{
	return _material;
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
	DeviceContext->PSSetShaderResources(0, ARRAYSIZE(_pshr), _pshr);
	DeviceContext->PSSetSamplers(0, ARRAYSIZE(_psam), _psam);

	ID3D11Buffer *buf[] =
	{
		_material->getMatBuffer(),
		_material->getStateBuffer(),
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
	DeviceContext->PSSetShaderResources(0, ARRAYSIZE(_pshr), _pshr);
	DeviceContext->PSSetSamplers(0, ARRAYSIZE(_psam), _psam);

	ID3D11ShaderResourceView *pcube_shr = cubeTex->getShaderResourceView();
	ID3D11SamplerState *pcube_sam = cubeTex->getSamplerState();

	DeviceContext->PSSetShaderResources(ARRAYSIZE(_pshr), 1, &pcube_shr);
	DeviceContext->PSSetSamplers(ARRAYSIZE(_psam), 1, &pcube_sam);

	ID3D11Buffer *buf[] =
	{
		_material->getMatBuffer(),
		_material->getStateBuffer(),
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
	DeviceContext->PSSetShaderResources(0, 1, _pshr);
	DeviceContext->PSSetSamplers(0, 1, _psam);

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
	if (first->_material == NULL)
		return false;
	if (first->_material->getOpacity() < second->_material->getOpacity())
		return true;
	return false;
}
