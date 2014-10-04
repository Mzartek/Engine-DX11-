#ifndef BUFFER
#define BUFFER

#include "Object.hpp"

namespace Engine
{
	class DLLAPI Buffer : public Object
	{
	private:
		EngineDevice _EngineDevice;
		D3D11_BIND_FLAG _target;
		ID3D11Buffer *_pBuffer;
		UINT _size;
	public:
		Buffer(const EngineDevice &EngineDevice);
		~Buffer(void);
		ID3D11Buffer *getBuffer(void);
		void createStore(const D3D11_BIND_FLAG &target, const void *data, const UINT &size, const D3D11_USAGE &usage);
		void updateStoreSub(const void *data);
		void updateStoreMap(const void *data);
	};
}

#endif