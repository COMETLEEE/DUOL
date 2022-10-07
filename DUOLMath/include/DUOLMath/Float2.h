#pragma once
#include <cassert>
#include <sal.h>

namespace DUOLMath
{
	// 2D Vector : 32비트 (4바이트) 플로팅 포인트 2개
	struct Float2
	{
		float x;
		float y;

		Float2() = default;

		Float2(const Float2&) = default;
		Float2& operator=(const Float2&) = default;

		Float2(Float2&&) = default;
		Float2& operator=(Float2&&) = default;

		~Float2() = default;

		constexpr Float2(float x, float y) : x(x), y(y) {}
		explicit Float2(_In_reads_(2) const float* pArray) : x(pArray[0]), y(pArray[1])
		{
			assert(pArray != nullptr);

			x = pArray[0];
			y = pArray[1];
		}
	};

	// 2D Vector : 32비트 (4바이트) 플로팅 포인트 2개, 16바이트 정렬 버전
	_declspec(align(16)) struct Float2A : public Float2
	{
		Float2A() = default;

		Float2A(const Float2A&) = default;
		Float2A& operator=(const Float2A&) = default;

		Float2A(Float2A&&) = default;
		Float2A& operator=(Float2A&&) = default;

		~Float2A() = default;

		constexpr Float2A(float x, float y) : Float2(x, y) {}
		explicit Float2A(_In_reads_(2) const float* pArray) : Float2(pArray) {}
	};
}