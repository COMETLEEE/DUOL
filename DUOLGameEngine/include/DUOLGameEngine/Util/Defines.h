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