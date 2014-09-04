#ifndef GBUFFER
#define GBUFFER

#include "FrameBuffer.hpp"

#define GBUF_NUM_TEX 3
#define GBUF_NORMAL 0
#define GBUF_MATERIAL 1
#define GBUF_DEPTH_STENCIL 2

namespace engine
{
	class DLLAPI GBuffer : public FrameBuffer
	{
	private:
		// Texture
		ID3D11Texture2D *_pTexture[GBUF_NUM_TEX];
		// Shader Resource View
		ID3D11ShaderResourceView *_pShaderResourceView[GBUF_NUM_TEX];
		// Render View
		ID3D11RenderTargetView *_pRenderTargetView[GBUF_NUM_TEX - 1];
		// Depth View
		ID3D11DepthStencilView *_pDepthStencilView;
		// State
		ID3D11DepthStencilState *_pDepthStencilState;
		ID3D11BlendState *_pBlendState;
		ID3D11RasterizerState *_pRasterizerState;
	public:
		GBuffer(void);
		~GBuffer(void);
		void config(const UINT &width, const UINT &height, ID3D11Device *pd3dDevice, ID3D11DeviceContext *pContext);
		ID3D11ShaderResourceView *getShaderResourceView(const UINT &num) const;
		void depthMask(const D3D11_DEPTH_WRITE_MASK &writeMask);
		void clear(void) const;
	};
}

#endif