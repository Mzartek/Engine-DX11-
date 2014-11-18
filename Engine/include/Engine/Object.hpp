#ifndef OBJECT_HEADER
#define OBJECT_HEADER

#include "EngineHead.hpp"

namespace Engine
{;
	class DLLAPI Object
	{
	private:
		static INT _memState;
	public:
		Object(void);
		virtual ~Object(void);
		void *operator new(size_t sz);
		void *operator new[](size_t sz);
		void operator delete(void *p);
		void operator delete[](void *p);
		static INT getMemoryState(void);
		static void saveMemoryInfo(const CHAR *filename);
	};
}

#endif
