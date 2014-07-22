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
	public:
		FrameBuffer(void);
		~FrameBuffer(void);
		UINT getWidth(void) const;
		UINT getHeight(void) const;
		virtual void clear(void) const = 0;
	};
}

#endif