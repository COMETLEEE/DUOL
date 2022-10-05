#pragma once
#include <sal.h>

namespace MathLibrary
{
	struct Matrix
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

		Matrix() = default;

		constexpr Matrix(float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33)
			: _11(m00), _12(m01), _13(m02), _14(m03),
			_21(m10), _22(m11), _23(m12), _24(m13),
			_31(m20), _32(m21), _33(m22), _34(m23),
			_41(m30), _42(m31), _43(m32), _44(m33) {}

		explicit Matrix(_In_reads_(16) const float* pArray) 
			: _11(pArray[0]), _12(pArray[1]), _13(pArray[2]), _14(pArray[3]),
			_21(pArray[4]), _22(pArray[5]), _23(pArray[6]), _24(pArray[7]),
			_31(pArray[8]), _32(pArray[9]), _33(pArray[10]), _34(pArray[11]),
			_41(pArray[12]), _42(pArray[13]), _43(pArray[14]), _44(pArray[15]) {}

		Matrix(const Matrix&) = default;
		Matrix& operator=(const Matrix&) = default;

		Matrix(Matrix&&) = default;
		Matrix& operator=(Matrix&&) = default;

		float       operator() (size_t Row, size_t Column) const { return m[Row][Column]; }
		float&		operator() (size_t Row, size_t Column) { return m[Row][Column]; }
	};
}