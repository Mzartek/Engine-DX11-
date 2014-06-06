#include "include/graphics.h"

HRESULT initBuffer(void)
{
	HRESULT hr;
	vertex vertexArray[] = {
			{ DirectX::XMFLOAT3(-5, 0, -5), DirectX::XMFLOAT2(0, 0), DirectX::XMFLOAT3(0, 1, 0) },
			{ DirectX::XMFLOAT3(-5, 0,  5), DirectX::XMFLOAT2(0, 1), DirectX::XMFLOAT3(0, 1, 0) },
			{ DirectX::XMFLOAT3( 5, 0,  5), DirectX::XMFLOAT2(1, 1), DirectX::XMFLOAT3(0, 1, 0) },
			{ DirectX::XMFLOAT3( 5, 0, -5), DirectX::XMFLOAT2(1, 0), DirectX::XMFLOAT3(0, 1, 0) }
	};
	UINT index[] = {0, 1, 2, 0, 2, 3};
}
