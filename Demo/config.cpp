#include "include/config.h"

HRESULT configShader(void)
{
	return mainProgram->loadProgram(L"shader/mainVertex.hlsl", L"shader/mainPixel.hlsl", window->getD3DDevice());
}

HRESULT configSol(void)
{
	HRESULT hr;

	FLOAT mat_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	FLOAT mat_diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	FLOAT mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	FLOAT mat_shininess[] = { 20.0f };

	engine::vertex vertexArray[] = {
			{ DirectX::XMFLOAT3(-5, 0, -5), DirectX::XMFLOAT2(0, 0), DirectX::XMFLOAT3(0, 1, 0) },
			{ DirectX::XMFLOAT3(-5, 0, 5), DirectX::XMFLOAT2(0, 1), DirectX::XMFLOAT3(0, 1, 0) },
			{ DirectX::XMFLOAT3(5, 0, 5), DirectX::XMFLOAT2(1, 1), DirectX::XMFLOAT3(0, 1, 0) },
			{ DirectX::XMFLOAT3(5, 0, -5), DirectX::XMFLOAT2(1, 0), DirectX::XMFLOAT3(0, 1, 0) }
	};
	UINT index[] = { 0, 1, 2, 0, 2, 3 };

	sol->initObjectArray();
	hr = sol->config(mainProgram, window->getD3DDevice());
	if (FAILED(hr))
		return hr;

	hr = sol->createObject(sizeof(vertexArray), (FLOAT *)vertexArray,
		sizeof index, index,
		"ressources/sand.jpg",
		mat_ambient, mat_diffuse, mat_specular, mat_shininess,
		window->getD3DDevice());

	return hr;
}