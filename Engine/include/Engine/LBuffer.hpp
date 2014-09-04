#ifndef LBUFFER
#define LBUFFER

#include "FrameBuffer.hpp"

namespace engine
{
	class DLLAPI LBuffer : public FrameBuffer
	{
		// Texture
		ID3D11Texture2D *_pTexture;
		// Shader Resource View
		ID3D11ShaderResourceView *_pShaderResourceView;
		// Render View
		ID3D11RenderTargetView *_pRenderTargetView;
		// State
		ID3D11DepthStencilState *_pDepthStencilState;
		ID3D11BlendState *_pBlendState;
		ID3D11RasterizerState *_pRasterizerState;
	public:
		LBuffer(void);
		~LBuffer(void);
		void config(const UINT &width, const UINT &height, ID3D11Device *pd3dDevice, ID3D11DeviceContext *pContext);
		ID3D11ShaderResourceView *getShaderResourceView(void) const;
		void clear(void) const;
	};
}

#endif