#include <Engine/Input.hpp>

Engine::Input::Input(const HWND &hWnd)
{
	LPDIRECTINPUT8 directInputObject;

	DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&directInputObject, NULL);
	directInputObject->CreateDevice(GUID_SysKeyboard, &_pKeyBoard, NULL);
	directInputObject->CreateDevice(GUID_SysMouse, &_pMouse, NULL);

	_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);
	_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);

	_pMouse->SetDataFormat(&c_dfDIMouse);
	_pMouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);

	_pKeyBoard->Acquire();
	_pMouse->Acquire();
	directInputObject->Release();

	ZeroMemory(_keyState, sizeof(_keyState));
	ZeroMemory(&_mouseState, sizeof(_mouseState));
}

Engine::Input::~Input(void)
{
	_pKeyBoard->Release();
	_pMouse->Release();
}

BOOL Engine::Input::getKeyBoardState(const BYTE &button)
{
	if (_keyState[button] & 0x80)
		return TRUE;
	return FALSE;
}

BOOL Engine::Input::getMouseState(const BYTE &button)
{
	if (_mouseState.rgbButtons[button] & 0x80)
		return TRUE;
	return FALSE;
}

LONG Engine::Input::getMouseRelX(void)
{
	return _mouseState.lX;
}

LONG Engine::Input::getMouseRelY(void)
{
	return _mouseState.lY;
}

void Engine::Input::refresh(void)
{
	HRESULT hr;

	hr = _pKeyBoard->GetDeviceState(sizeof(_keyState), (LPVOID)&_keyState);
	if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED)
		_pKeyBoard->Acquire();

	hr = _pMouse->GetDeviceState(sizeof(_mouseState), (LPVOID)&_mouseState);
	if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED)
		_pMouse->Acquire();
}