#include <Engine/Input.hpp>

engine::Input::Input(void)
	: _pKeyBoard(NULL), _pMouse(NULL)
{
	ZeroMemory(_keyState, sizeof(_keyState));
	ZeroMemory(&_mouseState, sizeof(_mouseState));
}

engine::Input::~Input(void)
{
	if (_pKeyBoard)	_pKeyBoard->Release();
	if (_pMouse) _pMouse->Release();
}

void engine::Input::initInput(const HINSTANCE &hInstance, const HWND &hWnd)
{
	LPDIRECTINPUT8 directInputObject;

	if (_pKeyBoard)	_pKeyBoard->Release();
	if (_pMouse) _pMouse->Release();

	DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&directInputObject, NULL);
	directInputObject->CreateDevice(GUID_SysKeyboard, &_pKeyBoard, NULL);
	directInputObject->CreateDevice(GUID_SysMouse, &_pMouse, NULL);

	_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);
	_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);

	_pMouse->SetDataFormat(&c_dfDIMouse);
	_pMouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);

	_pKeyBoard->Acquire();
	_pMouse->Acquire();
	directInputObject->Release();
}

BOOL engine::Input::getKeyBoardState(const BYTE &button)
{
	if (_keyState[button] & 0x80)
		return TRUE;
	return FALSE;
}

BOOL engine::Input::getMouseState(const BYTE &button)
{
	if (_mouseState.rgbButtons[button] & 0x80)
		return TRUE;
	return FALSE;
}

LONG engine::Input::getMouseRelX(void)
{
	return _mouseState.lX;
}

LONG engine::Input::getMouseRelY(void)
{
	return _mouseState.lY;
}

void engine::Input::refresh(void)
{
	HRESULT hr;

	hr = _pKeyBoard->GetDeviceState(sizeof(_keyState), (LPVOID)&_keyState);
	if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED)
		_pKeyBoard->Acquire();

	hr = _pMouse->GetDeviceState(sizeof(_mouseState), (LPVOID)&_mouseState);
	if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED)
		_pMouse->Acquire();
}