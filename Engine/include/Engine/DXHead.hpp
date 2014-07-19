#ifndef DX_HEAD
#define DX_HEAD

#ifdef ENGINE_EXPORTS
#define DLLAPI __declspec(dllexport)
#else
#define DLLAPI __declspec(dllimport)
#endif

#define DIRECTINPUT_VERSION 0x0800

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <Windows.h>
#include <malloc.h>
#include <tchar.h>
#include <d3d11_2.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include <dinput.h>

namespace engine
{
	typedef struct Vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texCoord;
		DirectX::XMFLOAT3 normal;
	} Vertex;

	extern DLLAPI HRESULT loadTextureFromFile(const TCHAR *szFileName, ID3D11ShaderResourceView **pptex, ID3D11SamplerState **ppsam, ID3D11Device *pd3dDevice);
	extern DLLAPI HRESULT loadDDSTexture(const WCHAR *szFileName, ID3D11ShaderResourceView **pptex, ID3D11SamplerState **ppsam, ID3D11Device *pd3dDevice);
}

#endif