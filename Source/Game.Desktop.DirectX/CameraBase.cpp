#include "pch.h"
#include "CameraBase.h"

namespace Rendering
{
	CameraBase::CameraBase(float nearPlane, float farPlane ) : NearPlane(nearPlane), FarPlane(farPlane), Position(DirectX::XMFLOAT3(0.0f,0.0f,0.0f)), Target(DirectX::XMFLOAT3(0.0f,0.0f,1.0f)), Angle(0.0f), ScreenWidth(1024), ScreenHeight(768)
	{
		DirectX::XMFLOAT3 temp(0.0f, 1.0f, 0.0f);
		DirectX::XMVECTOR upVector = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&Position), DirectX::XMLoadFloat3(&temp));
		DirectX::XMStoreFloat3(&UpVectorEndPoint, upVector);
		
		InitializeViewMatrix();
	}

	CameraBase::~CameraBase()
	{
	}

	DirectX::XMVECTOR CameraBase::GetUpVector() const
	{
		return DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&UpVectorEndPoint), DirectX::XMLoadFloat3(&Position));
	}

	DirectX::XMFLOAT3 CameraBase::GetUpVectorAsFloat3() const
	{
		DirectX::XMFLOAT3 temp;
		DirectX::XMStoreFloat3(&temp, GetUpVector());
		return temp;
	}

	DirectX::XMVECTOR CameraBase::GetPosition() const
	{
		return DirectX::XMLoadFloat3(&Position);
	}

	const DirectX::XMFLOAT3& CameraBase::GetPositionAsFloat3() const
	{
		return Position;
	}

	void CameraBase::InitializeViewMatrix()
	{
		DirectX::XMStoreFloat4x4(&ViewMatrix, DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat3(&Position), DirectX::XMLoadFloat3(&Target), GetUpVector()));
	}
}