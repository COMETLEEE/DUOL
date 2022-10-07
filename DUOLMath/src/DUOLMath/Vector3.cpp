#include "DUOLMath\Vector3.h"
#include "DUOLMath\Constants.h"

namespace DUOLMath
{
	const Vector3 Vector3::Zero = { 0.f, 0.f, 0.f };
	const Vector3 Vector3::One = { 1.f, 1.f, 1.f };
	const Vector3 Vector3::UnitX = { 1.f, 0.f, 0.f };
	const Vector3 Vector3::UnitY = { 0.f, 1.f, 0.f };
	const Vector3 Vector3::UnitZ = { 0.f, 0.f, 1.f };
	const Vector3 Vector3::Up = { 0.f, 1.f, 0.f };
	const Vector3 Vector3::Down = { 0.f, -1.f, 0.f };
	const Vector3 Vector3::Right = { 1.f, 0.f, 0.f };
	const Vector3 Vector3::Left = { -1.f, 0.f, 0.f };
	const Vector3 Vector3::Forward = { 0.f, 0.f, -1.f };
	const Vector3 Vector3::Backward = { 0.f, 0.f, 1.f };

	bool Vector3::operator==(const Vector3& V) const noexcept
	{
		// 모든 원소의 차이가 입실론보다 작으면, True를 반환한다.
		return (std::abs(this->x - V.x) <= DUOLMath::EPSILON) && (std::abs(this->y - V.y) <= DUOLMath::EPSILON) &&
			(std::abs(this->z - V.z) <= DUOLMath::EPSILON);
	}

	bool Vector3::operator!=(const Vector3& V) const noexcept
	{
		return !(*this == V);
	}

	Vector3& Vector3::operator+=(const Vector3& V) noexcept
	{
		*this = Vector3{ this->x + V.x, this->y + V.y, this->z + V.z };

		return *this;
	}

	Vector3& Vector3::operator-=(const Vector3& V) noexcept
	{
		*this = Vector3{ this->x - V.x, this->y - V.y, this->z - V.z };

		return *this;
	}

	Vector3& Vector3::operator*=(const Vector3& V) noexcept
	{
		*this = Vector3{ this->x * V.x, this->y * V.y, this->z * V.z };

		return *this;
	}

	Vector3& Vector3::operator*=(float S) noexcept
	{
		*this = Vector3{ this->x * S, this->y * S, this->z * S };

		return *this;
	}

	Vector3& Vector3::operator/=(float S) noexcept
	{
		assert(S != 0.0f);

		const float invS = 1 / S;

		*this = Vector3{ this->x * invS, this->y * invS, this->z * invS };

		return *this;
	}

	Vector3 Vector3::operator-() const noexcept
	{
		return Vector3{ -this->x, -this->y, -this->z };
	}

	bool Vector3::InBounds(const Vector3& Bounds) const noexcept
	{
		return ((this->x <= Bounds.x && this->x >= -Bounds.x) && (this->y <= Bounds.y && this->y >= -Bounds.y) 
			&& (this->z <= Bounds.z && this->z >= -Bounds.z));
	}

	float Vector3::Length() const noexcept
	{
		return std::sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
	}

	float Vector3::LengthSquared() const noexcept
	{
		const float length = this->Length();

		return length * length;
	}

	float Vector3::Dot(const Vector3& V) const noexcept
	{
		return this->x * V.x + this->y * V.y + this->z * V.z;
	}

	void Vector3::Cross(const Vector3& V, Vector3& result) const noexcept
	{
		// 외적 공식
		result.x = this->y * V.z - this->z * V.y;

		result.y = this->z * V.x - this->x * V.z;

		result.z = this->x * V.y - this->y * V.x;
	}

	Vector3 Vector3::Cross(const Vector3& V) const noexcept
	{
		return Vector3{ this->y * V.z - this->z * V.y, this->z * V.x - this->x * V.z, this->x * V.y - this->y * V.x };
	}

	void Vector3::Normalize() noexcept
	{
		const float invLength = 1 / this->Length();

		this->x = this->x * invLength;

		this->y = this->y * invLength;

		this->z = this->z * invLength;
	}

	void Vector3::Normalize(Vector3& result) const noexcept
	{
		const float invLength = 1 / this->Length();

		result.x = this->x * invLength;

		result.y = this->y * invLength;

		result.z = this->z * invLength;
	}

	void Vector3::Clamp(const Vector3& vmin, const Vector3& vmax) noexcept
	{
		const Vector3 max = Vector3::Max(vmin, *this);

		*this = Vector3::Min(max, vmax);
	}

	void Vector3::Clamp(const Vector3& vmin, const Vector3& vmax, Vector3& result) const noexcept
	{
		const Vector3 max = Vector3::Max(vmin, *this);

		result = Vector3::Min(max, vmax);
	}

	float Vector3::Distance(const Vector3& v1, const Vector3& v2) noexcept
	{
		const Vector3 subVec = v1 - v2;

		return subVec.Length();
	}

	float Vector3::DistanceSquared(const Vector3& v1, const Vector3& v2) noexcept
	{
		const Vector3 subVec = v1 - v2;

		return subVec.LengthSquared();
	}

	void Vector3::Min(const Vector3& v1, const Vector3& v2, Vector3& result) noexcept
	{
		result.x = v1.x < v2.x ? v1.x : v2.x;

		result.y = v1.y < v2.y ? v1.y : v2.y;

		result.z = v1.z < v2.z ? v1.z : v2.z;
	}

	Vector3 Vector3::Min(const Vector3& v1, const Vector3& v2) noexcept
	{
		Vector3 retVec;

		Vector3::Min(v1, v2, retVec);

		return retVec;
	}

	void Vector3::Max(const Vector3& v1, const Vector3& v2, Vector3& result) noexcept
	{
		result.x = v1.x > v2.x ? v1.x : v2.x;

		result.y = v1.y > v2.y ? v1.y : v2.y;

		result.z = v1.z > v2.z ? v1.z : v2.z;
	}

	Vector3 Vector3::Max(const Vector3& v1, const Vector3& v2) noexcept
	{
		Vector3 retVec;

		Vector3::Max(v1, v2, retVec);

		return retVec;
	}

	void Vector3::Lerp(const Vector3& v1, const Vector3& v2, float t, Vector3& result) noexcept
	{
		result = v1 + t * (v2 - v1);
	}

	Vector3 Vector3::Lerp(const Vector3& v1, const Vector3& v2, float t) noexcept
	{
		Vector3 retVec;

		Vector3::Lerp(v1, v2, t, retVec);

		return retVec;
	}

	void Vector3::SmoothStep(const Vector3& v1, const Vector3& v2, float t, Vector3& result) noexcept
	{

	}

	Vector3 Vector3::SmoothStep(const Vector3& v1, const Vector3& v2, float t) noexcept
	{

	}

	void Vector3::Barycentric(const Vector3& v1, const Vector3& v2, const Vector3& v3, float f, float g,
		Vector3& result) noexcept
	{

	}

	Vector3 Vector3::Barycentric(const Vector3& v1, const Vector3& v2, const Vector3& v3, float f, float g) noexcept
	{

	}

	void Vector3::CatmullRom(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& v4, float t,
		Vector3& result) noexcept
	{
	}

	Vector3 Vector3::CatmullRom(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& v4,
		float t) noexcept
	{
	}

	void Vector3::Hermite(const Vector3& v1, const Vector3& t1, const Vector3& v2, const Vector3& t2, float t,
		Vector3& result) noexcept
	{
	}

	Vector3 Vector3::Hermite(const Vector3& v1, const Vector3& t1, const Vector3& v2, const Vector3& t2,
		float t) noexcept
	{
	}

	void Vector3::Reflect(const Vector3& ivec, const Vector3& nvec, Vector3& result) noexcept
	{
	}

	Vector3 Vector3::Reflect(const Vector3& ivec, const Vector3& nvec) noexcept
	{
	}

	void Vector3::Refract(const Vector3& ivec, const Vector3& nvec, float refractionIndex, Vector3& result) noexcept
	{
	}

	Vector3 Vector3::Refract(const Vector3& ivec, const Vector3& nvec, float refractionIndex) noexcept
	{
	}

	void Vector3::Transform(const Vector3& v, const Quaternion& quat, Vector3& result) noexcept
	{
	}

	Vector3 Vector3::Transform(const Vector3& v, const Quaternion& quat) noexcept
	{
	}

	void Vector3::Transform(const Vector3& v, const Matrix& m, Vector3& result) noexcept
	{
	}

	Vector3 Vector3::Transform(const Vector3& v, const Matrix& m) noexcept
	{
	}

	void Vector3::Transform(const Vector3* varray, size_t count, const Matrix& m, Vector3* resultArray) noexcept
	{
	}

	void Vector3::Transform(const Vector3& v, const Matrix& m, Vector4& result) noexcept
	{
	}

	void Vector3::Transform(const Vector3* varray, size_t count, const Matrix& m, Vector4* resultArray) noexcept
	{
	}

	void Vector3::TransformNormal(const Vector3& v, const Matrix& m, Vector3& result) noexcept
	{
	}

	Vector3 Vector3::TransformNormal(const Vector3& v, const Matrix& m) noexcept
	{
	}

	void Vector3::TransformNormal(const Vector3* varray, size_t count, const Matrix& m, Vector3* resultArray) noexcept
	{
	}
}
