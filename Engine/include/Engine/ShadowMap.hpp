#ifndef SHADOW_MAP
#define SHADOW_MAP

#include "FrameBuffer.hpp"
#include "ShaderProgram.hpp"

namespace engine
{
	class DLLAPI ShadowMap : public FrameBuffer
	{
	private:
		// Texture
		ID3D11Texture2D *_pTexture;
		// ShaderRessource
		ID3D11ShaderResourceView *_pShaderResourceView;
		// View
		ID3D11DepthStencilView *_pDepthView;
		// State
		ID3D11SamplerState *_pSamplerState;
		ID3D11DepthStencilState *_pDepthState;
		ID3D11RasterizerState *_pRasterizerState;
		ShaderProgram *_program;
	public:
		ShadowMap();
		~ShadowMap();
		HRESULT config(const UINT &width, const UINT &height, ID3D11Device *pd3dDevice);
	};
}

#endif