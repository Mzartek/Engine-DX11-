#ifndef GBUFFER_HEADER
#define GBUFFER_HEADER

#include "FrameBuffer.hpp"

#define GBUF_NUM_TEX 5
#define GBUF_NORMAL 0
#define GBUF_MATERIAL 1
#define GBUF_LIGHT 2
#define GBUF_BACKGROUND 3
#define GBUF_DEPTH 4
#define GBUF_STENCIL 5

namespace Engine
{
	class DLLAPI GBuffer : public FrameBuffer
	{
	private:
		// Shader Resource View
		ID3D11ShaderResourceView *_pShaderResourceView[GBUF_NUM_TEX + 1];
		// Render View
		ID3D11RenderTargetView *_pRenderTargetView[GBUF_NUM_TEX - 1];
		// Depth View
		ID3D11DepthStencilView *_pDepthStencilView;
		// Depth Stencil State
		ID3D11DepthStencilState *_pDepthNoWriteState;
		ID3D11DepthStencilState *_pDepthStencilState;
		ID3D11DepthStencilState *_pNoDepthStencilState;
		ID3D11DepthStencilState *_pStencilState;
		// Blend State
		ID3D11BlendState *_pNoBlendState;
		ID3D11BlendState *_pAdditiveBlendState;
		ID3D11BlendState *_pAlphaBlendState;
		// Rasterizer State
		ID3D11RasterizerState *_pRasterizerState;
	public:
		GBuffer(void);
		~GBuffer(void);
		void config(const UINT &width, const UINT &height);
		ID3D11ShaderResourceView *getShaderResourceView(const UINT &num) const;
		void setSkyboxState(void) const;
		void setGeometryState(void) const;
		void setLightState(void) const;
		void setParticlesState(void) const;
		void setBackgroundState(void) const;
		void clear(void) const;
		void clearLight(void) const;
	};
}

#endif