#include <Engine/Input.hpp>

engine::Input::Input(void)
{
	_pDirectInputObject = NULL;
	_pKeyBoard = NULL;
	_pMouse = NULL;
	ZeroMemory(_keyState, sizeof(_keyState));
	ZeroMemory(&_mouseState, sizeof(_mouseState));
}

engine::Input::~Input(void)
{
	if (_pMouse)
	{
		_pMouse->Unacquire();
		_pMouse->Release();
	}
	if (_pKeyBoard)
	{
		_pKeyBoard->Unacquire();
		_pKeyBoard->Release();
	}
	if (_pDirectInputObject)
		_pDirectInputObject->Release();
}

HRESULT engine::Input::initInput(const HINSTANCE &hInstance, const HWND &hWnd)
{
	HRESULT hr;
	hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&_pDirectInputObject, NULL);
	if (FAILED(hr))
		return hr;

	hr = _pDirectInputObject->CreateDevice(GUID_SysKeyboard, &_pKeyBoard, NULL);
	if (FAILED(hr))
		return hr;
	hr = _pDirectInputObject->CreateDevice(GUID_SysMouse, &_pMouse, NULL);
	if (FAILED(hr))
		return hr;

	hr = _pKeyBoard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
		return hr;
	hr = _pKeyBoard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(hr))
		return hr;

	hr = _pMouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(hr))
		return hr;
	hr = _pMouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(hr))
		return hr;

	_pKeyBoard->Acquire();
	_pMouse->Acquire();

	return S_OK;
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

BOOL engine::Input::getKeyBoardState(const BYTE &button)
{
	if (_keyState[button] & 0x80)
		return TRUE;
	return FALSE;
}

BOOL engine::Input::getMouseState(const BYTE &button)
{
	if (_mouseState.buttons[button] & 0x80)
		return TRUE;
	return FALSE;
}

LONG engine::Input::getMouseRelX(void)
{
	return _mouseState.x;
}

LONG engine::Input::getMouseRelY(void)
{
	return _mouseState.y;
}