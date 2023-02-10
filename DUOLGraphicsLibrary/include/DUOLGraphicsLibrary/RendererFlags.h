#pragma once
#include "DUOLGraphicsLibrary/Export.h"
#include <DUOLMath/DUOLMath.h>
#include <utility>

namespace DUOLGraphicsLibrary
{
	struct DUOLGRAPHICSLIBRARY_EXPORT DUOLMath::Vector2;

	enum class ModuleType
	{
		DIRECTX11 = 1
		//, DIRECTX12
	};

	struct ModuleInfo
	{
		ModuleType _moduleType;

		void* _device;

		void* _deviceContext;
	};
	
	struct DUOLGRAPHICSLIBRARY_EXPORT RendererDesc
	{
		long _handle;

		ModuleType _moduleType;
	};


	struct DUOLGRAPHICSLIBRARY_EXPORT ScreenDesc
	{
		ScreenDesc() :
			_screenSize(0.f, 0.f)
			, _isFullscreen(false)
			, _isMSAA(false)
			, _sampleCount(1)
		{
		}

		// width, height
		DUOLMath::Vector2 _screenSize;

		bool _isFullscreen;

		bool _isMSAA;

		int _sampleCount;
	};

	struct DUOLGRAPHICSLIBRARY_EXPORT FrameRateDesc
	{
		FrameRateDesc() :
			_refreshRate(60)
			, _interval(1)
		{

		}

		~FrameRateDesc() = default;

		FrameRateDesc(const FrameRateDesc& frameRateDesc) = default;

		int  _refreshRate;

		int _interval;
	};

	struct DUOLGRAPHICSLIBRARY_EXPORT RenderContextDesc
	{
		ScreenDesc _screenDesc;

		FrameRateDesc _frameRate;
	};

	struct DUOLGRAPHICSLIBRARY_EXPORT QueryInfo
	{
		UINT64 _IAVertices;

		UINT64 _IAPrimitives;

		UINT64 _VSInvocations;

		UINT64 _GSInvocations;

		UINT64 _GSPrimitives;

		UINT64 _CInvocations;

		UINT64 _CPrimitives;

		UINT64 _PSInvocations;

		UINT64 _HSInvocations;

		UINT64 _DSInvocations;

		UINT64 _CSInvocations;
	};

	struct VideoAdapterDesc
	{
		// hardware Name
		wchar_t _hardwareName[128];

		unsigned __int64 _videoMemorySize;
	};
}
