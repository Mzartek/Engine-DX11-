#include <Engine/Object.hpp>

engine::Object::Object(void)
{
	UINT i;
	_pTexture = NULL;
	_pSampler = NULL;
	_pVertexBuffer = NULL;
	_pIndexBuffer = NULL;
	_pVertexLayout = NULL;
	_pConstantBuffer = NULL;
	_material= (struct uniform *)_aligned_malloc(sizeof *_material, 16);
	_program = NULL;

	for (i = 0; i<4; i++)
	{
		_material->ambient[i] = 1.0;
		_material->diffuse[i] = 1.0;
		_material->specular[i] = 1.0;
	}
	shininess[0] = 1.0;
}

engine::Object::~Object(void)
{
	if (_pTexture) 
		_pTexture->Release();
	if (_pSampler) 
		_pSampler->Release();
	if (_pVertexBuffer) 
		_pVertexBuffer->Release();
	if (_pVertexLayout) 
		_pVertexLayout->Release();
	if (_pIndexBuffer) 
		_pIndexBuffer->Release();
	if (_pConstantBuffer) 
		_pConstantBuffer->Release();

	_aligned_free(_material);
}

HRESULT engine::Object::setShaderProgram(ShaderProgram *program, ID3D11Device *pd3dDevice)
{
	HRESULT hr;

	_program = program;

	// Create and set the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "IN_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "IN_TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "IN_NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = ARRAYSIZE(layout);
	hr = pd3dDevice->CreateInputLayout(layout, numElements, _program->getVSBlob()->GetBufferPointer(),
		_program->getVSBlob()->GetBufferSize(), &_pVertexLayout);
	if (FAILED(hr))
		return hr;

	// Create uniform
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(*_material);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = pd3dDevice->CreateBuffer(&bd, NULL, &_pConstantBuffer);

	return hr;
}

void engine::Object::setTexture(ID3D11ShaderResourceView *pTexture, ID3D11SamplerState *pSampler)
{
	_pTexture = pTexture;
	_pSampler = pSampler;
}

void engine::Object::setAmbient(const FLOAT &x, const FLOAT &y, const FLOAT &z, const FLOAT &w)
{
	_material->ambient[0] = x;
	_material->ambient[1] = y;
	_material->ambient[2] = z;
	_material->ambient[3] = w;
}

void engine::Object::setDiffuse(const FLOAT &x, const FLOAT &y, const FLOAT &z, const FLOAT &w)
{
	_material->diffuse[0] = x;
	_material->diffuse[1] = y;
	_material->diffuse[2] = z;
	_material->diffuse[3] = w;
}

void engine::Object::setSpecular(const FLOAT &x, const FLOAT &y, const FLOAT &z, const FLOAT &w)
{
	_material->specular[0] = x;
	_material->specular[1] = y;
	_material->specular[2] = z;
	_material->specular[3] = w;
}

void engine::Object::setShininess(const FLOAT &x)
{
	shininess[0] = x;
}

FLOAT engine::Object::getTransparency(void)
{
	return _material->diffuse[3];
}

#define BUFFER_OFFSET(i) ((CHAR *)NULL + i)

HRESULT engine::Object::load(const UINT &sizeVertexArray, const FLOAT *vertexArray, 
	const UINT &sizeIndexArray, const UINT *indexArray,
	ID3D11Device *pd3dDevice)
{
	HRESULT hr;
	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA InitData;

	_numElement = sizeIndexArray / (UINT)sizeof(UINT);

	// Create vertex buffer
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeVertexArray;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertexArray;
	hr = pd3dDevice->CreateBuffer(&bd, &InitData, &_pVertexBuffer);
	if (FAILED(hr))
		return hr;

	// Create index buffer
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeIndexArray;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = indexArray;
	hr = pd3dDevice->CreateBuffer(&bd, &InitData, &_pIndexBuffer);

	return hr;
}

#undef BUFFER_OFFSET

void engine::Object::display(Window *win) const
{
	UINT stride = 32;
	UINT offset = 0;

	if(_program == NULL)
	{
		MessageBox(NULL, "You need to set the ShaderProgram before!", "Error", MB_OK);
		exit(1);
	}

	win->getImmediateContext()->VSSetShader(_program->getVertexShader(), NULL, 0);
	win->getImmediateContext()->PSSetShader(_program->getPixelShader(), NULL, 0);
	win->getImmediateContext()->IASetInputLayout(_pVertexLayout);

	win->getImmediateContext()->UpdateSubresource(_pConstantBuffer, 0, NULL, _material, 0, 0);
	win->getImmediateContext()->PSSetConstantBuffers(1, 1, &_pConstantBuffer);
	win->getImmediateContext()->PSSetShaderResources(0, 1, &_pTexture);
	win->getImmediateContext()->PSSetSamplers(0, 1, &_pSampler);

	win->getImmediateContext()->IASetVertexBuffers(0, 1, &_pVertexBuffer, &stride, &offset);
	win->getImmediateContext()->IASetIndexBuffer(_pIndexBuffer, DXGI_FORMAT_R32_UINT, offset);
	win->getImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	win->getImmediateContext()->DrawIndexed(_numElement, 0, 0);
}

/*void engine::Object::displayOnGBuffer(GBuffer *g) const
{	
	if(g == NULL)
	{
		std::cerr <<"Bad GBuffer!" << std::endl;
		return;
	}
	
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, g->getIdFBO());
	glUseProgram(g->getProgramId());
	glBindVertexArray(_idVAO);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _idTexture);
	glUniform1i(g->getColorTextureLocation(), 0);
	
	glUniform1fv(g->getShininessLocation(), 1, _shininess);
        
	glDrawElements(GL_TRIANGLES, _numElement, GL_UNSIGNED_INT, 0);
	
	glBindVertexArray(0);
	glUseProgram(0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void engine::Object::displayShadow(Light *l) const
{
	if(l == NULL)
	{
		std::cerr << "Bad Light!" << std::endl;
		return;
	}
	if(l->getShadowMap() == NULL)
	{
		std::cerr << "You need to config the ShadowMap before!" << std::endl;
		return;
	}
	
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, l->getShadowMap()->getIdFBO());
	glUseProgram(l->getShadowMap()->getProgramId());
	glBindVertexArray(_idVAO);
	
	glDrawElements(GL_TRIANGLES, _numElement, GL_UNSIGNED_INT, 0);
	
	glBindVertexArray(0);
	glUseProgram(0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}*/

int engine::comparObject(const void *p1, const void *p2)
{
	Object **obj1 = (engine::Object **)p1;
	Object **obj2 = (engine::Object **)p2;

	if ((*obj1)->_material->diffuse[3] < (*obj2)->_material->diffuse[3])
		return 1;
	if ((*obj1)->_material->diffuse[3] > (*obj2)->_material->diffuse[3])
		return -1;
	return 0;
}
