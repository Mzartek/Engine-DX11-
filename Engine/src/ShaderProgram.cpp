#include <Engine/ShaderProgram.hpp>

static void CompileShaderFromFile(LPCWSTR szFileName, LPCSTR szEntryPoint, std::string szShaderModel, ID3DBlob **ppBlobOut)
{
	HRESULT hr;
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
	ID3DBlob *pErrorBlob = NULL;

	hr = D3DCompileFromFile(szFileName, NULL, NULL, szEntryPoint, szShaderModel.c_str(),
		dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
	if (FAILED(hr))
	{
		if (pErrorBlob)
		{
			MessageBox(NULL, (char *)pErrorBlob->GetBufferPointer(), "ShaderProgram", MB_OK);
			pErrorBlob->Release();
		}
		MessageBoxW(NULL, szFileName, L"ShaderProgram", MB_OK);
		exit(1);
	}
	if (pErrorBlob) 
		pErrorBlob->Release();
}

engine::ShaderProgram::ShaderProgram(void)
{
	_pVertexShader = NULL;
	_pHullShader = NULL;
	_pDomainShader = NULL;
	_pGeometryShader = NULL;
	_pPixelShader = NULL;
	_pBlob = NULL;
}

engine::ShaderProgram::~ShaderProgram(void)
{
	if (_pVertexShader) 
		_pVertexShader->Release();
	if (_pHullShader)
		_pHullShader->Release();
	if (_pDomainShader)
		_pDomainShader->Release();
	if (_pGeometryShader)
		_pGeometryShader->Release();
	if (_pPixelShader) 
		_pPixelShader->Release();
	if (_pBlob)
		_pBlob->Release();
}

void engine::ShaderProgram::loadProgram(LPCWSTR vs, LPCWSTR hs, LPCWSTR ds, LPCWSTR gs, LPCWSTR ps, ID3D11Device *pDevice)
{
	HRESULT hr;
	ID3DBlob *pTmpBlob;

	if (_pVertexShader)
	{
		_pVertexShader->Release();
		_pVertexShader = NULL;
	}
	if (_pHullShader)
	{
		_pHullShader->Release();
		_pHullShader = NULL;
	}
	if (_pDomainShader)
	{
		_pDomainShader->Release();
		_pDomainShader = NULL;
	}
	if (_pGeometryShader)
	{
		_pGeometryShader->Release();
		_pGeometryShader = NULL;
	}
	if (_pPixelShader)
	{
		_pPixelShader->Release();
		_pPixelShader = NULL;
	}
	if (_pBlob)
	{
		_pBlob->Release();
		_pBlob = NULL;
	}

	// Compile and create the Vertex Shader
	if (vs == NULL)
	{
		MessageBox(NULL, "Need a Vertex Shader for ShaderProgram", "ShaderProgram", NULL);
		exit(1);
	}
	CompileShaderFromFile(vs, "main", "vs_" + engine::EngineShaderLevel, &_pBlob);
	hr = pDevice->CreateVertexShader(_pBlob->GetBufferPointer(), _pBlob->GetBufferSize(), NULL, &_pVertexShader);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed to create Vertex Shader.", "ShaderProgram", MB_OK);
		exit(1);
	}

	// Compile and create the Hull Shader
	if (hs != NULL)
	{
		CompileShaderFromFile(hs, "main", "hs_" + engine::EngineShaderLevel, &pTmpBlob);
		hr = pDevice->CreateHullShader(pTmpBlob->GetBufferPointer(), pTmpBlob->GetBufferSize(), NULL, &_pHullShader);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to create Hull Shader.", "ShaderProgram", MB_OK);
			exit(1);
		}
		pTmpBlob->Release();
	}

	// Compile and create the Domain Shader
	if (ds != NULL)
	{
		CompileShaderFromFile(ds, "main", "ds_" + engine::EngineShaderLevel, &pTmpBlob);
		hr = pDevice->CreateDomainShader(pTmpBlob->GetBufferPointer(), pTmpBlob->GetBufferSize(), NULL, &_pDomainShader);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to create Domain Shader.", "ShaderProgram", MB_OK);
			exit(1);
		}
		pTmpBlob->Release();
	}

	// Compile and create the Geometry Shader
	if (gs != NULL)
	{
		CompileShaderFromFile(gs, "main", "gs_" + engine::EngineShaderLevel, &pTmpBlob);
		hr = pDevice->CreateGeometryShader(pTmpBlob->GetBufferPointer(), pTmpBlob->GetBufferSize(), NULL, &_pGeometryShader);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to create Geometry Shader.", "ShaderProgram", MB_OK);
			exit(1);
		}
		pTmpBlob->Release();
	}

	// Compile and create the Pixel Shader
	if (ps != NULL)
	{
		CompileShaderFromFile(ps, "main", "ps_" + engine::EngineShaderLevel, &pTmpBlob);
		hr = pDevice->CreatePixelShader(pTmpBlob->GetBufferPointer(), pTmpBlob->GetBufferSize(), NULL, &_pPixelShader);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to create Pixel Shader.", "ShaderProgram", MB_OK);
			exit(1);
		}
		pTmpBlob->Release();
	}
}

ID3D11VertexShader *engine::ShaderProgram::getVertexShader(void) const
{
	return _pVertexShader;
}

ID3D11HullShader *engine::ShaderProgram::getHullShader(void) const
{
	return _pHullShader;
}

ID3D11DomainShader *engine::ShaderProgram::getDomainShader(void) const
{
	return _pDomainShader;
}

ID3D11GeometryShader *engine::ShaderProgram::getGeometryShader(void) const
{
	return _pGeometryShader;
}

ID3D11PixelShader *engine::ShaderProgram::getPixelShader(void) const
{
	return _pPixelShader;
}

void *engine::ShaderProgram::getEntryBufferPointer(void)
{
	return _pBlob->GetBufferPointer();
}

SIZE_T engine::ShaderProgram::getEntryBytecodeLength(void)
{
	return _pBlob->GetBufferSize();
}