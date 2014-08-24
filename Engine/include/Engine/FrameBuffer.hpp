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
		// Device and Context
		ID3D11Device *_pd3dDevice;
		ID3D11DeviceContext *_pContext;
		// DeferredContext
		ID3D11DeviceContext *_pDeferredContext;
	public:
		FrameBuffer(void);
		~FrameBuffer(void);
		UINT getWidth(void) const;
		UINT getHeight(void) const;
		ID3D11DeviceContext *getContext(void) const;
		void executeContext(void) const;
		virtual void clear(void) const = 0;
	};
}

#endif