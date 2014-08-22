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
		// Device
		ID3D11Device *_pd3dDevice;
		// DeferredContext
		ID3D11DeviceContext *_pDefferedContext;
	public:
		FrameBuffer(void);
		~FrameBuffer(void);
		UINT getWidth(void) const;
		UINT getHeight(void) const;
		ID3D11DeviceContext *getContext(void) const;
		virtual void clear(void) const = 0;
	};
}

#endif