#pragma once
#include <sal.h>
#include <cassert>

namespace DUOLMath
{
	// 4바이트 부동소수점 16개
	struct Float4X4
	{
		union
		{
			struct
			{
				float _11, _12, _13, _14;
				float _21, _22, _23, _24;
				float _31, _32, _33, _34;
				float _41, _42, _43, _44;
			};

			float m[4][4];
		};

		Float4X4() = default;

		Float4X4(const Float4X4&) = default;
		Float4X4& operator=(const Float4X4&) = default;

		Float4X4(Float4X4&&) = default;
		Float4X4& operator=(Float4X4&&) = default;

		~Float4X4() = default;

		constexpr Float4X4(float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33)
			: _11(m00), _12(m01), _13(m02), _14(m03),
			_21(m10), _22(m11), _23(m12), _24(m13),
			_31(m20), _32(m21), _33(m22), _34(m23),
			_41(m30), _42(m31), _43(m32), _44(m33) {}

		explicit Float4X4(_In_reads_(16) const float* pArray)
		{
			assert(pArray != nullptr);

			m[0][0] = pArray[0];
			m[0][1] = pArray[1];
			m[0][2] = pArray[2];
			m[0][3] = pArray[3];

			m[1][0] = pArray[4];
			m[1][1] = pArray[5];
			m[1][2] = pArray[6];
			m[1][3] = pArray[7];

			m[2][0] = pArray[8];
			m[2][1] = pArray[9];
			m[2][2] = pArray[10];
			m[2][3] = pArray[11];

			m[3][0] = pArray[12];
			m[3][1] = pArray[13];
			m[3][2] = pArray[14];
			m[3][3] = pArray[15];
		}

		float  operator() (size_t Row, size_t Column) const { return m[Row][Column]; }
		float& operator() (size_t Row, size_t Column) { return m[Row][Column]; }
	};

	// 4x4 Matrix: 32 bit floating point components aligned on a 16 byte boundary
	__declspec(align(16)) struct Float4X4A : public Float4X4
	{
		Float4X4A() = default;

		Float4X4A(const Float4X4A&) = default;
		Float4X4A& operator=(const Float4X4A&) = default;

		Float4X4A(Float4X4A&&) = default;
		Float4X4A& operator=(Float4X4A&&) = default;

		~Float4X4A() = default;

		constexpr Float4X4A(float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33)
			: Float4X4(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33) {}
		explicit Float4X4A(_In_reads_(16) const float* pArray) : Float4X4(pArray) {}
	};
}