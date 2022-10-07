#pragma once
#include <sal.h>
#include <cassert>

namespace DUOLMath
{
	// 4D Vector : 32비트 (4바이트) 플로팅 포인트 4개
	struct Float4
	{
		float x;
		float y;
		float z;
		float w;

		Float4() = default;

		Float4(const Float4&) = default;
		Float4& operator=(const Float4&) = default;

		Float4(Float4&&) = default;
		Float4& operator=(Float4&&) = default;

		~Float4() = default;

		constexpr Float4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
		explicit Float4(_In_reads_(4) const float* pArray) : x(pArray[0]), y(pArray[1]), z(pArray[2]), w(pArray[3])
		{
			assert(pArray != nullptr);

			x = pArray[0];
			y = pArray[1];
			z = pArray[2];
			w = pArray[3];
		}
	};

	// 4D Vector : 32비트 (4바이트) 플로팅 포인트 4개, 16바이트 정렬 버전
	_declspec(align(16)) struct Float4A : public Float4
	{
		Float4A() = default;

		Float4A(const Float4A&) = default;
		Float4A& operator=(const Float4A&) = default;

		Float4A(Float4A&&) = default;
		Float4A& operator=(Float4A&&) = default;

		~Float4A() = default;

		constexpr Float4A(float x, float y, float z, float w) : Float4(x, y, z, w) {}
		explicit Float4A(_In_reads_(4) const float* pArray) : Float4(pArray) {}
	};
}