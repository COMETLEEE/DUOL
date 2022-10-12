#pragma once
#include <cassert>
#include <utility>
#include <cmath>

#include <xmmintrin.h>

#define _DM_SSE_INTRINSICS_

#if defined(_DM_SSE_INTRINSICS_)
	using DMVECTOR = __m128;
#endif

namespace DUOLMath
{
	// 소수점의 대, 소 비교에 사용됩니다.
	constexpr float EPSILON = 0.0001f;


}