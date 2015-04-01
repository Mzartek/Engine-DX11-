#ifndef TEXTURE_HEADER
#define TEXTURE_HEADER

#include "Object.hpp"

namespace Engine
{
	enum TextureType { TEXTURE_2D_TYPE, TEXTURE_CUBE_TYPE };

	class DLLAPI Texture : public Object
	{
	protected:
		ID3D11ShaderResourceView *_pShaderResourceView;
		ID3D11SamplerState *_pSamplerState;
	public:
		Texture(void);
		~Texture(void);
		ID3D11ShaderResourceView *getShaderResourceView(void);
		ID3D11SamplerState *getSamplerState(void);
		virtual TextureType getType(void) const = 0;
	};
}

#endif