#ifndef TEXTURE
#define TEXTURE

#include "Object.hpp"

namespace engine
{
	class DLLAPI Texture : public Object
	{
	private:
		ID3D11ShaderResourceView *_pShaderResourceView;
		ID3D11SamplerState *_pSamplerState;
	public:
		Texture(void);
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