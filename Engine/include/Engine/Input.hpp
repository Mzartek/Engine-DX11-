#ifndef INPUT_HEADER
#define INPUT_HEADER

#include "Object.hpp"

namespace Engine
{
	class DLLAPI Input : public Object
	{
	private:
		LPDIRECTINPUTDEVICE8 _pKeyBoard;
		LPDIRECTINPUTDEVICE8 _pMouse;
		BYTE _keyState[256];
		DIMOUSESTATE _mouseState;
	public:
		Input(const HWND &hWnd, HINSTANCE hInstance);
		~Input(void);
		BOOL getKeyBoardState(const BYTE &button);
		BOOL getMouseState(const BYTE &button);
		LONG getMouseRelX(void);
		LONG getMouseRelY(void);
		void refresh(void);
	};
}

#endif
