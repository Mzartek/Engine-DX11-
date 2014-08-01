#include "include/config.hpp"

HRESULT configShader(void)
{
	HRESULT hr;

	hr = gObjectProgram->loadProgram(L"shader/object/gObjectVertex.hlsl", NULL, L"shader/object/gObjectPixel.hlsl", window->getD3DDevice());
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed gBufferProgramm.", "ShaderProgram", MB_OK);
		return hr;
	}

	hr = screenProgram->loadProgram(L"shader/screen/screenVertex.hlsl", NULL, L"shader/screen/screenPixel.hlsl", window->getD3DDevice());
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed screenProgram.", "ShaderProgram", MB_OK);
		return hr;
	}

	return S_OK;
}

HRESULT configBuffer(void)
{
	return gBuffer->config(window->getWidth(), window->getHeight(), window->getD3DDevice());
}

HRESULT configModels(void)
{
	HRESULT hr;

	FLOAT mat_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	FLOAT mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	FLOAT mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	FLOAT mat_shininess[] = { 20.0f };

	engine::Vertex vertexArray[] = {
			{ DirectX::XMFLOAT3(-500, 0, -500), DirectX::XMFLOAT2(0, 0), DirectX::XMFLOAT3(0, 1, 0) },
			{ DirectX::XMFLOAT3(-500, 0, 500), DirectX::XMFLOAT2(0, 1), DirectX::XMFLOAT3(0, 1, 0) },
			{ DirectX::XMFLOAT3(500, 0, 500), DirectX::XMFLOAT2(1, 1), DirectX::XMFLOAT3(0, 1, 0) },
			{ DirectX::XMFLOAT3(500, 0, -500), DirectX::XMFLOAT2(1, 0), DirectX::XMFLOAT3(0, 1, 0) }
	};
	UINT index[] = { 0, 2, 1, 0, 2, 3 };

	sol->initObjectArray();
	hr = sol->config(gObjectProgram, window->getD3DDevice());
	if (FAILED(hr))
		return hr;
	hr = sol->createObject(sizeof(vertexArray), (FLOAT *)vertexArray,
		sizeof index, index,
		"resources/ornaments.jpg",
		mat_ambient, mat_diffuse, mat_specular, mat_shininess,
		window->getD3DDevice(), window->getImmediateContext());
	if (FAILED(hr))
		return hr;

	// Heli
	heli->initObjectArray();
	hr = heli->config(gObjectProgram, window->getD3DDevice());
	if (FAILED(hr))
		return hr;
	hr = heli->loadFromFile("resources/heli/corps.obj", window->getD3DDevice(), window->getImmediateContext());
	if (FAILED(hr))
		return hr;
	heli->sortObject();
	heli->matIdentity();
	heli->matTranslate(0.0f, 6.0f, 0.0f);
	heli->matScale(2, 2, 2);

	return S_OK;
}

HRESULT configScreen(void)
{
	return screen->config(screenProgram, window->getD3DDevice());
}