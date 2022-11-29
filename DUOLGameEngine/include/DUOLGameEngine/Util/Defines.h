#pragma once

#define WIN32_LEAN_AND_MEAN

// 쓸액히 디파인 안씁니다 ..
#define NOMINMAX

#include <Windows.h>
#include <string>

#include "DUOLCommon/StringHelper.h"

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

 /**
  * \brief 기본 복사, 이동 생성자 및 대입 연산자를 삭제합니다.
  * \param TYPE 클래스 타입
  */
#define DELETE_COPY_MOVE(TYPE)							\
	public:												\
		TYPE(const TYPE& other) = delete;				\
														\
		TYPE& operator=(const TYPE& other) = delete;	\
														\
		TYPE(TYPE&& other) = delete;					\
														\
		TYPE& operator=(TYPE&& other) = delete;

/**
 * \brief 싱글톤 클래스 상속 후 싱글톤의 기능을 추가합니다.
 * \param TYPE 클래스 타입
 */
#define DECLARE_SINGLETON(TYPE)					\
		friend class SingletonBase<TYPE>;		\
												\
	private:									\
		TYPE();