#include <Engine/FrameBuffer.hpp>

engine::FrameBuffer::FrameBuffer(void)
{
	_pd3dDevice = NULL;
	_pContext = NULL;
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

void engine::FrameBuffer::executeDeferredContext(void) const
{
	ID3D11CommandList *pCommandList;
	_pDeferredContext->FinishCommandList(TRUE, &pCommandList);
	_pContext->ExecuteCommandList(pCommandList, TRUE);
	pCommandList->Release();
}