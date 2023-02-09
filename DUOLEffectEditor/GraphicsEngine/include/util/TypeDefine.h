#pragma once
#pragma comment(lib,"D3D11.lib")

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

#define HR(x) {HRESULT hr;\
	hr = x;\
if(FAILED(hr)) \
assert(false);}

#define HRTex(x) {HRESULT hr;\
	hr = x;\
if(FAILED(hr)) \
assert(false);}

#define ReleaseCOM(x) { if(x){ x->Release(); x = 0; } }

#if defined(DEBUG) || defined(_DEBUG)
#if defined (_WIN32) && !defined(_WIN64)
#pragma comment(lib, "..\\Library\\lib\\DirectXTex_x86_Debug.lib")
#pragma comment(lib, "..\\Library\\lib\\DirectXTK_x86_Debug.lib")
#else
#pragma comment(lib, "..\\Library\\lib\\DirectXTex_x64_Debug.lib")
#pragma comment(lib, "..\\Library\\lib\\DirectXTK_x64_Debug.lib")
#endif
#else
#if defined (_WIN32) && !defined(_WIN64)
#pragma comment(lib, "..\\Library\\lib\\DirectXTex_x86_Release")
#pragma comment(lib, "..\\Library\\lib\\DirectXTK_x86_Release")
#else
#pragma comment(lib, "..\\Library\\lib\\DirectXTex_x64_Release")
#pragma comment(lib, "..\\Library\\lib\\DirectXTK_x64_Release")
#endif
#endif

#pragma comment(lib,"dxguid.lib")

// 각종 typedef
using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;
using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;

using TypeUID = unsigned long long;
// 문자열을...
#include <basetsd.h>
#include <tchar.h>
#include <xstring>
#include <stringapiset.h>
#include <d3d11.h>
#include <fstream>

using tchar = TCHAR;

#if defined(UNICODE)
using tstring = std::wstring;
#else
using tstring = std::string;
#endif

