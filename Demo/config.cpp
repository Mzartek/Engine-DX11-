#include "include/config.hpp"

HRESULT configShader(void)
{
	return mainProgram->loadProgram(L"shader/main/mainVertex.hlsl", NULL, L"shader/main/mainPixel.hlsl", window->getD3DDevice());
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
	hr = sol->config(mainProgram, window->getD3DDevice());
	if (FAILED(hr))
		return hr;
	hr = sol->createObject(sizeof(vertexArray), (FLOAT *)vertexArray,
		sizeof index, index,
		"resources/ornaments.jpg",
		mat_ambient, mat_diffuse, mat_specular, mat_shininess,
		window->getD3DDevice());
	if (FAILED(hr))
		return hr;

	// Heli
	heli->initObjectArray();
	hr = heli->config(mainProgram, window->getD3DDevice());
	if (FAILED(hr))
		return hr;
	hr = heli->loadFromFile("resources/heli/corps.obj", window->getD3DDevice());
	heli->matTranslate(0.0f, 4.0f, 0.0f);
	heli->sortObject();

	return hr;
}