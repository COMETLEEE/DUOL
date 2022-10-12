#pragma once

#include <wrl.h>

namespace DUOLGraphicsLibrary
{
	// Type alias for Win32 ComPtr template
	template <typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
}
