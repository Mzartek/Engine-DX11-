#include <Engine/ShaderProgram.hpp>

extern std::string ShaderLevel;
extern ID3D11Device1 *Device;

static void CompileShaderFromFile(const LPCWSTR szFileName, const std::string szEntryPoint, const std::string szShaderModel, ID3DBlob **ppBlobOut)
{
	HRESULT hr;
	ID3DBlob *pErrorBlob = NULL;

	hr = D3DCompileFromFile(szFileName, NULL, NULL, szEntryPoint.c_str(), szShaderModel.c_str(), D3DCOMPILE_ENABLE_STRICTNESS, 0, ppBlobOut, &pErrorBlob);
	if (FAILED(hr))
	{
		std::wstring txt = L"Error file: ";
		txt.append(szFileName);
		MessageBoxW(NULL, txt.c_str(), L"Error shader", MB_OK);
		if (pErrorBlob)
		{
			MessageBoxA(NULL, (char *)pErrorBlob->GetBufferPointer(), "Error shader", MB_OK);
			pErrorBlob->Release();
		}
		exit(1);
	}
	if (pErrorBlob) pErrorBlob->Release();
}

Engine::ShaderProgram::ShaderProgram(void)
	: _pVertexShader(NULL), _pHullShader(NULL), _pDomainShader(NULL), _pGeometryShader(NULL), _pPixelShader(NULL),
	_pBlob(NULL)
{
}

Engine::ShaderProgram::~ShaderProgram(void)
{
	if (_pVertexShader) _pVertexShader->Release();
	if (_pHullShader) _pHullShader->Release();
	if (_pDomainShader) _pDomainShader->Release();
	if (_pGeometryShader) _pGeometryShader->Release();
	if (_pPixelShader) _pPixelShader->Release();
	if (_pBlob) _pBlob->Release();
}

void Engine::ShaderProgram::loadProgram(LPCWSTR vs, LPCWSTR hs, LPCWSTR ds, LPCWSTR gs, LPCWSTR ps)
{
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
		MessageBox(NULL, TEXT("Need a Vertex Shader for ShaderProgram"), TEXT(__FILE__), MB_OK);
		exit(1);
	}
	CompileShaderFromFile(vs, "main", "vs_" + ShaderLevel, &_pBlob);
	Device->CreateVertexShader(_pBlob->GetBufferPointer(), _pBlob->GetBufferSize(), NULL, &_pVertexShader);

	// Compile and create the Hull Shader
	if (hs != NULL)
	{
		CompileShaderFromFile(hs, "main", "hs_" + ShaderLevel, &pTmpBlob);
		Device->CreateHullShader(pTmpBlob->GetBufferPointer(), pTmpBlob->GetBufferSize(), NULL, &_pHullShader);
		pTmpBlob->Release();
	}

	// Compile and create the Domain Shader
	if (ds != NULL)
	{
		CompileShaderFromFile(ds, "main", "ds_" + ShaderLevel, &pTmpBlob);
		Device->CreateDomainShader(pTmpBlob->GetBufferPointer(), pTmpBlob->GetBufferSize(), NULL, &_pDomainShader);
		pTmpBlob->Release();
	}

	// Compile and create the Geometry Shader
	if (gs != NULL)
	{
		CompileShaderFromFile(gs, "main", "gs_" + ShaderLevel, &pTmpBlob);
		Device->CreateGeometryShader(pTmpBlob->GetBufferPointer(), pTmpBlob->GetBufferSize(), NULL, &_pGeometryShader);
		pTmpBlob->Release();
	}

	// Compile and create the Pixel Shader
	if (ps != NULL)
	{
		CompileShaderFromFile(ps, "main", "ps_" + ShaderLevel, &pTmpBlob);
		Device->CreatePixelShader(pTmpBlob->GetBufferPointer(), pTmpBlob->GetBufferSize(), NULL, &_pPixelShader);
		pTmpBlob->Release();
	}
}

ID3D11VertexShader *Engine::ShaderProgram::getVertexShader(void) const
{
	return _pVertexShader;
}

ID3D11HullShader *Engine::ShaderProgram::getHullShader(void) const
{
	return _pHullShader;
}

ID3D11DomainShader *Engine::ShaderProgram::getDomainShader(void) const
{
	return _pDomainShader;
}

ID3D11GeometryShader *Engine::ShaderProgram::getGeometryShader(void) const
{
	return _pGeometryShader;
}

ID3D11PixelShader *Engine::ShaderProgram::getPixelShader(void) const
{
	return _pPixelShader;
}

void *Engine::ShaderProgram::getEntryBufferPointer(void)
{
	return _pBlob->GetBufferPointer();
}

SIZE_T Engine::ShaderProgram::getEntryBytecodeLength(void)
{
	return _pBlob->GetBufferSize();
}