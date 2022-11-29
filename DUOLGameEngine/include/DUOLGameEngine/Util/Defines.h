#pragma once

#define WIN32_LEAN_AND_MEAN

// ������ ������ �Ⱦ��ϴ� ..
#define NOMINMAX

#include <Windows.h>
#include <string>

#include "DUOLCommon/StringHelper.h"

/**
 * \brief �⺻ ����, �̵� ������ �� ���� �����ڸ� �����մϴ�.
 * \param TYPE Ŭ���� Ÿ��
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
  * \brief �⺻ ����, �̵� ������ �� ���� �����ڸ� �����մϴ�.
  * \param TYPE Ŭ���� Ÿ��
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
 * \brief �̱��� Ŭ���� ��� �� �̱����� ����� �߰��մϴ�.
 * \param TYPE Ŭ���� Ÿ��
 */
#define DECLARE_SINGLETON(TYPE)					\
		friend class SingletonBase<TYPE>;		\
												\
	private:									\
		TYPE();