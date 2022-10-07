#include "DUOLMath\Vector4.h"
#include "DUOLMath\Constants.h"

namespace DUOLMath
{
	// STATIC VALUE
	const Vector4 Vector4::Zero = { 0.f, 0.f, 0.f, 0.f };
	const Vector4 Vector4::One = { 1.f, 1.f, 1.f, 1.f };
	const Vector4 Vector4::UnitX = { 1.f, 0.f, 0.f, 0.f };
	const Vector4 Vector4::UnitY = { 0.f, 1.f, 0.f, 0.f };
	const Vector4 Vector4::UnitZ = { 0.f, 0.f, 1.f, 0.f };
	const Vector4 Vector4::UnitW = { 0.f, 0.f, 0.f, 1.f };

	bool Vector4::operator==(const Vector4& V) const noexcept
	{
		// 모든 원소의 차이가 입실론보다 작으면, True를 반환한다.
		return ((std::abs(this->x - V.x) <= DUOLMath::EPSILON) && (std::abs(this->y - V.y) <= DUOLMath::EPSILON) &&
			(std::abs(this->z - V.z) <= DUOLMath::EPSILON) && (std::abs(this->w - V.w) <= DUOLMath::EPSILON));
	}

	bool Vector4::operator!=(const Vector4& V) const noexcept
	{
		return !(*this == V);
	}

	Vector4& Vector4::operator+=(const Vector4& V) noexcept
	{
		*this = Vector4{ this->x + V.x, this->y + V.y, this->z + V.z, this->w + V.w };

		return *this;
	}

	Vector4& Vector4::operator-=(const Vector4& V) noexcept
	{
		*this = Vector4{ this->x - V.x, this->y - V.y, this->z - V.z, this->w - V.w };

		return *this;
	}

	Vector4& Vector4::operator*=(const Vector4& V) noexcept
	{
		*this = Vector4{ this->x * V.x, this->y * V.y, this->z * V.z, this->w * V.w };

		return *this;
	}

	Vector4& Vector4::operator*=(float S) noexcept
	{
		*this = Vector4{ this->x * S, this->y * S, this->z * S, this->w * S };

		return *this;
	}

	Vector4& Vector4::operator/=(float S) noexcept
	{
		assert(S != 0.f);

		const float invS = 1 / S;

		*this = Vector4{ this->x * invS, this->y * invS, this->z * invS, this->w * invS };

		return *this;
	}

	Vector4 Vector4::operator-() const noexcept
	{
		return Vector4{ -this->x, -this->y, -this->z, -this->w };
	}

	bool Vector4::InBounds(const Vector4& Bounds) const noexcept
	{
		return ((this->x <= Bounds.x && this->x >= -Bounds.x) && (this->y <= Bounds.y && this->y >= -Bounds.y)
			&& (this->z <= Bounds.z && this->z >= -Bounds.z) && (this->w <= Bounds.w && this->w >= -Bounds.w));
	}

	float Vector4::Length() const noexcept
	{
		return std::sqrt(this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w);
	}

	float Vector4::LengthSquared() const noexcept
	{
		const float length = this->Length();

		return length * length;
	}

	float Vector4::Dot(const Vector4& V) const noexcept
	{
		return this->x * V.x + this->y * V.y + this->z * V.z + this->w * V.w;
	}

	void Vector4::Cross(const Vector4& v1, const Vector4& v2, Vector4& result) const noexcept
	{
		result.x = ((v1.z * v2.w - v1.w * v2.z) * this->y) - ((v1.y * v2.w - v1.w * v2.y) * this->z) + ((v1.y * v2.z - v1.z * v2.y) * this->w);

		result.y = ((v1.w * v2.z - v1.z * v2.w) * this->x) - ((v1.w * v2.x - v1.x * v2.w) * this->z) + ((v1.z * v2.x - v1.x * v2.z) * this->w);

		result.z = ((v1.y * v2.w - v1.w * v2.y) * this->x) - ((v1.x * v2.w - v1.w * v2.x) * this->y) + ((v1.x * v2.y - v1.y * v2.x) * this->w);

		result.w = ((v1.z * v2.y - v1.y * v2.z) * this->x) - ((v1.z * v2.x - v1.x * v2.z) * this->y) + ((v1.y * v2.x - v1.x * v2.y) * this->z);
	}

	Vector4 Vector4::Cross(const Vector4& v1, const Vector4& v2) const noexcept
	{
		return Vector4{ ((v1.z * v2.w - v1.w * v2.z) * this->y) - ((v1.y * v2.w - v1.w * v2.y) * this->z) + ((v1.y * v2.z - v1.z * v2.y) * this->w),
			   ((v1.w * v2.z - v1.z * v2.w) * this->x) - ((v1.w * v2.x - v1.x * v2.w) * this->z) + ((v1.z * v2.x - v1.x * v2.z) * this->w),
			   ((v1.y * v2.w - v1.w * v2.y) * this->x) - ((v1.x * v2.w - v1.w * v2.x) * this->y) + ((v1.x * v2.y - v1.y * v2.x) * this->w),
			   ((v1.z * v2.y - v1.y * v2.z) * this->x) - ((v1.z * v2.x - v1.x * v2.z) * this->y) + ((v1.y * v2.x - v1.x * v2.y) * this->z) };
	}

	void Vector4::Normalize() noexcept
	{
		const float invLength = 1 / this->Length();

		this->x = this->x * invLength;

		this->y = this->y * invLength;

		this->z = this->z * invLength;

		this->w = this->w * invLength;
	}

	void Vector4::Normalize(Vector4& result) const noexcept
	{
		const float invLength = 1 / this->Length();

		result.x = this->x * invLength;

		result.y = this->y * invLength;

		result.z = this->z * invLength;

		result.w = this->w * invLength;
	}

	void Vector4::Clamp(const Vector4& vmin, const Vector4& vmax) noexcept
	{
		const Vector4 max = Vector4::Max(vmin, *this);

		*this = Vector4::Min(max, vmax);
	}

	void Vector4::Clamp(const Vector4& vmin, const Vector4& vmax, Vector4& result) const noexcept
	{
		const Vector4 max = Vector4::Max(vmin, *this);

		result = Vector4::Min(max, vmax);
	}

	float Vector4::Distance(const Vector4& v1, const Vector4& v2) noexcept
	{
		const Vector4 subVec = v1 - v2;

		return subVec.Length();
	}

	float Vector4::DistanceSquared(const Vector4& v1, const Vector4& v2) noexcept
	{
		const Vector4 subVec = v1 - v2;

		return subVec.LengthSquared();
	}

	void Vector4::Min(const Vector4& v1, const Vector4& v2, Vector4& result) noexcept
	{
		result.x = v1.x < v2.x ? v1.x : v2.x;

		result.y = v1.y < v2.y ? v1.y : v2.y;

		result.z = v1.z < v2.z ? v1.z : v2.z;

		result.w = v1.w < v2.w ? v1.w : v2.w;
	}

	Vector4 Vector4::Min(const Vector4& v1, const Vector4& v2) noexcept
	{
		Vector4 retVec;

		Vector4::Min(v1, v2, retVec);

		return retVec;
	}

	void Vector4::Max(const Vector4& v1, const Vector4& v2, Vector4& result) noexcept
	{
		result.x = v1.x > v2.x ? v1.x : v2.x;

		result.y = v1.y > v2.y ? v1.y : v2.y;

		result.z = v1.z > v2.z ? v1.z : v2.z;

		result.w = v1.w > v2.w ? v1.w : v2.w;
	}

	Vector4 Vector4::Max(const Vector4& v1, const Vector4& v2) noexcept
	{
		Vector4 retVec;

		Vector4::Max(v1, v2, retVec);

		return retVec;
	}

	void Vector4::Lerp(const Vector4& v1, const Vector4& v2, float t, Vector4& result) noexcept
	{
		result = v1 + t * (v2 - v1);
	}

	Vector4 Vector4::Lerp(const Vector4& v1, const Vector4& v2, float t) noexcept
	{
		Vector4 retVec;

		Vector4::Lerp(v1, v2, t, retVec);

		return retVec;
	}

	void Vector4::SmoothStep(const Vector4& v1, const Vector4& v2, float t, Vector4& result) noexcept
	{
	}

	Vector4 Vector4::SmoothStep(const Vector4& v1, const Vector4& v2, float t) noexcept
	{
	}

	void Vector4::Barycentric(const Vector4& v1, const Vector4& v2, const Vector4& v3, float f, float g,
		Vector4& result) noexcept
	{
	}

	Vector4 Vector4::Barycentric(const Vector4& v1, const Vector4& v2, const Vector4& v3, float f, float g) noexcept
	{
	}

	void Vector4::CatmullRom(const Vector4& v1, const Vector4& v2, const Vector4& v3, const Vector4& v4, float t,
		Vector4& result) noexcept
	{
	}

	Vector4 Vector4::CatmullRom(const Vector4& v1, const Vector4& v2, const Vector4& v3, const Vector4& v4,
		float t) noexcept
	{
	}

	void Vector4::Hermite(const Vector4& v1, const Vector4& t1, const Vector4& v2, const Vector4& t2, float t,
		Vector4& result) noexcept
	{
	}

	Vector4 Vector4::Hermite(const Vector4& v1, const Vector4& t1, const Vector4& v2, const Vector4& t2,
		float t) noexcept
	{
	}

	void Vector4::Reflect(const Vector4& ivec, const Vector4& nvec, Vector4& result) noexcept
	{
	}

	Vector4 Vector4::Reflect(const Vector4& ivec, const Vector4& nvec) noexcept
	{
	}

	void Vector4::Refract(const Vector4& ivec, const Vector4& nvec, float refractionIndex, Vector4& result) noexcept
	{
	}

	Vector4 Vector4::Refract(const Vector4& ivec, const Vector4& nvec, float refractionIndex) noexcept
	{
	}

	void Vector4::Transform(const Vector2& v, const Quaternion& quat, Vector4& result) noexcept
	{
	}

	Vector4 Vector4::Transform(const Vector2& v, const Quaternion& quat) noexcept
	{
	}

	void Vector4::Transform(const Vector3& v, const Quaternion& quat, Vector4& result) noexcept
	{
	}

	Vector4 Vector4::Transform(const Vector3& v, const Quaternion& quat) noexcept
	{
	}

	void Vector4::Transform(const Vector4& v, const Quaternion& quat, Vector4& result) noexcept
	{
	}

	Vector4 Vector4::Transform(const Vector4& v, const Quaternion& quat) noexcept
	{
	}

	void Vector4::Transform(const Vector4& v, const Matrix& m, Vector4& result) noexcept
	{
	}

	Vector4 Vector4::Transform(const Vector4& v, const Matrix& m) noexcept
	{
	}

	void Vector4::Transform(const Vector4* varray, size_t count, const Matrix& m, Vector4* resultArray) noexcept
	{
	}

	Vector4 Vector4::VectorSelect(const Vector4& v1, const Vector4& v2, const Vector4& control)
	{

	}
}