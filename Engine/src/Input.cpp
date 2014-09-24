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
	HRESULT hr;
	LPDIRECTINPUT8 directInputObject;

	if (_pKeyBoard)	_pKeyBoard->Release();
	if (_pMouse) _pMouse->Release();

	hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&directInputObject, NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Failed to create Input", TEXT(__FILE__), MB_OK);
		exit(1);
	}

	hr = directInputObject->CreateDevice(GUID_SysKeyboard, &_pKeyBoard, NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Failed to create KeyBoard Device", TEXT(__FILE__), MB_OK);
		exit(1);
	}

	hr = directInputObject->CreateDevice(GUID_SysMouse, &_pMouse, NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Failed to create Mouse Device", TEXT(__FILE__), MB_OK);
		exit(1);
	}

	hr = _pKeyBoard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Failed to set Keyboard DataFormat", TEXT(__FILE__), MB_OK);
		exit(1);
	}

	hr = _pKeyBoard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Failed to set Keyboard CooperativeLevel", TEXT(__FILE__), MB_OK);
		exit(1);
	}

	hr = _pMouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Failed to set Mouse DataFormat", TEXT(__FILE__), MB_OK);
		exit(1);
	}

	hr = _pMouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Failed to set Mouse CooperativeLevel", TEXT(__FILE__), MB_OK);
		exit(1);
	}

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