#pragma once
#include <string>

namespace DUOLGameEngine
{
	#if defined(UNICODE)
		using tstring = std::wstring;
	#else
		using tstring = std::string;
	#endif
}

/**
 * \brief 기본 복사, 이동 생성자 및 대입 연산자를 정의합니다.
 * \param TYPE 클래스 타입
 */
#define DEFINE_DEFAULT_COPY_MOVE(TYPE)					\
	public:												\
		TYPE(const TYPE& other) = default;				\
														\
		TYPE& operator=(const TYPE& other) = default;	\
														\
		TYPE(TYPE&& other) = default;					\
														\
		TYPE& operator=(TYPE&& other) = default;