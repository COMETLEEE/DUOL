#pragma once
// 각종 typedef
using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;
using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;

// 문자열을...
#include <basetsd.h>
#include <tchar.h>
#include <xstring>
#include <stringapiset.h>

using tchar = TCHAR;

#if defined(UNICODE)
using tstring = std::wstring;
#else
using tstring = std::string;
#endif