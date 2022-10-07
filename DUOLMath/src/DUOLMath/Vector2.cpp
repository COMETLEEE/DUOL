#include "DUOLMath\Vector2.h"
#include "DUOLMath\Constants.h"

namespace DUOLMath
{
	// static const numbers.
	const Vector2 Vector2::Zero = { 0.f, 0.f };
	const Vector2 Vector2::One = { 1.f, 1.f };
	const Vector2 Vector2::UnitX = { 1.f, 0.f };
	const Vector2 Vector2::UnitY = { 0.f, 1.f };

	// Comparison Operators
	bool Vector2::operator==(const Vector2& V) const noexcept
	{
		return (std::abs(this->x - V.x) <= DUOLMath::EPSILON) && (std::abs(this->y - V.y) <= DUOLMath::EPSILON);
	}

	bool Vector2::operator!=(const Vector2& V) const noexcept
	{
		return !(*this == V);
	}

	// Assignment Operators
	Vector2& Vector2::operator-=(const Vector2& V) noexcept
	{
		*this = (Vector2(this->x - V.x, this->y - V.y));

		return *this; 
	}

	Vector2& Vector2::operator*=(const Vector2& V) noexcept
	{
		*this = (Vector2(this->x * V.x, this->y * V.y));

		return *this;
	}

	Vector2& Vector2::operator*=(float S) noexcept
	{
		*this = (Vector2(this->x * S, this->y * S));

		return *this;
	}

	Vector2& Vector2::operator/=(float S) noexcept
	{
		assert(S != 0.0f);

		const float invS = 1 / S;

		*this = (Vector2(this->x * invS, this->y * invS));

		return *this;
	}

	Vector2& Vector2::operator+=(const Vector2& V) noexcept
	{
		*this = (Vector2(this->x + V.x, this->y + V.y));

		return *this;
	}

	// Positive 상한을 받아서 해당 원소들이 한도 범위 내에 (-value <= element <= value)
	// 있는지 체크합니다.
	bool Vector2::InBounds(const Vector2& Bounds) const noexcept
	{
		return ((this->x <= Bounds.x && this->x >= -Bounds.x) && (this->y <= Bounds.y && this->y >= -Bounds.y));
	}

	float Vector2::Length() const noexcept
	{
		return std::sqrt(this->x * this->x + this->y * this->y);
	}

	float Vector2::LengthSquared() const noexcept
	{
		const float length = this->Length();

		return length * length;
	}

	float Vector2::Dot(const Vector2& V) const noexcept
	{
		return this->x * V.x + this->y * V.y;
	}

	void Vector2::Cross(const Vector2& V, Vector2& result) const noexcept
	{
		result.x = this->x * V.y - this->y * V.x;

		result.y = this->x * V.y - this->y * V.x;
	}

	Vector2 Vector2::Cross(const Vector2& V) const noexcept
	{
		return Vector2{ this->x * V.y - this->y * V.x, this->x * V.y - this->y * V.x };
	}

	void Vector2::Normalize() noexcept
	{
		const float invLength = 1 / this->Length();

		this->x = this->x * invLength;

		this->y = this->y * invLength;
	}

	void Vector2::Normalize(Vector2& result) const noexcept
	{
		const float invLength = 1 / this->Length();

		result.x = this->x * invLength;

		result.y = this->y * invLength;
	}

	void Vector2::Clamp(const Vector2& vmin, const Vector2& vmax) noexcept
	{
		const Vector2 max = Vector2::Max(vmin, *this);

		*this = Vector2::Min(max, vmax);
	}

	void Vector2::Clamp(const Vector2& vmin, const Vector2& vmax, Vector2& result) const noexcept
	{
		const Vector2 max = Vector2::Max(vmin, *this);

		result = Vector2::Min(max, vmax);
	}

	float Vector2::Distance(const Vector2& v1, const Vector2& v2) noexcept
	{
		const Vector2 subVec = v1 - v2;

		return subVec.Length();
	}

	float Vector2::DistanceSquared(const Vector2& v1, const Vector2& v2) noexcept
	{
		const Vector2 subVec = v1 - v2;

		return subVec.LengthSquared();
	}

	void Vector2::Min(const Vector2& v1, const Vector2& v2, Vector2& result) noexcept
	{
		result.x = v1.x < v2.x ? v1.x : v2.x;

		result.y = v1.y < v2.y ? v1.y : v2.y;
	}

	Vector2 Vector2::Min(const Vector2& v1, const Vector2& v2) noexcept
	{
		Vector2 retVec;

		Vector2::Min(v1, v2, retVec);

		return retVec;
	}

	void Vector2::Max(const Vector2& v1, const Vector2& v2, Vector2& result) noexcept
	{
		result.x = v1.x > v2.x ? v1.x : v2.x;

		result.y = v1.y > v2.y ? v1.y : v2.y;
	}

	Vector2 Vector2::Max(const Vector2& v1, const Vector2& v2) noexcept
	{
		Vector2 retVec;

		Vector2::Max(v1, v2, retVec);

		return retVec;
	}

	void Vector2::Lerp(const Vector2& v1, const Vector2& v2, float t, Vector2& result) noexcept
	{
		result = v1 + t * (v2 - v1);
	}

	Vector2 Vector2::Lerp(const Vector2& v1, const Vector2& v2, float t) noexcept
	{
		Vector2 retVec;

		Vector2::Lerp(v1, v2, t, retVec);

		return retVec;
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
