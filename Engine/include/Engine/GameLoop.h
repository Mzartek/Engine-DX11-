#pragma once

#include "Object.h"

namespace Engine
{
	class DLLAPI GameLoop : public Object
	{
	public:
		virtual void display(FLOAT state) = 0;
		virtual void state(ULONG64 time) = 0;
		virtual void lastState(void) = 0;
		virtual void reshape(UINT w, UINT h) = 0;
	};
}