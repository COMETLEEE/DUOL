#include "DUOLMath\Quaternion.h"
#include "DUOLMath\Constants.h"

namespace DUOLMath
{
	const Quaternion Quaternion::Identity = { 0.f, 0.f, 0.f, 1.f };

	bool Quaternion::operator==(const Quaternion& q) const noexcept
	{
		return ((std::abs(this->x - q.x) <= DUOLMath::EPSILON) && (std::abs(this->y - q.y) <= DUOLMath::EPSILON) &&
			(std::abs(this->z - q.z) <= DUOLMath::EPSILON) && (std::abs(this->w - q.w) <= DUOLMath::EPSILON));
	}

	bool Quaternion::operator!=(const Quaternion& q) const noexcept
	{
		return !(*this == q);
	}

	Quaternion& Quaternion::operator+=(const Quaternion& q) noexcept
	{
		*this = *this + q;

		return *this;
	}

	Quaternion& Quaternion::operator-=(const Quaternion& q) noexcept
	{
		*this = *this - q;

		return *this;
	}

	Quaternion& Quaternion::operator*=(const Quaternion& q) noexcept
	{
		*this = *this * q;

		return *this;
	}

	Quaternion& Quaternion::operator*=(float S) noexcept
	{
		*this = *this * S;

		return *this;
	}

	Quaternion& Quaternion::operator/=(const Quaternion& q) noexcept
	{
		*this = *this / q;

		return *this;
	}

	Quaternion Quaternion::operator-() const noexcept
	{
		const Quaternion retQua{this->x, this->y, this->z, this->w };

		return retQua;
	}

	float Quaternion::Length() const noexcept
	{
		const Vector4 vec = Vector4(*this);

		return vec.Length();
	}

	float Quaternion::LengthSquared() const noexcept
	{
		const float length = this->Length();

		return length * length;
	}

	void Quaternion::Normalize() noexcept
	{
		float length = this->Length();
		
		if (length > 0.f)
			length = 1 / length;

		this->x = this->x * length;
		this->y = this->y * length;
		this->z = this->z * length;
		this->w = this->w * length;
	}

	void Quaternion::Normalize(Quaternion& result) const noexcept
	{
		float length = this->Length();

		if (length > 0.f)
			length = 1 / length;

		result.x = this->x * length;
		result.y = this->y * length;
		result.z = this->z * length;
		result.w = this->w * length;
	}

	void Quaternion::Conjugate() noexcept
	{
		this->x = -this->x;
		this->y = -this->y;
		this->z = -this->z;
		this->w = this->w;
	}

	void Quaternion::Conjugate(Quaternion& result) const noexcept
	{
		result.x = -this->x;
		result.y = -this->y;
		result.z = -this->z;
		result.w = this->w;
	}

	void Quaternion::Inverse(Quaternion& result) const noexcept
	{
		// Vector4 연산을 통한 계산이 필요합니다.
	}

	float Quaternion::Dot(const Quaternion& Q) const noexcept
	{
		return this->x * Q.x + this->y * Q.y + this->z * Q.z + this->w * Q.w;
	}

	Quaternion Quaternion::CreateFromAxisAngle(const Vector3& axis, float angle) noexcept
	{
		// 중요한 함수
	}

	Quaternion Quaternion::CreateFromYawPitchRoll(float yaw, float pitch, float roll) noexcept
	{
		// 중요한 함수
	}

	Quaternion Quaternion::CreateFromRotationMatrix(const Matrix& M) noexcept
	{
		// 중요한 함수
	}

	void Quaternion::Lerp(const Quaternion& q1, const Quaternion& q2, float t, Quaternion& result) noexcept
	{
		const Vector4 dot = Vector4(q1.Dot(q2));

		Quaternion ret;

		if (Vector4GreaterOrEqual(dot, Vector4::Zero))
		{
			ret = VectorLerp(q1, q2, t);
		}
		else
		{
			const Vector4 tv = VectorReplicate(t);
			const Vector4 t1v = VectorReplicate(1.f - t);
			const Vector4 X0 = VectorMultiply(q1, t1v);
			const Vector4 X1 = VectorMultiply(q2, tv);

			ret = X0 - X1;
		}

		ret.Normalize();

		result = ret;
	}

	Quaternion Quaternion::Lerp(const Quaternion& q1, const Quaternion& q2, float t) noexcept
	{
		const Vector4 dot = Vector4(q1.Dot(q2));

		Quaternion ret;

		if (Vector4GreaterOrEqual(dot, Vector4::Zero))
		{
			ret = VectorLerp(q1, q2, t);
		}
		else
		{
			const Vector4 tv = VectorReplicate(t);
			const Vector4 t1v = VectorReplicate(1.f - t);
			const Vector4 X0 = VectorMultiply(q1, t1v);
			const Vector4 X1 = VectorMultiply(q2, tv);

			ret = X0 - X1;
		}

		ret.Normalize();

		return ret;
	}

	void Quaternion::Slerp(const Quaternion& q1, const Quaternion& q2, float t, Quaternion& result) noexcept
	{

	}

	Quaternion Quaternion::Slerp(const Quaternion& q1, const Quaternion& q2, float t) noexcept
	{

	}

	void Quaternion::Concatenate(const Quaternion& q1, const Quaternion& q2, Quaternion& result) noexcept
	{

	}

	Quaternion Quaternion::Concatenate(const Quaternion& q1, const Quaternion& q2) noexcept
	{

	}
}
