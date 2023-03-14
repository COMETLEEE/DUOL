#include "DUOLMath/DUOLMath.h"

namespace DUOLMath
{
	std::random_device MathHelper::_rd;

	const Vector2 Vector2::Zero = { 0.f, 0.f };
	const Vector2 Vector2::One = { 1.f, 1.f };
	const Vector2 Vector2::UnitX = { 1.f, 0.f };
	const Vector2 Vector2::UnitY = { 0.f, 1.f };

	const Vector3 Vector3::Zero = { 0.f, 0.f, 0.f };
	const Vector3 Vector3::One = { 1.f, 1.f, 1.f };
	const Vector3 Vector3::UnitX = { 1.f, 0.f, 0.f };
	const Vector3 Vector3::UnitY = { 0.f, 1.f, 0.f };
	const Vector3 Vector3::UnitZ = { 0.f, 0.f, 1.f };
	const Vector3 Vector3::Up = { 0.f, 1.f, 0.f };
	const Vector3 Vector3::Down = { 0.f, -1.f, 0.f };
	const Vector3 Vector3::Right = { 1.f, 0.f, 0.f };
	const Vector3 Vector3::Left = { -1.f, 0.f, 0.f };
	const Vector3 Vector3::Forward = { 0.f, 0.f, 1.f };
	const Vector3 Vector3::Backward = { 0.f, 0.f, -1.f };

	const Vector4 Vector4::Zero = { 0.f, 0.f, 0.f, 0.f };
	const Vector4 Vector4::One = { 1.f, 1.f, 1.f, 1.f };
	const Vector4 Vector4::UnitX = { 1.f, 0.f, 0.f, 0.f };
	const Vector4 Vector4::UnitY = { 0.f, 1.f, 0.f, 0.f };
	const Vector4 Vector4::UnitZ = { 0.f, 0.f, 1.f, 0.f };
	const Vector4 Vector4::UnitW = { 0.f, 0.f, 0.f, 1.f };

	const Matrix Matrix::Identity = { 1.f, 0.f, 0.f, 0.f,
									  0.f, 1.f, 0.f, 0.f,
									  0.f, 0.f, 1.f, 0.f,
									  0.f, 0.f, 0.f, 1.f };

	const Quaternion Quaternion::Identity = { 0.f, 0.f, 0.f, 1.f };
}