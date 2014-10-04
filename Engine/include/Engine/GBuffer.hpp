#ifndef GBUFFER
#define GBUFFER

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
		ID3D11DepthStencilState *_pSkyboxDepthStencilState;
		ID3D11DepthStencilState *_pGeometryDepthStencilState;
		ID3D11DepthStencilState *_pLightDepthStencilState;
		ID3D11DepthStencilState *_pBackgroundDepthStencilState;
		// Blend State
		ID3D11BlendState *_pSkyboxBlendState;
		ID3D11BlendState *_pGeometryBlendState;
		ID3D11BlendState *_pLightBlendState;
		ID3D11BlendState *_pBackgroundBlendState;
		// Rasterizer State
		ID3D11RasterizerState *_pRasterizerState;
	public:
		GBuffer(const EngineDevice &EngineDevice);
		~GBuffer(void);
		void config(const UINT &width, const UINT &height);
		ID3D11ShaderResourceView *getShaderResourceView(const UINT &num) const;
		void setSkyboxState(void) const;
		void setGeometryState(void) const;
		void setLightState(void) const;
		void setBackgroundState(void) const;
		void clear(void) const;
		void clearLight(void) const;
	};
}

#endif