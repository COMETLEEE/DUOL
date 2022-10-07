#pragma once
#include <intrin.h>

#include "Constants.h"
#include "Float4.h"
#include "Vector3.h"
#include "Vector4.h"

namespace DUOLMath
{
	struct Vector2;
	struct Vector3;
	struct Vector4;

	struct Quaternion : public Float4
	{
        Quaternion() noexcept : Float4(0, 0, 0, 1.f) {}
        constexpr Quaternion(float ix, float iy, float iz, float iw) noexcept : Float4(ix, iy, iz, iw) {}
        Quaternion(const Vector3& v, float scalar) noexcept : Float4(v.x, v.y, v.z, scalar) {}
        explicit Quaternion(const Vector4& v) noexcept : Float4(v.x, v.y, v.z, v.w) {}
        explicit Quaternion(_In_reads_(4) const float* pArray) noexcept : Float4(pArray) {}
        Quaternion(Float4 V) noexcept : Float4(V) {}
        Quaternion(const Float4& q) noexcept : Float4(q) {}

        Quaternion(const Quaternion&) = default;
        Quaternion& operator=(const Quaternion&) = default;

        Quaternion(Quaternion&&) = default;
        Quaternion& operator=(Quaternion&&) = default;

        operator Vector4() const noexcept { return Vector4{ this->x, this->y, this->z, this->w }; }

        ~Quaternion() = default;

        // Comparison operators
        bool operator == (const Quaternion& q) const noexcept;
        bool operator != (const Quaternion& q) const noexcept;

        // Assignment operators
        Quaternion& operator= (const Vector4& F) noexcept { x = F.x; y = F.y; z = F.z; w = F.w; return *this; }
        Quaternion& operator+= (const Quaternion& q) noexcept;
        Quaternion& operator-= (const Quaternion& q) noexcept;
        Quaternion& operator*= (const Quaternion& q) noexcept;
        Quaternion& operator*= (float S) noexcept;
        Quaternion& operator/= (const Quaternion& q) noexcept;

        // Unary operators
        Quaternion operator+ () const  noexcept { return *this; }
        Quaternion operator- () const noexcept;

        // Quaternion operations
        float Length() const noexcept;
        float LengthSquared() const noexcept;

        void Normalize() noexcept;
        void Normalize(Quaternion& result) const noexcept;

        void Conjugate() noexcept;
        void Conjugate(Quaternion& result) const noexcept;

        void Inverse(Quaternion& result) const noexcept;

        float Dot(const Quaternion& Q) const noexcept;

        // Static functions
        static Quaternion CreateFromAxisAngle(const Vector3& axis, float angle) noexcept;
        static Quaternion CreateFromYawPitchRoll(float yaw, float pitch, float roll) noexcept;
        static Quaternion CreateFromRotationMatrix(const Matrix& M) noexcept;

        static void Lerp(const Quaternion& q1, const Quaternion& q2, float t, Quaternion& result) noexcept;
        static Quaternion Lerp(const Quaternion& q1, const Quaternion& q2, float t) noexcept;

        static void Slerp(const Quaternion& q1, const Quaternion& q2, float t, Quaternion& result) noexcept;
        static Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, float t) noexcept;

        static void Concatenate(const Quaternion& q1, const Quaternion& q2, Quaternion& result) noexcept;
        static Quaternion Concatenate(const Quaternion& q1, const Quaternion& q2) noexcept;

        // Constants
        static const Quaternion Identity;
	};

    // Binary operators
    inline Quaternion operator+ (const Quaternion& Q1, const Quaternion& Q2) noexcept
    {
        return Quaternion{ Q1.x + Q2.x, Q1.y + Q2.y, Q1.z + Q2.z, Q1.w + Q2.w };
    }

    inline Quaternion operator- (const Quaternion& Q1, const Quaternion& Q2) noexcept
    {
        return Quaternion{ Q1.x - Q2.x, Q1.y - Q2.y, Q1.z - Q2.z, Q1.w - Q2.w };
    }

    inline Quaternion operator* (const Quaternion& Q1, const Quaternion& Q2) noexcept
    {
        // Returns the product Q2 * Q1 (which is the concatenation of a rotation Q1 followed by the rotation Q2)

		// [ (Q2.w * Q1.x) + (Q2.x * Q1.w) + (Q2.y * Q1.z) - (Q2.z * Q1.y),
		//   (Q2.w * Q1.y) - (Q2.x * Q1.z) + (Q2.y * Q1.w) + (Q2.z * Q1.x),
		//   (Q2.w * Q1.z) + (Q2.x * Q1.y) - (Q2.y * Q1.x) + (Q2.z * Q1.w),
		//   (Q2.w * Q1.w) - (Q2.x * Q1.x) - (Q2.y * Q1.y) - (Q2.z * Q1.z) ]
        return Quaternion{ (Q2.w * Q1.x) + (Q2.x * Q1.w) + (Q2.y * Q1.z) - (Q2.z * Q1.y),
            (Q2.w * Q1.y) - (Q2.x * Q1.z) + (Q2.y * Q1.w) + (Q2.z * Q1.x),
            (Q2.w * Q1.z) + (Q2.x * Q1.y) - (Q2.y * Q1.x) + (Q2.z * Q1.w),
            (Q2.w * Q1.w) - (Q2.x * Q1.x) - (Q2.y * Q1.y) - (Q2.z * Q1.z)
        };
    }

    inline Quaternion operator* (const Quaternion& Q, float S) noexcept
    {
        return Quaternion{ Q.x * S, Q.y * S, Q.z * S, Q.w * S };
    }

    inline Quaternion operator/ (const Quaternion& Q1, const Quaternion& Q2) noexcept
    {
        const float lengthSqr = Q1.Dot(Q2);

        Quaternion conjugate;

        Q2.Conjugate(conjugate);

        const Vector4U32 control = { 
            (lengthSqr <= DUOLMath::EPSILON) ? (0xFFFFFFFF) : 0,
            (lengthSqr <= DUOLMath::EPSILON) ? (0xFFFFFFFF) : 0,
            (lengthSqr <= DUOLMath::EPSILON) ? (0xFFFFFFFF) : 0,
            (lengthSqr <= DUOLMath::EPSILON) ? (0xFFFFFFFF) : 0
        };
    }

    inline Quaternion operator* (float S, const Quaternion& Q) noexcept
    {
        return Quaternion{Q.x * S, Q.y * S, Q.z * S, Q.w * S };
    }
}