#include <Engine/ShaderProgram.hpp>

HRESULT engine::ShaderProgram::CompileShaderFromFile(WCHAR *szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob **ppBlobOut)
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
			OutputDebugStringA((char *)(pErrorBlob->GetBufferPointer()));
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
	_pPixelShader = NULL;
	_pVSBlob = NULL;
	_pPSBlob = NULL;
}

engine::ShaderProgram::~ShaderProgram(void)
{
	if (_pVertexShader) 
		_pVertexShader->Release();
	if (_pPixelShader) 
		_pPixelShader->Release();
	if (_pVSBlob)
		_pVSBlob->Release();
	if (_pPSBlob)
		_pPSBlob->Release();
}

HRESULT engine::ShaderProgram::loadProgram(WCHAR *vs, WCHAR *ps, ID3D11Device *pDevice)
{
	HRESULT hr;

	// Compile and create the vertex shader
	hr = CompileShaderFromFile(vs, "main", "vs_4_0", &_pVSBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Error compiling shader.", "Error", MB_OK);
		return hr;
	}
	hr = pDevice->CreateVertexShader(_pVSBlob->GetBufferPointer(), _pVSBlob->GetBufferSize(), NULL, &_pVertexShader);
	if (FAILED(hr))
		return hr;

	// Compile and create the pixel shader
	hr = CompileShaderFromFile(ps, "main", "ps_4_0", &_pPSBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Error compiling shader.", "Error", MB_OK);
		return hr;
	}
	hr = pDevice->CreatePixelShader(_pPSBlob->GetBufferPointer(), _pPSBlob->GetBufferSize(), NULL, &_pPixelShader);
	if (FAILED(hr))
		return hr;

	return S_OK;
}

ID3D11VertexShader *engine::ShaderProgram::getVertexShader(void)
{
	return _pVertexShader;
}

ID3D11PixelShader *engine::ShaderProgram::getPixelShader(void)
{
	return _pPixelShader;
}

ID3DBlob *engine::ShaderProgram::getVSBlob(void)
{
	return _pVSBlob;
}

ID3DBlob *engine::ShaderProgram::getPSBlob(void)
{
	return _pPSBlob;
}