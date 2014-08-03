#include <Engine/Camera.hpp>

engine::Camera::Camera(void)
{
	_pcamera = new DirectX::XMFLOAT3;
	_ptarget = new DirectX::XMFLOAT3;
	_projectionMatrix = (DirectX::XMMATRIX *)_aligned_malloc(sizeof *_projectionMatrix, 16);
	_VP = (DirectX::XMMATRIX *)_aligned_malloc(sizeof *_VP, 16);
	_pcamera->x = 0;
	_pcamera->y = 0;
	_pcamera->z = 0;
}

engine::Camera::Camera(const FLOAT &x, const FLOAT &y, const FLOAT &z)
{
	_pcamera = new DirectX::XMFLOAT3;
	_ptarget = new DirectX::XMFLOAT3;
	_projectionMatrix = (DirectX::XMMATRIX *)_aligned_malloc(sizeof *_projectionMatrix, 16);
	_VP = (DirectX::XMMATRIX *)_aligned_malloc(sizeof *_VP, 16);
	_pcamera->x = x;
	_pcamera->y = y;
	_pcamera->z = z;
}

engine::Camera::~Camera(void)
{
	delete _pcamera;
	delete _ptarget;
	_aligned_free(_projectionMatrix);
	_aligned_free(_VP);
}

void engine::Camera::setPositionCamera(const FLOAT &x, const FLOAT &y, const FLOAT &z)
{
	_pcamera->x = x;
	_pcamera->y = y;
	_pcamera->z = z;
}

void engine::Camera::setPositionTarget(const FLOAT &x, const FLOAT &y, const FLOAT &z)
{
	_ptarget->x = x;
	_ptarget->y = y;
	_ptarget->z = z;
}

void engine::Camera::setPerspective(const FLOAT &fov, const UINT &width, const UINT &height, const FLOAT &n, const FLOAT &f)
{
	_width = width;
	_height = height;
	*_projectionMatrix = DirectX::XMMatrixPerspectiveFovRH(fov*((FLOAT)DirectX::XM_PI / 180), (FLOAT)_width / (FLOAT)_height, n, f);
}

DirectX::XMFLOAT3 engine::Camera::getPositionCamera(void) const
{
	return *_pcamera;
}

DirectX::XMFLOAT3 engine::Camera::getPositionTarget(void) const
{
	return *_ptarget;
}

UINT engine::Camera::getWidth(void) const
{
	return _width;
}

UINT engine::Camera::getHeight(void) const
{
	return _height;
}

DirectX::XMMATRIX engine::Camera::getVPMatrix(void)
{
	return *_VP;
}

void engine::Camera::position(void)
{
	DirectX::XMVECTOR camera = DirectX::XMVectorSet(_pcamera->x, _pcamera->y, _pcamera->z, 0.0f);
	DirectX::XMVECTOR target = DirectX::XMVectorSet(_ptarget->x, _ptarget->y, _ptarget->z, 0.0f);
	DirectX::XMVECTOR head = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	DirectX::XMMATRIX view;

	view = DirectX::XMMatrixLookAtRH(camera, target, head);
	*_VP = XMMatrixTranspose(*_projectionMatrix) * XMMatrixTranspose(view);
}
