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
	// �Ҽ����� ��, �� �񱳿� ���˴ϴ�.
	constexpr float EPSILON = 0.0001f;


}