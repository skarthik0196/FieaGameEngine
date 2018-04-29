#include "pch.h"
#include "CameraBase.h"

namespace Rendering
{
	CameraBase::CameraBase(float nearPlane, float farPlane ) : NearPlane(nearPlane), FarPlane(farPlane), Position(DirectX::XMFLOAT3(0.0f,0.0f,0.0f)), Target(DirectX::XMFLOAT3(0.0f,0.0f,1.0f)), Angle(1.0f), ScreenWidth(1024), ScreenHeight(768)
	{
		DirectX::XMFLOAT3 temp(0.0f, 1.0f, 0.0f);
		DirectX::XMVECTOR upVector = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&Position), DirectX::XMLoadFloat3(&temp));
		DirectX::XMStoreFloat3(&UpVectorEndPoint, upVector);
		
		InitializeViewMatrix();
		InitializePerspectiveProjectionMatrix();
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

	void CameraBase::SetAngle(float angle)
	{
		Angle = angle;
	}

	float CameraBase::GetAngle() const
	{
		return Angle;
	}

	float CameraBase::GetScreenWidth()
	{
		return ScreenWidth;
	}

	void CameraBase::SetScreenWidth(float screenWidth)
	{
		ScreenWidth = screenWidth;
	}

	float CameraBase::GetScreenHeight()
	{
		return ScreenHeight;
	}

	void CameraBase::SetScreenHeight(float screenHeight)
	{
		ScreenHeight = screenHeight;
	}

	void CameraBase::SetNearPlane(float nearPlane)
	{
		NearPlane = nearPlane;
	}

	void CameraBase::SetFarPlane(float farPlane)
	{
		FarPlane = farPlane;
	}

	float CameraBase::GetNearPlane() const
	{
		return NearPlane;
	}

	float CameraBase::GetFarPlane() const
	{
		return FarPlane;
	}

	const DirectX::XMFLOAT4X4 & CameraBase::GetViewMatrixAsFloat4x4() const
	{
		return ViewMatrix;
	}

	DirectX::XMMATRIX CameraBase::GetViewMatrix() const
	{
		return DirectX::XMLoadFloat4x4(&ViewMatrix);
	}

	const DirectX::XMFLOAT4X4 & CameraBase::GetProjectionMatrixAsFloat4x4() const
	{
		return ProjectionMatrix;
	}

	DirectX::XMMATRIX CameraBase::GetProjectionMatrix() const
	{
		
		return DirectX::XMLoadFloat4x4(&ProjectionMatrix);
	}

	const DirectX::XMFLOAT4X4 & CameraBase::GetViewProjectionMatrixAsFloat4x4()
	{
		DirectX::XMStoreFloat4x4(&ViewProjectionMatrix, DirectX::XMLoadFloat4x4(&ViewMatrix) * DirectX::XMLoadFloat4x4(&ProjectionMatrix));
		return ViewProjectionMatrix;
	}

	DirectX::XMMATRIX CameraBase::GetViewProjectionMatrix()
	{
		DirectX::XMStoreFloat4x4(&ViewProjectionMatrix, DirectX::XMLoadFloat4x4(&ViewMatrix) * DirectX::XMLoadFloat4x4(&ProjectionMatrix));
		return DirectX::XMLoadFloat4x4(&ViewProjectionMatrix);
	}

	void CameraBase::InitializeViewMatrix()
	{
		DirectX::XMStoreFloat4x4(&ViewMatrix, DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat3(&Position), DirectX::XMLoadFloat3(&Target), GetUpVector()));
	}

	void CameraBase::InitializePerspectiveProjectionMatrix()
	{
		DirectX::XMStoreFloat4x4(&ProjectionMatrix, DirectX::XMMatrixPerspectiveFovLH(Angle, (ScreenWidth / ScreenHeight), 0.01f, FarPlane));

		//DirectX::XMStoreFloat4x4(&ViewProjectionMatrix, DirectX::XMLoadFloat4x4(&ViewMatrix) * DirectX::XMLoadFloat4x4(&ProjectionMatrix));
	}

	void CameraBase::Move(const DirectX::XMFLOAT3& translation)
	{
		DirectX::XMVECTOR temp = DirectX::XMLoadFloat3(&translation);
		Move(temp);
	}

	void CameraBase::Move(const DirectX::XMVECTOR& translation)
	{
		DirectX::XMVECTOR temp = DirectX::XMLoadFloat3(&Position);
		temp = DirectX::XMVector3Transform(temp, DirectX::XMMatrixTranslation(DirectX::XMVectorGetX(translation), DirectX::XMVectorGetY(translation), DirectX::XMVectorGetZ(translation)));
		DirectX::XMStoreFloat3(&Position, temp);

		temp = DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&Target), DirectX::XMMatrixTranslation(DirectX::XMVectorGetX(translation), DirectX::XMVectorGetY(translation), DirectX::XMVectorGetZ(translation)));
		DirectX::XMStoreFloat3(&Target, temp);

		temp = DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&UpVectorEndPoint), DirectX::XMMatrixTranslation(DirectX::XMVectorGetX(translation), DirectX::XMVectorGetY(translation), DirectX::XMVectorGetZ(translation)));
		DirectX::XMStoreFloat3(&UpVectorEndPoint, temp);

		InitializeViewMatrix();
	}

	void CameraBase::Rotate(const DirectX::XMFLOAT3& axis, float angleInDegrees)
	{
		DirectX::XMVECTOR temp = DirectX::XMLoadFloat3(&axis);
		Rotate(temp, angleInDegrees);
	}

	void CameraBase::Rotate(const DirectX::XMVECTOR& axis, float angleInDegrees)
	{
		if ((DirectX::XMVector3Equal(DirectX::XMVectorZero(), axis)) || (angleInDegrees == 0.0f))
		{
			return;
		}

		DirectX::XMVECTOR targetLookAt =  DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&Target), DirectX::XMLoadFloat3(&Position));
		DirectX::XMVECTOR upVectorLookAt = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&UpVectorEndPoint), DirectX::XMLoadFloat3(&Position));

		targetLookAt = DirectX::XMVector3Transform(targetLookAt, DirectX::XMMatrixRotationAxis(axis, DirectX::XMConvertToRadians(angleInDegrees)));
		upVectorLookAt = DirectX::XMVector3Transform(upVectorLookAt, DirectX::XMMatrixRotationAxis(axis, DirectX::XMConvertToRadians(angleInDegrees)));

		DirectX::XMStoreFloat3(&Target, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&Position), targetLookAt));
		DirectX::XMStoreFloat3(&UpVectorEndPoint, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&Position), upVectorLookAt));

		InitializeViewMatrix();
	}

	void CameraBase::SetPosition(const DirectX::XMFLOAT3& position)
	{
		DirectX::XMVECTOR temp = DirectX::XMLoadFloat3(&position);
		SetPosition(temp);
	}

	void CameraBase::SetPosition(const DirectX::XMVECTOR& position)
	{
		DirectX::XMVECTOR moveVector = DirectX::XMVectorSubtract(position, DirectX::XMLoadFloat3(&Position));
		Move(moveVector);
	}
}