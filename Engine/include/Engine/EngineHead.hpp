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

#endif