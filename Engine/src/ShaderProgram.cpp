#include <Engine/ShaderProgram.hpp>

extern std::string ShaderLevel;
extern ID3D11Device *Device;

static void CompileShaderFromFile(const LPCWSTR szFileName, const std::string szEntryPoint, const std::string szShaderModel, ID3DBlob **ppBlobOut)
{
	HRESULT hr;
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
	ID3DBlob *pErrorBlob = NULL;

	hr = D3DCompileFromFile(szFileName, NULL, NULL, szEntryPoint.c_str(), szShaderModel.c_str(),
		dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
	if (FAILED(hr))
	{
		if (pErrorBlob)
		{
			MessageBoxA(NULL, (char *)pErrorBlob->GetBufferPointer(), __FILE__, MB_OK);
			pErrorBlob->Release();
		}
		MessageBox(NULL, szFileName, L"File not found", MB_OK);
		exit(1);
	}
	if (pErrorBlob) 
		pErrorBlob->Release();
}

engine::ShaderProgram::ShaderProgram(void)
	: _pVertexShader(NULL), _pHullShader(NULL), _pDomainShader(NULL), _pGeometryShader(NULL), _pPixelShader(NULL),
	_pBlob(NULL)
{
}

engine::ShaderProgram::~ShaderProgram(void)
{
	if (_pVertexShader) _pVertexShader->Release();
	if (_pHullShader) _pHullShader->Release();
	if (_pDomainShader) _pDomainShader->Release();
	if (_pGeometryShader) _pGeometryShader->Release();
	if (_pPixelShader) _pPixelShader->Release();
	if (_pBlob) _pBlob->Release();
}

void engine::ShaderProgram::loadProgram(LPCWSTR vs, LPCWSTR hs, LPCWSTR ds, LPCWSTR gs, LPCWSTR ps)
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
		MessageBox(NULL, L"Need a Vertex Shader for ShaderProgram", TEXT(__FILE__), MB_OK);
		exit(1);
	}
	CompileShaderFromFile(vs, "main", "vs_" + ShaderLevel, &_pBlob);
	hr = Device->CreateVertexShader(_pBlob->GetBufferPointer(), _pBlob->GetBufferSize(), NULL, &_pVertexShader);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Failed to create Vertex Shader.", TEXT(__FILE__), MB_OK);
		exit(1);
	}

	// Compile and create the Hull Shader
	if (hs != NULL)
	{
		CompileShaderFromFile(hs, "main", "hs_" + ShaderLevel, &pTmpBlob);
		hr = Device->CreateHullShader(pTmpBlob->GetBufferPointer(), pTmpBlob->GetBufferSize(), NULL, &_pHullShader);
		if (FAILED(hr))
		{
			MessageBox(NULL, L"Failed to create Hull Shader.", TEXT(__FILE__), MB_OK);
			exit(1);
		}
		pTmpBlob->Release();
	}

	// Compile and create the Domain Shader
	if (ds != NULL)
	{
		CompileShaderFromFile(ds, "main", "ds_" + ShaderLevel, &pTmpBlob);
		hr = Device->CreateDomainShader(pTmpBlob->GetBufferPointer(), pTmpBlob->GetBufferSize(), NULL, &_pDomainShader);
		if (FAILED(hr))
		{
			MessageBox(NULL, L"Failed to create Domain Shader.", TEXT(__FILE__), MB_OK);
			exit(1);
		}
		pTmpBlob->Release();
	}

	// Compile and create the Geometry Shader
	if (gs != NULL)
	{
		CompileShaderFromFile(gs, "main", "gs_" + ShaderLevel, &pTmpBlob);
		hr = Device->CreateGeometryShader(pTmpBlob->GetBufferPointer(), pTmpBlob->GetBufferSize(), NULL, &_pGeometryShader);
		if (FAILED(hr))
		{
			MessageBox(NULL, L"Failed to create Geometry Shader.", TEXT(__FILE__), MB_OK);
			exit(1);
		}
		pTmpBlob->Release();
	}

	// Compile and create the Pixel Shader
	if (ps != NULL)
	{
		CompileShaderFromFile(ps, "main", "ps_" + ShaderLevel, &pTmpBlob);
		hr = Device->CreatePixelShader(pTmpBlob->GetBufferPointer(), pTmpBlob->GetBufferSize(), NULL, &_pPixelShader);
		if (FAILED(hr))
		{
			MessageBox(NULL, L"Failed to create Pixel Shader.", TEXT(__FILE__), MB_OK);
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