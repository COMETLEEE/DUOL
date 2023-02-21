#pragma once

#include <wrl.h>

namespace DUOLGraphicsLibrary
{
	// Type alias for Win32 ComPtr template
	template <typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

    template <class T> void SafeRelease(T** ppT)
    {
        if (*ppT)
        {
            (*ppT)->Release();
            *ppT = NULL;
        }
    }
}
