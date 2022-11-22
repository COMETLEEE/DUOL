#pragma once

#define JSON_ASSERT(b)  { MessageBox( nullptr, b, L"Error", MB_OK); }

namespace DUOLJson
{
	inline void DUOLJSON_ASSERT(const TCHAR* text, ...)
	{
		TCHAR buffer[64];

		int result;
		va_list args;

		va_start(args, text);
		result = _snwprintf_s(buffer, 64, text, args);
		va_end(args);

		throw MessageBox(nullptr, buffer, L"Error", MB_OK);
	}
}