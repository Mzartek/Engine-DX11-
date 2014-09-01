#ifndef FRAMEBUFFER
#define FRAMEBUFFER

#include "Object.hpp"

namespace engine
{
	class DLLAPI FrameBuffer : public Object
	{
	protected:
		UINT _width;
		UINT _height;
		ID3D11Device *_pd3dDevice;
		ID3D11DeviceContext *_pContext;
		ID3D11DeviceContext *_pDeferredContext;
	public:
		FrameBuffer(void);
		~FrameBuffer(void);
		virtual void config(const UINT &width, const UINT &height, ID3D11Device *pd3dDevice, ID3D11DeviceContext *pContext) = 0;
		UINT getWidth(void) const;
		UINT getHeight(void) const;
		ID3D11DeviceContext *getContext(void) const;
		void executeDeferredContext(void) const;
		virtual void clear(void) const = 0;
	};
}

#endif