#ifndef INPUT_MANAGER
#define INPUT_MANAGER

#include "Object.hpp"

namespace engine
{
	class DLLAPI Input : public Object
	{
	private:
		LPDIRECTINPUT8 _pDirectInputObject;
		LPDIRECTINPUTDEVICE8 _pKeyBoard;
		LPDIRECTINPUTDEVICE8 _pMouse;
		BYTE _keyState[256];
		struct MOUSESTATE 
		{
			LONG x;
			LONG y;
			LONG z;
			BYTE buttons[4];
		} _mouseState;
	public:
		Input(void);
		~Input(void);
		HRESULT initInput(const HINSTANCE &hInstance, const HWND &hWnd);
		void refresh(void);
		BOOL getKeyBoardState(const BYTE &button);
		BOOL getMouseState(const BYTE &button);
		LONG getMouseRelX(void);
		LONG getMouseRelY(void);
	};
}

#endif
