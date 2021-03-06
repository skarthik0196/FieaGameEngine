#pragma once
#include <d3d11_2.h>
#include <DirectXPackedVector.h>
#include <DirectXMath.h>

namespace Rendering
{
	class CameraBase
	{
	public:
		CameraBase(float nearPlane = 0.01f, float farPlane = 10000.0f);

		CameraBase(const CameraBase& rhs) = default;
		CameraBase(CameraBase&& rhs) = default;

		CameraBase& operator=(const CameraBase& rhs) = default;
		CameraBase& operator=(CameraBase&& rhs) = default;
		~CameraBase();

		DirectX::XMVECTOR GetUpVector() const;
		DirectX::XMFLOAT3 GetUpVectorAsFloat3() const;

		DirectX::XMVECTOR GetPosition() const;
		const DirectX::XMFLOAT3& GetPositionAsFloat3() const;

		void SetAngle(float angle);
		float GetAngle() const;

		float GetScreenWidth();
		void SetScreenWidth(float screenWidth);

		float GetScreenHeight();
		void SetScreenHeight(float screenHeight);

		void SetNearPlane(float nearPlane);
		void SetFarPlane(float farPlane);

		float GetNearPlane() const;
		float GetFarPlane() const;
		
		const DirectX::XMFLOAT4X4& GetViewMatrixAsFloat4x4() const;
		DirectX::XMMATRIX GetViewMatrix() const;

		const DirectX::XMFLOAT4X4& GetProjectionMatrixAsFloat4x4() const;
		DirectX::XMMATRIX GetProjectionMatrix() const;

		const DirectX::XMFLOAT4X4& GetViewProjectionMatrixAsFloat4x4();
		DirectX::XMMATRIX GetViewProjectionMatrix();

		void InitializeViewMatrix();
		void InitializePerspectiveProjectionMatrix();

		void Move(const DirectX::XMFLOAT3& translation);
		void Move(const DirectX::XMVECTOR& translation);

		void Rotate(const DirectX::XMFLOAT3& axis, float angleInDegrees);
		void Rotate(const DirectX::XMVECTOR& axis, float angleInDegrees);

		void SetPosition(const DirectX::XMFLOAT3& position);
		void SetPosition(const DirectX::XMVECTOR& position);

	private:
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT3 Target;
		DirectX::XMFLOAT3 UpVectorEndPoint;

		DirectX::XMFLOAT4X4 ViewMatrix;
		DirectX::XMFLOAT4X4 ProjectionMatrix;
		DirectX::XMFLOAT4X4 ViewProjectionMatrix;

		float Angle;
		float NearPlane;
		float FarPlane;
		float ScreenWidth;
		float ScreenHeight;
	};
}