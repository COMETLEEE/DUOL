#pragma once
#include <stdexcept>
#include <windows.h>
#include <string>

namespace DUOLGraphicsLibrary
{

#define CASE_STRINGIZING(VALUE) \
	case VALUE: return #VALUE

	static const char* DXErrorToStr(const HRESULT hr)
	{
		switch (hr)
		{
            CASE_STRINGIZING(S_OK);
            CASE_STRINGIZING(S_FALSE);
            CASE_STRINGIZING(E_ABORT);
            CASE_STRINGIZING(E_ACCESSDENIED);
            CASE_STRINGIZING(E_FAIL);
            CASE_STRINGIZING(E_HANDLE);
            CASE_STRINGIZING(E_INVALIDARG);
            CASE_STRINGIZING(E_NOINTERFACE);
            CASE_STRINGIZING(E_NOTIMPL);
            CASE_STRINGIZING(E_OUTOFMEMORY);
            CASE_STRINGIZING(E_POINTER);
            CASE_STRINGIZING(E_UNEXPECTED);
            CASE_STRINGIZING(DXGI_ERROR_DEVICE_HUNG);
            CASE_STRINGIZING(DXGI_ERROR_DEVICE_REMOVED);
            CASE_STRINGIZING(DXGI_ERROR_DEVICE_RESET);
            CASE_STRINGIZING(DXGI_ERROR_DRIVER_INTERNAL_ERROR);
            CASE_STRINGIZING(DXGI_ERROR_FRAME_STATISTICS_DISJOINT);
            CASE_STRINGIZING(DXGI_ERROR_GRAPHICS_VIDPN_SOURCE_IN_USE);
            CASE_STRINGIZING(DXGI_ERROR_INVALID_CALL);
            CASE_STRINGIZING(DXGI_ERROR_MORE_DATA);
            CASE_STRINGIZING(DXGI_ERROR_NONEXCLUSIVE);
            CASE_STRINGIZING(DXGI_ERROR_NOT_CURRENTLY_AVAILABLE);
            CASE_STRINGIZING(DXGI_ERROR_NOT_FOUND);
            CASE_STRINGIZING(DXGI_ERROR_REMOTE_CLIENT_DISCONNECTED);
            CASE_STRINGIZING(DXGI_ERROR_REMOTE_OUTOFMEMORY);
            CASE_STRINGIZING(DXGI_ERROR_WAS_STILL_DRAWING);
            CASE_STRINGIZING(DXGI_ERROR_UNSUPPORTED);
            CASE_STRINGIZING(DXGI_ERROR_ACCESS_LOST);
            CASE_STRINGIZING(DXGI_ERROR_WAIT_TIMEOUT);
            CASE_STRINGIZING(DXGI_ERROR_SESSION_DISCONNECTED);
            CASE_STRINGIZING(DXGI_ERROR_RESTRICT_TO_OUTPUT_STALE);
            CASE_STRINGIZING(DXGI_ERROR_CANNOT_PROTECT_CONTENT);
            CASE_STRINGIZING(DXGI_ERROR_ACCESS_DENIED);
            CASE_STRINGIZING(DXGI_ERROR_NAME_ALREADY_EXISTS);
            CASE_STRINGIZING(DXGI_ERROR_SDK_COMPONENT_MISSING);
            CASE_STRINGIZING(D3D10_ERROR_FILE_NOT_FOUND);
            CASE_STRINGIZING(D3D10_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS);
            CASE_STRINGIZING(D3D11_ERROR_FILE_NOT_FOUND);
            CASE_STRINGIZING(D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS);
            CASE_STRINGIZING(D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS);
            CASE_STRINGIZING(D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD);
		}

        return nullptr;
	}

	inline void DXThrowError(const HRESULT& hr, const char* errorMessage)
	{
		if(FAILED(hr))
		{

			std::string strErrorMessage = errorMessage;

			strErrorMessage += "\n";
            strErrorMessage += "ErrorCode : ";

            if(auto hrMessage = DXErrorToStr(hr))
            {
                strErrorMessage += hrMessage;
            }
            else
            {
                strErrorMessage += hr;
            }

            throw std::runtime_error(strErrorMessage);
		}
	}

#ifndef DUOLGRAPHICS_ASSERT
#if defined( DEBUG ) || defined( _DEBUG )
#define DUOLGRAPHICS_ASSERT(b) if (!(b)) {OutputDebugStringA("Assert: " #b "\n");}
#else
#define DUOLGRAPHICS_ASSERT(b)
#endif //DEBUG || _DEBUG
#endif

}
