#ifndef DX_HEAD
#define DX_HEAD

#pragma warning(disable:4324)

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
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxcolors.h>
#include <dinput.h>

using namespace DirectX;

namespace engine
{
	typedef struct Vertex
	{
		XMFLOAT3 position;
		XMFLOAT2 texCoord;
		XMFLOAT3 normal;
	} Vertex;

	extern DLLAPI void loadTextureFromFile(const TCHAR *szFileName, 
		ID3D11Texture2D **pptex, ID3D11ShaderResourceView **ppshr, ID3D11SamplerState **ppsam,
		ID3D11Device *pd3dDevice, ID3D11DeviceContext *pContext);
}

#endif