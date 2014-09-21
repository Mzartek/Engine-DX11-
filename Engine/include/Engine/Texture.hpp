#ifndef TEXTURE
#define TEXTURE

#include "Object.hpp"

namespace engine
{
	class DLLAPI Texture : public Object
	{
	private:
		BYTE _dimension;
		union texture
		{
			ID3D11Texture1D *oneDim;
			ID3D11Texture2D *twoDim;
			ID3D11Texture3D *threeDim;
		} _pTexture;
		ID3D11ShaderResourceView *_pShaderResourceView;
		ID3D11SamplerState *_pSamplerState;
	public:
		Texture(void);
		~Texture(void);
		ID3D11ShaderResourceView *getShaderResourceView(void);
		ID3D11SamplerState *getSamplerState(void);
		void load2DTextureFromFile(const TCHAR *path);
		void loadCubeTextureFromFiles(
			const TCHAR *posx, const TCHAR *negx,
			const TCHAR *posy, const TCHAR *negy,
			const TCHAR *posz, const TCHAR *negz);
	};
}

#endif