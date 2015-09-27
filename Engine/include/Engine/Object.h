#pragma once

#define _CRT_SECURE_NO_WARNINGS
#pragma warning ( disable : 4251)

#ifdef ENGINE_EXPORTS
#define DLLAPI __declspec(dllexport)
#else
#define DLLAPI __declspec(dllimport)
#endif

#include <iostream>
#include <fstream>
#include <codecvt>
#include <vector>
#include <set>
#include <string>
#include <algorithm>
#include <memory>

#include <Windows.h>
#include <d3d11_2.h>
#include <d3dcompiler.h>
#include <directxcolors.h>
#include <DirectXMath.h>
#include <Xinput.h>

using namespace DirectX;

namespace Engine
{
	class DLLAPI Object
	{
	public:
		Object(void);
		virtual ~Object(void);
	};
}