#ifndef SHADOW_MAP
#define SHADOW_MAP

#include "FrameBuffer.hpp"
#include "ShaderProgram.hpp"

namespace engine
{
	class DLLAPI ShadowMap : public FrameBuffer
	{
	private:
		ID3D11Texture2D *_pTexture;
		// ShaderRessource
		ID3D11ShaderResourceView *_pShaderResourceView;
		// View
		ID3D11DepthStencilView *_pDepthView;
		// State
		ID3D11DepthStencilState *_pDepthState;
		ID3D11RasterizerState *_pRasterizerState;
		ID3D11SamplerState *_pSamplerComparisonState;
	public:
		ShadowMap();
		~ShadowMap();
		void config(const UINT &width, const UINT &height, ID3D11Device *pd3dDevice, ID3D11DeviceContext *pContext);
		ID3D11ShaderResourceView *getShaderResourceView(void) const;
		ID3D11SamplerState *getSamplerComparisonState(void) const;
		void setConfig(void) const;
		void clear(void) const;
	};
}

#endif