#pragma once
#include <windows.h>
#include <string>
#include <tchar.h>

namespace DUOLCommon
{
	#if defined(UNICODE)
		using tstring = std::wstring;
	#else
		using tstring = std::string;
	#endif

	/**
	 * \brief ���ڿ� ���ۿ� ������ �ִ� �Լ��� �����ϴ� Ŭ�����Դϴ�.
	 */
	class StringHelper
	{
	public:
		inline static std::string WStringToString(const wchar_t* target)
		{
			const int stringLength = static_cast<int>(std::wcslen(target));

			// WCHAR �� �� ���� �� 2����Ʈ�� ����.
			const int bufferLength = stringLength * 2 + 1;

			char* buffer = new char[bufferLength];

			// WideCharToMultiByte �Լ��� ���ڿ��� ���� �ڵ����� null ���� ('\0')�� �־����� �ʽ��ϴ�.
			// ���� ���ڿ��� ��ȯ�� ��ģ �� �� �ڿ��� �������� null���ڸ� �־��־�� �մϴ�.
			int end = WideCharToMultiByte(CP_UTF8, 0, target, stringLength,
				buffer, bufferLength,
				nullptr, nullptr);

			buffer[end] = '\0';

			auto retVal = std::string{ buffer };

			delete[] buffer;

			return retVal;
		}

		inline static std::string WStringToString(const std::wstring& target)
		{
			return WStringToString(target.c_str());
		}

		inline static std::string ToString(const std::wstring& target)
		{
			return WStringToString(target);
		}

		inline static std::string ToString(const std::string& target)
		{
			return target;
		}

		inline static std::wstring StringToWString(const char* target)
		{
			const int stringLength = static_cast<int>(strlen(target));

			const int bufferLength = stringLength + 2;

			wchar_t* buffer = new wchar_t[bufferLength];

			int end = MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, target, stringLength,
				buffer, bufferLength);

			buffer[end] = '\0';

			auto retVal = std::wstring{ buffer };

			delete[] buffer;

			return retVal;
		}

		inline static std::wstring StringToWString(const std::string& target)
		{
			return StringToWString(target.c_str());
		}

		inline static std::wstring ToWString(const std::string& target)
		{
			return StringToWString(target);
		}

		inline static std::wstring ToWString(const std::wstring& target)
		{
			return target;
		}

		inline static tstring ToTString(const std::string& target)
		{
#if defined(UNICODE)
			return ToWString(target);
#else
			return target;
#endif
		}

		inline static tstring ToTString(const std::wstring& target)
		{
#if defined(UNICODE)
			return target;
#else
			return ToString(target);
#endif
		}
	};
}