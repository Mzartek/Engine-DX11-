#include <Engine/Camera.hpp>

engine::Camera::Camera(void)
{
	_pcamera = (XMVECTOR *)_aligned_malloc(sizeof *_pcamera, 16);
	_ptarget = (XMVECTOR *)_aligned_malloc(sizeof *_ptarget, 16);
	_projectionMatrix = (XMMATRIX *)_aligned_malloc(sizeof *_projectionMatrix, 16);
	_VPMatrix = (XMMATRIX *)_aligned_malloc(sizeof *_VPMatrix, 16);

	*_pcamera = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
}

engine::Camera::Camera(const FLOAT &x, const FLOAT &y, const FLOAT &z)
{
	_pcamera = (XMVECTOR *)_aligned_malloc(sizeof *_pcamera, 16);
	_ptarget = (XMVECTOR *)_aligned_malloc(sizeof *_ptarget, 16);
	_projectionMatrix = (XMMATRIX *)_aligned_malloc(sizeof *_projectionMatrix, 16);
	_VPMatrix = (XMMATRIX *)_aligned_malloc(sizeof *_VPMatrix, 16);

	*_pcamera = XMVectorSet(x, y, z, 0.0f);
}

engine::Camera::~Camera(void)
{
	_aligned_free(_pcamera);
	_aligned_free(_ptarget);
	_aligned_free(_projectionMatrix);
	_aligned_free(_VPMatrix);
}

void engine::Camera::setPositionCamera(const XMFLOAT3 &pos)
{
	*_pcamera = XMVectorSet(pos.x, pos.y, pos.z, 0.0f);
}

void engine::Camera::setPositionTarget(const XMFLOAT3 &pos)
{
	*_ptarget = XMVectorSet(pos.x, pos.y, pos.z, 0.0f);
}

void engine::Camera::setPerspective(const FLOAT &fov, const UINT &width, const UINT &height, const FLOAT &n, const FLOAT &f)
{
	*_projectionMatrix = XMMatrixPerspectiveFovRH(fov*((FLOAT)XM_PI / 180), (FLOAT)width / height, n, f);
}

XMFLOAT3 engine::Camera::getPositionCamera(void) const
{
	return XMFLOAT3(XMVectorGetX(*_pcamera), XMVectorGetY(*_pcamera), XMVectorGetZ(*_pcamera));
}

XMFLOAT3 engine::Camera::getPositionTarget(void) const
{
	return XMFLOAT3(XMVectorGetX(*_ptarget), XMVectorGetY(*_ptarget), XMVectorGetZ(*_ptarget));
}

XMMATRIX engine::Camera::getProjectionMatrix(void) const
{
	return *_projectionMatrix;
}

XMMATRIX engine::Camera::getVPMatrix(void) const
{
	return *_VPMatrix;
}

void engine::Camera::position(void)
{
	*_VPMatrix = XMMatrixLookAtRH(*_pcamera, *_ptarget, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)) * *_projectionMatrix;
}
