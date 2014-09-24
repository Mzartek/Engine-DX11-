#ifndef INPUT_MANAGER
#define INPUT_MANAGER

#include "Object.hpp"

namespace engine
{
	class DLLAPI Input : public Object
	{
	private:
		LPDIRECTINPUTDEVICE8 _pKeyBoard;
		LPDIRECTINPUTDEVICE8 _pMouse;
		BYTE _keyState[256];
		DIMOUSESTATE _mouseState;
	public:
		Input(void);
		~Input(void);
		void initInput(const HINSTANCE &hInstance, const HWND &hWnd);
		BOOL getKeyBoardState(const BYTE &button);
		BOOL getMouseState(const BYTE &button);
		LONG getMouseRelX(void);
		LONG getMouseRelY(void);
		void refresh(void);
	};
}

#endif
