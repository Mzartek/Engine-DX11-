#ifndef FRAMEBUFFER_HEADER
#define FRAMEBUFFER_HEADER

#include "Object.hpp"

namespace Engine
{
	class DLLAPI FrameBuffer : public Object
	{
	protected:
		UINT _width;
		UINT _height;
		D3D11_VIEWPORT _VP;
	public:
		FrameBuffer(void);
		~FrameBuffer(void);
		virtual void config(const UINT &width, const UINT &height);
		UINT getWidth(void) const;
		UINT getHeight(void) const;
		virtual void clear(void) const = 0;
	};
}

#endif