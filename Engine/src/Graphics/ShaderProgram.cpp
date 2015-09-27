#include <Engine\Graphics\ShaderProgram.h>

void Engine::Graphics::ShaderProgram::compiledShaderFromFile(const LPCWSTR &szFileName, const std::string &szEntryPoint, const std::string &szShaderModel, ID3DBlob **ppBlobOut)
{
	HRESULT hr;
	ID3DBlob *pErrorBlob = NULL;

	hr = D3DCompileFromFile(szFileName, NULL, NULL, szEntryPoint.c_str(), szShaderModel.c_str(), D3DCOMPILE_ENABLE_STRICTNESS, 0, ppBlobOut, &pErrorBlob);
	if (FAILED(hr))
	{
		if (pErrorBlob)
		{
			std::exception ex(reinterpret_cast<const char *>(pErrorBlob->GetBufferPointer()));
			pErrorBlob->Release();
			Logger::Instance().error(L"ShaderProgram", L"Error while compiling the shader \"" + std::wstring(szFileName) + L'\"', ex);

			throw ex;
		}
		else
		{
			Logger::Instance().error(L"ShaderProgram", L"Error while compiling the shader \"" + std::wstring(szFileName) + L'\"');

			throw std::exception();
		}
	}
	if (pErrorBlob) pErrorBlob->Release();
}

Engine::Graphics::ShaderProgram::ShaderProgram(const LPCWSTR &vs, const LPCWSTR &hs, const LPCWSTR &ds, const LPCWSTR &gs, const LPCWSTR &ps)
{
	ID3DBlob *pTmpBlob;

	// Compile and create the Vertex Shader
	if (vs != NULL)
	{
		this->compiledShaderFromFile(vs, "main", "vs_" + GraphicsRenderer::Instance().getShaderLevel(), &_pBlob);
		GraphicsRenderer::Instance().getDevice()->CreateVertexShader(_pBlob->GetBufferPointer(), _pBlob->GetBufferSize(), NULL, &_pVertexShader);
	}

	// Compile and create the Hull Shader
	if (hs != NULL)
	{
		this->compiledShaderFromFile(hs, "main", "hs_" + GraphicsRenderer::Instance().getShaderLevel(), &pTmpBlob);
		GraphicsRenderer::Instance().getDevice()->CreateHullShader(pTmpBlob->GetBufferPointer(), pTmpBlob->GetBufferSize(), NULL, &_pHullShader);
		pTmpBlob->Release();
	}

	// Compile and create the Domain Shader
	if (ds != NULL)
	{
		this->compiledShaderFromFile(ds, "main", "ds_" + GraphicsRenderer::Instance().getShaderLevel(), &pTmpBlob);
		GraphicsRenderer::Instance().getDevice()->CreateDomainShader(pTmpBlob->GetBufferPointer(), pTmpBlob->GetBufferSize(), NULL, &_pDomainShader);
		pTmpBlob->Release();
	}

	// Compile and create the Geometry Shader
	if (gs != NULL)
	{
		this->compiledShaderFromFile(gs, "main", "gs_" + GraphicsRenderer::Instance().getShaderLevel(), &pTmpBlob);
		GraphicsRenderer::Instance().getDevice()->CreateGeometryShader(pTmpBlob->GetBufferPointer(), pTmpBlob->GetBufferSize(), NULL, &_pGeometryShader);
		pTmpBlob->Release();
	}

	// Compile and create the Pixel Shader
	if (ps != NULL)
	{
		this->compiledShaderFromFile(ps, "main", "ps_" + GraphicsRenderer::Instance().getShaderLevel(), &pTmpBlob);
		GraphicsRenderer::Instance().getDevice()->CreatePixelShader(pTmpBlob->GetBufferPointer(), pTmpBlob->GetBufferSize(), NULL, &_pPixelShader);
		pTmpBlob->Release();
	}
}

Engine::Graphics::ShaderProgram::ShaderProgram(const LPCWSTR &vs, const LPCWSTR &hs, const LPCWSTR &ds, const LPCWSTR &gs, const LPCWSTR &ps, const D3D11_SO_DECLARATION_ENTRY * pDecl, const UINT & count)
{
	ID3DBlob *pTmpBlob;

	// Compile and create the Vertex Shader
	if (vs != NULL)
	{
		this->compiledShaderFromFile(vs, "main", "vs_" + GraphicsRenderer::Instance().getShaderLevel(), &_pBlob);
		GraphicsRenderer::Instance().getDevice()->CreateVertexShader(_pBlob->GetBufferPointer(), _pBlob->GetBufferSize(), NULL, &_pVertexShader);
	}

	// Compile and create the Hull Shader
	if (hs != NULL)
	{
		this->compiledShaderFromFile(hs, "main", "hs_" + GraphicsRenderer::Instance().getShaderLevel(), &pTmpBlob);
		GraphicsRenderer::Instance().getDevice()->CreateHullShader(pTmpBlob->GetBufferPointer(), pTmpBlob->GetBufferSize(), NULL, &_pHullShader);
		pTmpBlob->Release();
	}

	// Compile and create the Domain Shader
	if (ds != NULL)
	{
		this->compiledShaderFromFile(ds, "main", "ds_" + GraphicsRenderer::Instance().getShaderLevel(), &pTmpBlob);
		GraphicsRenderer::Instance().getDevice()->CreateDomainShader(pTmpBlob->GetBufferPointer(), pTmpBlob->GetBufferSize(), NULL, &_pDomainShader);
		pTmpBlob->Release();
	}

	// Compile and create the Geometry Shader
	if (gs != NULL)
	{
		this->compiledShaderFromFile(gs, "main", "gs_" + GraphicsRenderer::Instance().getShaderLevel(), &pTmpBlob);
		GraphicsRenderer::Instance().getDevice()->CreateGeometryShaderWithStreamOutput(pTmpBlob->GetBufferPointer(), pTmpBlob->GetBufferSize(), pDecl, count, NULL, 0, 0, NULL, &_pGeometryShader);
		pTmpBlob->Release();
	}

	// Compile and create the Pixel Shader
	if (ps != NULL)
	{
		this->compiledShaderFromFile(ps, "main", "ps_" + GraphicsRenderer::Instance().getShaderLevel(), &pTmpBlob);
		GraphicsRenderer::Instance().getDevice()->CreatePixelShader(pTmpBlob->GetBufferPointer(), pTmpBlob->GetBufferSize(), NULL, &_pPixelShader);
		pTmpBlob->Release();
	}
}

Engine::Graphics::ShaderProgram::~ShaderProgram(void)
{
	if (_pVertexShader) _pVertexShader->Release();
	if (_pHullShader) _pHullShader->Release();
	if (_pDomainShader) _pDomainShader->Release();
	if (_pGeometryShader) _pGeometryShader->Release();
	if (_pPixelShader) _pPixelShader->Release();
	if (_pBlob) _pBlob->Release();
}

ID3D11VertexShader *Engine::Graphics::ShaderProgram::getVertexShader(void) const
{
	return _pVertexShader;
}

ID3D11HullShader *Engine::Graphics::ShaderProgram::getHullShader(void) const
{
	return _pHullShader;
}

ID3D11DomainShader *Engine::Graphics::ShaderProgram::getDomainShader(void) const
{
	return _pDomainShader;
}

ID3D11GeometryShader *Engine::Graphics::ShaderProgram::getGeometryShader(void) const
{
	return _pGeometryShader;
}

ID3D11PixelShader *Engine::Graphics::ShaderProgram::getPixelShader(void) const
{
	return _pPixelShader;
}

void *Engine::Graphics::ShaderProgram::getEntryBufferPointer(void)
{
	return _pBlob->GetBufferPointer();
}

SIZE_T Engine::Graphics::ShaderProgram::getEntryBytecodeLength(void)
{
	return _pBlob->GetBufferSize();
}
