#ifndef OBJECT_HEADER
#define OBJECT_HEADER

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
#include <set>
#include <string>
#include <algorithm>

#include <Windows.h>
#include <d3d11_2.h>
#include <dinput.h>
#include <d3dcompiler.h>
#include <directxcolors.h>
#include <DirectXMath.h>

using namespace DirectX;

namespace Engine
{
	extern D3D_FEATURE_LEVEL FeatureLevel;
	extern std::string ShaderLevel;
	extern ID3D11Device1 *Device;
	extern ID3D11DeviceContext1 *DeviceContext;

	class DLLAPI Object
	{
	private:
		static INT _memState;
	public:
		Object(void);
		virtual ~Object(void);
		void *operator new(size_t sz);
		void *operator new[](size_t sz);
		void operator delete(void *p);
		void operator delete[](void *p);
		static INT getMemoryState(void);
		static void saveMemoryInfo(const CHAR *filename);
	};
}

#endif
