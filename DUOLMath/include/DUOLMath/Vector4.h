#pragma once
#include <cstdint>

#include "DUOLMath/Vector3.h"
#include "DUOLMath/Float4.h"

namespace DUOLMath
{
	struct Vector4U32;
	struct Vector2;
	struct Vector3;
	struct Quaternion;
	struct Matrix;

	struct Vector4 : public Float4
	{
		Vector4() noexcept : Float4(0.f, 0.f, 0.f, 0.f) {}
		constexpr explicit Vector4(float ix) noexcept : Float4(ix, ix, ix, ix) {}
		constexpr Vector4(float ix, float iy, float iz, float iw) noexcept : Float4(ix, iy, iz, iw) {}
		explicit Vector4(_In_reads_(4) const float* pArray) noexcept : Float4(pArray) {}
		Vector4(const Float4& V) noexcept : Float4(V.x, V.y, V.z, V.w) {}

		Vector4(const Vector4&) = default;
		Vector4& operator=(const Vector4&) = default;

		Vector4(Vector4&&) = default;
		Vector4& operator=(Vector4&&) = default;

		~Vector4() = default;

		// Comparison operators
		bool operator == (const Vector4 & V) const noexcept;
		bool operator != (const Vector4 & V) const noexcept;

		// Assignment operators
		Vector4& operator+= (const Vector4 & V) noexcept;
		Vector4& operator-= (const Vector4 & V) noexcept;
		Vector4& operator*= (const Vector4 & V) noexcept;
		Vector4& operator*= (float S) noexcept;
		Vector4& operator/= (float S) noexcept;
		Vector4& operator=(const Vector3 & V) noexcept { x = V.x; y = V.y; z = V.z; w = 0.f; return *this; }

		// Unary operators
		Vector4 operator+ () const noexcept { return *this; }
		auto operator-() const noexcept -> Vector4;

		// Vector operations

		/* Positive 상한을 받아서 해당 원소들이 한도 범위 내에(-value <= element <= value)
		있는지 체크합니다. */
		bool InBounds(const Vector4 & Bounds) const noexcept;

		float Length() const noexcept;
		float LengthSquared() const noexcept;

		float Dot(const Vector4 & V) const noexcept;
		void Cross(const Vector4 & v1, const Vector4 & v2, Vector4 & result) const noexcept;
		Vector4 Cross(const Vector4 & v1, const Vector4 & v2) const noexcept;

		void Normalize() noexcept;
		void Normalize(Vector4 & result) const noexcept;

		void Clamp(const Vector4 & vmin, const Vector4 & vmax) noexcept;
		void Clamp(const Vector4 & vmin, const Vector4 & vmax, Vector4 & result) const noexcept;

		// Static functions
		static float Distance(const Vector4 & v1, const Vector4 & v2) noexcept;
		static float DistanceSquared(const Vector4 & v1, const Vector4 & v2) noexcept;

		static void Min(const Vector4 & v1, const Vector4 & v2, Vector4 & result) noexcept;
		static Vector4 Min(const Vector4 & v1, const Vector4 & v2) noexcept;

		static void Max(const Vector4 & v1, const Vector4 & v2, Vector4 & result) noexcept;
		static Vector4 Max(const Vector4 & v1, const Vector4 & v2) noexcept;

		static void Lerp(const Vector4 & v1, const Vector4 & v2, float t, Vector4 & result) noexcept;
		static Vector4 Lerp(const Vector4 & v1, const Vector4 & v2, float t) noexcept;

		static void SmoothStep(const Vector4 & v1, const Vector4 & v2, float t, Vector4 & result) noexcept;
		static Vector4 SmoothStep(const Vector4 & v1, const Vector4 & v2, float t) noexcept;

		static void Barycentric(const Vector4 & v1, const Vector4 & v2, const Vector4 & v3, float f, float g, Vector4 & result) noexcept;
		static Vector4 Barycentric(const Vector4 & v1, const Vector4 & v2, const Vector4 & v3, float f, float g) noexcept;

		static void CatmullRom(const Vector4 & v1, const Vector4 & v2, const Vector4 & v3, const Vector4 & v4, float t, Vector4 & result) noexcept;
		static Vector4 CatmullRom(const Vector4 & v1, const Vector4 & v2, const Vector4 & v3, const Vector4 & v4, float t) noexcept;

		static void Hermite(const Vector4 & v1, const Vector4 & t1, const Vector4 & v2, const Vector4 & t2, float t, Vector4 & result) noexcept;
		static Vector4 Hermite(const Vector4 & v1, const Vector4 & t1, const Vector4 & v2, const Vector4 & t2, float t) noexcept;

		static void Reflect(const Vector4 & ivec, const Vector4 & nvec, Vector4 & result) noexcept;
		static Vector4 Reflect(const Vector4 & ivec, const Vector4 & nvec) noexcept;

		static void Refract(const Vector4 & ivec, const Vector4 & nvec, float refractionIndex, Vector4 & result) noexcept;
		static Vector4 Refract(const Vector4 & ivec, const Vector4 & nvec, float refractionIndex) noexcept;

		static void Transform(const Vector2 & v, const Quaternion & quat, Vector4 & result) noexcept;
		static Vector4 Transform(const Vector2 & v, const Quaternion & quat) noexcept;

		static void Transform(const Vector3 & v, const Quaternion & quat, Vector4 & result) noexcept;
		static Vector4 Transform(const Vector3 & v, const Quaternion & quat) noexcept;

		static void Transform(const Vector4 & v, const Quaternion & quat, Vector4 & result) noexcept;
		static Vector4 Transform(const Vector4 & v, const Quaternion & quat) noexcept;

		static void Transform(const Vector4 & v, const Matrix & m, Vector4 & result) noexcept;
		static Vector4 Transform(const Vector4 & v, const Matrix & m) noexcept;
		static void Transform(_In_reads_(count) const Vector4 * varray, size_t count, const Matrix & m, _Out_writes_(count) Vector4 * resultArray) noexcept;

		static Vector4 VectorSelect(const Vector4& v1, const Vector4& v2, const Vector4& control);

		// Constants
		static const Vector4 Zero;
		static const Vector4 One;
		static const Vector4 UnitX;
		static const Vector4 UnitY;
		static const Vector4 UnitZ;
		static const Vector4 UnitW;
	};

	inline Vector4 operator+ (const Vector4& V1, const Vector4& V2) noexcept
	{
		return Vector4{ V1.x + V2.x, V1.y + V2.y, V1.z + V2.z, V1.w + V2.w };
	}

	inline Vector4 operator- (const Vector4& V1, const Vector4& V2) noexcept
	{
		return Vector4{ V1.x - V2.x, V1.y - V2.y, V1.z - V2.z, V1.w - V2.w };
	}

	inline Vector4 operator* (const Vector4& V1, const Vector4& V2) noexcept
	{
		return Vector4{ V1.x * V2.x, V1.y * V2.y, V1.z * V2.z, V1.w * V2.w };
	}

	inline Vector4 operator* (const Vector4& V, float S) noexcept
	{
		return Vector4{ V.x * S, V.y * S, V.z * S, V.w * S };
	}

	inline Vector4 operator/ (const Vector4& V1, const Vector4& V2) noexcept
	{
		return Vector4{ V1.x / V2.x, V1.y / V2.y, V1.z / V2.z, V1.w / V2.w };
	}

	inline Vector4 operator/ (const Vector4& V, float S) noexcept
	{
		assert(S != 0.0f);

		const float invS = 1 / S;

		return Vector4{ V.x * S, V.y * S, V.z * S, V.w * S };
	}

	inline Vector4 operator* (float S, const Vector4& V) noexcept
	{
		return Vector4{ V.x * S, V.y * S, V.z * S, V.w * S };
	}

	// unsigned int 32를 같이 담는다.
	__declspec(align(16)) struct Vector4U32
	{
		union
		{
			uint32_t u[4];

			Vector4 v;
		};

		inline operator Vector4() const { return v; }
	};
}