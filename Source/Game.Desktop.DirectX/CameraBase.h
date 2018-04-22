#pragma once
#include <d3d11_2.h>
#include <DirectXPackedVector.h>
#include <DirectXMath.h>

namespace Rendering
{
	class CameraBase
	{
	public:
		CameraBase(float nearPlane = 0.0001f, float farPlane = 10000.0f);

		CameraBase(const CameraBase& rhs) = default;
		CameraBase(CameraBase&& rhs) = default;

		CameraBase& operator=(const CameraBase& rhs) = default;
		CameraBase& operator=(CameraBase&& rhs) = default;
		~CameraBase();

		DirectX::XMVECTOR GetUpVector() const;
		DirectX::XMFLOAT3 GetUpVectorAsFloat3() const;

		DirectX::XMVECTOR GetPosition() const;
		const DirectX::XMFLOAT3& GetPositionAsFloat3() const;

		void InitializeViewMatrix();

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