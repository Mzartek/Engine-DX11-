#include <Engine/FrameBuffer.hpp>

engine::FrameBuffer::FrameBuffer(void)
{
	// Device and Context
	_pd3dDevice = NULL;
	_pContext = NULL;
	// DefferedContext
	_pDeferredContext = NULL;
}

engine::FrameBuffer::~FrameBuffer(void)
{
	if (_pDeferredContext)
	{
		_pDeferredContext->ClearState();
		_pDeferredContext->Release();
	}
}

UINT engine::FrameBuffer::getWidth(void) const
{
	return _width;
}

UINT engine::FrameBuffer::getHeight(void) const
{
	return _height;
}

ID3D11DeviceContext *engine::FrameBuffer::getContext(void) const
{
	return _pDeferredContext;
}

void engine::FrameBuffer::executeContext(void) const
{
	ID3D11CommandList *commandList;
	if (_pContext == NULL)
	{
		MessageBox(NULL, "Need to config FrameBuffer before execution", "FrameBuffer", MB_OK);
		exit(1);
	}

	_pDeferredContext->FinishCommandList(TRUE, &commandList);
	_pContext->ExecuteCommandList(commandList, TRUE);
	commandList->Release();
}