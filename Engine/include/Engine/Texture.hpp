#ifndef TEXTURE_HEADER
#define TEXTURE_HEADER

#include "Object.hpp"

namespace Engine
{
	class DLLAPI Texture : public Object
	{
	private:
		EngineDevice _EngineDevice;
		ID3D11ShaderResourceView *_pShaderResourceView;
		ID3D11SamplerState *_pSamplerState;
	public:
		Texture(const EngineDevice &EngineDevice);
		~Texture(void);
		ID3D11ShaderResourceView *getShaderResourceView(void);
		ID3D11SamplerState *getSamplerState(void);
		void load2DTextureFromFile(const CHAR *path);
		void loadCubeTextureFromFiles(
			const CHAR *posx, const CHAR *negx,
			const CHAR *posy, const CHAR *negy,
			const CHAR *posz, const CHAR *negz);
	};
}

#endif