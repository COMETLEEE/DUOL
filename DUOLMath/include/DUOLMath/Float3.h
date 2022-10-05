#pragma once
#include <sal.h>

namespace MathLibrary
{
	// 3D Vector : 32��Ʈ (4����Ʈ) �÷��� ����Ʈ 3��
	struct Float3
	{
		float x;
		float y;
		float z;

		Float3() = default;

		Float3(const Float3&) = default;
		Float3& operator=(const Float3&) = default;

		Float3(Float3&&) = default;
		Float3& operator=(Float3&&) = default;

		constexpr Float3(float x, float y, float z) : x(x), y(y), z(z) {}
		explicit Float3(_In_reads_(3) const float* pArray) : x(pArray[0]), y(pArray[1]), z(pArray[2]) {}
	};

	// 3D Vector : 32��Ʈ (4����Ʈ) �÷��� ����Ʈ 3��, 16����Ʈ ���� ����
	_declspec(align(16)) struct Float3A : public Float3
	{
		Float3A() = default;

		Float3A(const Float3A&) = default;
		Float3A& operator=(const Float3A&) = default;

		Float3A(Float3A&&) = default;
		Float3A& operator=(Float3A&&) = default;

		constexpr Float3A(float x, float y, float z) : Float3(x, y, z) {}
		explicit Float3A(_In_reads_(3) const float* pArray) : Float3(pArray) {}
	};
}