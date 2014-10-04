#ifndef ENGINE_HEAD
#define ENGINE_HEAD

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
#include <d3d11_1.h>
#include <dinput.h>
#include <d3dcompiler.h>
#include <directxcolors.h>
#include <DirectXMath.h>

using namespace DirectX;

namespace Engine
{
	struct EngineDevice
	{
		D3D_FEATURE_LEVEL FeatureLevel;
		std::string ShaderLevel;
		ID3D11Device1 *Device;
		ID3D11DeviceContext1 *DeviceContext;
	};
}

#endif