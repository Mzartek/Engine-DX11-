#include <Engine/ShaderProgram.hpp>

static HRESULT CompileShaderFromFile(WCHAR *szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob **ppBlobOut)
{
	HRESULT hr;
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
	ID3DBlob *pErrorBlob = NULL;

	hr = D3DCompileFromFile(szFileName, NULL, NULL, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
	if (FAILED(hr))
	{
		if (pErrorBlob)
		{
			OutputDebugStringA((char *)pErrorBlob->GetBufferPointer());
			MessageBox(NULL, (char *)pErrorBlob->GetBufferPointer(), "ShaderProgram", MB_OK);
			pErrorBlob->Release();
		}
		return hr;
	}
	if (pErrorBlob) 
		pErrorBlob->Release();

	return S_OK;
}

engine::ShaderProgram::ShaderProgram(void)
{
	_pVertexShader = NULL;
	_pGeometryShader = NULL;
	_pPixelShader = NULL;
	_pBlob = NULL;
}

engine::ShaderProgram::~ShaderProgram(void)
{
	if (_pVertexShader) 
		_pVertexShader->Release();
	if (_pGeometryShader)
		_pGeometryShader->Release();
	if (_pPixelShader) 
		_pPixelShader->Release();
	if (_pBlob)
		_pBlob->Release();
}

HRESULT engine::ShaderProgram::loadProgram(WCHAR *vs, WCHAR *gs, WCHAR *ps, ID3D11Device *pDevice)
{
	HRESULT hr;

	if (_pVertexShader)
	{
		_pVertexShader->Release();
		_pVertexShader = NULL;
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

	// Compile and create the pixel shader
	if (ps != NULL)
	{
		if (_pBlob)
			_pBlob->Release();
		hr = CompileShaderFromFile(ps, "main", "ps_4_0", &_pBlob);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed compiling Pixel Shader.", "ShaderProgram", MB_OK);
			return hr;
		}
		hr = pDevice->CreatePixelShader(_pBlob->GetBufferPointer(), _pBlob->GetBufferSize(), NULL, &_pPixelShader);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to create Pixel Shader.", "ShaderProgram", MB_OK);
			return hr;
		}
	}

	// Compile and create the geometry shader
	if (gs != NULL)
	{
		if (_pBlob)
			_pBlob->Release();
		hr = CompileShaderFromFile(gs, "main", "gs_4_0", &_pBlob);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed compiling Geometry Shader.", "ShaderProgram", MB_OK);
			return hr;
		}
		hr = pDevice->CreateGeometryShader(_pBlob->GetBufferPointer(), _pBlob->GetBufferSize(), NULL, &_pGeometryShader);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to create Geometry Shader.", "ShaderProgram", MB_OK);
			return hr;
		}
	}

	// Compile and create the vertex shader
	if (vs != NULL)
	{
		if (_pBlob)
			_pBlob->Release();
		hr = CompileShaderFromFile(vs, "main", "vs_4_0", &_pBlob);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed compiling Vertex Shader.", "ShaderProgram", MB_OK);
			return hr;
		}
		hr = pDevice->CreateVertexShader(_pBlob->GetBufferPointer(), _pBlob->GetBufferSize(), NULL, &_pVertexShader);
		if (FAILED(hr))
		{
			MessageBox(NULL, "Failed to create Vertex Shader.", "ShaderProgram", MB_OK);
			return hr;
		}
	}

	return S_OK;
}

ID3D11VertexShader *engine::ShaderProgram::getVertexShader(void)
{
	return _pVertexShader;
}

ID3D11GeometryShader *engine::ShaderProgram::getGeometryShader(void)
{
	return _pGeometryShader;
}

ID3D11PixelShader *engine::ShaderProgram::getPixelShader(void)
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