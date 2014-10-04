#ifndef SHADOW_MAP
#define SHADOW_MAP

#include "FrameBuffer.hpp"

namespace Engine
{
	class DLLAPI ShadowMap : public FrameBuffer
	{
	private:
		// ShaderRessource
		ID3D11ShaderResourceView *_pShaderResourceView;
		// View
		ID3D11DepthStencilView *_pDepthView;
		// State
		ID3D11DepthStencilState *_pDepthState;
		ID3D11RasterizerState *_pRasterizerState;
		ID3D11SamplerState *_pSamplerComparisonState;
	public:
		ShadowMap(const EngineDevice &EngineDevice);
		~ShadowMap(void);
		void config(const UINT &width, const UINT &height);
		ID3D11ShaderResourceView *getShaderResourceView(void) const;
		ID3D11SamplerState *getSamplerComparisonState(void) const;
		void setState(void) const;
		void clear(void) const;
	};
}

#endif