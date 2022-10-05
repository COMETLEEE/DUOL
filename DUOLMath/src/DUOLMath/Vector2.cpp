#include "DUOLMath\Vector2.h"

namespace MathLibrary
{
	// static const numbers.
	const Vector2 Vector2::Zero = { 0.f, 0.f };
	const Vector2 Vector2::One = { 1.f, 1.f };
	const Vector2 Vector2::UnitX = { 1.f, 0.f };
	const Vector2 Vector2::UnitY = { 0.f, 1.f };

	bool Vector2::operator==(const Vector2& V) const noexcept
	{

	}

	bool Vector2::operator!=(const Vector2& V) const noexcept
	{

	}

	Vector2& Vector2::operator-=(const Vector2& V) noexcept
	{

	}

	Vector2& Vector2::operator*=(const Vector2& V) noexcept
	{

	}

	Vector2& Vector2::operator*=(float S) noexcept
	{

	}

	Vector2& Vector2::operator/=(float S) noexcept
	{

	}

	Vector2& Vector2::operator+=(const Vector2& V) noexcept
	{

	}

	bool Vector2::InBounds(const Vector2& Bounds) const noexcept
	{

	}

	float Vector2::Length() const noexcept
	{

	}

	float Vector2::LengthSquared() const noexcept
	{

	}

	float Vector2::Dot(const Vector2& V) const noexcept
	{

	}

	void Vector2::Cross(const Vector2& V, Vector2& result) const noexcept
	{

	}

	Vector2 Vector2::Cross(const Vector2& V) const noexcept
	{

	}

	void Vector2::Normalize() noexcept
	{

	}

	void Vector2::Normalize(Vector2& result) const noexcept
	{

	}

	void Vector2::Clamp(const Vector2& vmin, const Vector2& vmax) noexcept
	{

	}

	void Vector2::Clamp(const Vector2& vmin, const Vector2& vmax, Vector2& result) const noexcept
	{

	}

	float Vector2::Distance(const Vector2& v1, const Vector2& v2) noexcept
	{

	}

	float Vector2::DistanceSquared(const Vector2& v1, const Vector2& v2) noexcept
	{

	}

	void Vector2::Min(const Vector2& v1, const Vector2& v2, Vector2& result) noexcept
	{

	}

	Vector2 Vector2::Min(const Vector2& v1, const Vector2& v2) noexcept
	{

	}

	void Vector2::Max(const Vector2& v1, const Vector2& v2, Vector2& result) noexcept
	{

	}

	Vector2 Vector2::Max(const Vector2& v1, const Vector2& v2) noexcept
	{

	}

	void Vector2::Lerp(const Vector2& v1, const Vector2& v2, float t, Vector2& result) noexcept
	{

	}

	Vector2 Vector2::Lerp(const Vector2& v1, const Vector2& v2, float t) noexcept
	{

	}

	void Vector2::SmoothStep(const Vector2& v1, const Vector2& v2, float t, Vector2& result) noexcept
	{

	}

	Vector2 Vector2::SmoothStep(const Vector2& v1, const Vector2& v2, float t) noexcept
	{

	}

	void Vector2::Barycentric(const Vector2& v1, const Vector2& v2, const Vector2& v3, float f, float g, Vector2& result) noexcept
	{

	}

	Vector2 Vector2::Barycentric(const Vector2& v1, const Vector2& v2, const Vector2& v3, float f, float g) noexcept
	{

	}

	void Vector2::CatmullRom(const Vector2& v1, const Vector2& v2, const Vector2& v3, const Vector2& v4, float t, Vector2& result) noexcept
	{

	}

	Vector2 Vector2::CatmullRom(const Vector2& v1, const Vector2& v2, const Vector2& v3, const Vector2& v4, float t) noexcept
	{

	}

	void Vector2::Hermite(const Vector2& v1, const Vector2& t1, const Vector2& v2, const Vector2& t2, float t, Vector2& result) noexcept
	{

	}

	Vector2 Vector2::Hermite(const Vector2& v1, const Vector2& t1, const Vector2& v2, const Vector2& t2, float t) noexcept
	{

	}

	void Vector2::Reflect(const Vector2& ivec, const Vector2& nvec, Vector2& result) noexcept
	{

	}

	Vector2 Vector2::Reflect(const Vector2& ivec, const Vector2& nvec) noexcept
	{

	}

	void Vector2::Refract(const Vector2& ivec, const Vector2& nvec, float refractionIndex, Vector2& result) noexcept
	{

	}

	Vector2 Vector2::Refract(const Vector2& ivec, const Vector2& nvec, float refractionIndex) noexcept
	{

	}

	void Vector2::Transform(const Vector2& v, const Quaternion& quat, Vector2& result) noexcept
	{

	}

	Vector2 Vector2::Transform(const Vector2& v, const Quaternion& quat) noexcept
	{

	}

	void Vector2::Transform(const Vector2& v, const Matrix& m, Vector2& result) noexcept
	{

	}

	Vector2 Vector2::Transform(const Vector2& v, const Matrix& m) noexcept
	{

	}

	void Vector2::Transform(_In_reads_(count) const Vector2* varray, size_t count, const Matrix& m, _Out_writes_(count) Vector2* resultArray) noexcept
	{

	}

	void Vector2::Transform(const Vector2& v, const Matrix& m, Vector4& result) noexcept
	{

	}

	void Vector2::Transform(_In_reads_(count) const Vector2* varray, size_t count, const Matrix& m, _Out_writes_(count) Vector4* resultArray) noexcept
	{

	}

	void Vector2::TransformNormal(const Vector2& v, const Matrix& m, Vector2& result) noexcept
	{

	}

	Vector2 Vector2::TransformNormal(const Vector2& v, const Matrix& m) noexcept
	{

	}

	void Vector2::TransformNormal(_In_reads_(count) const Vector2* varray, size_t count, const Matrix& m, _Out_writes_(count) Vector2* resultArray) noexcept
	{

	}
}