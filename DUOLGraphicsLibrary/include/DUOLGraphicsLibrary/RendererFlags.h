#pragma once
#include "DUOLGraphicsLibrary/Export.h"
#include <utility>

namespace DUOLGraphicsLibrary
{
	DUOLGRAPHICSLIBRARY_TEMPLATE_EXPORT template struct DUOLGRAPHICSLIBRARY_EXPORT std::pair<int, int>;

	struct DUOLGRAPHICSLIBRARY_EXPORT RendererDesc
	{
		long _instance;

		long _handle;
	};

	struct DUOLGRAPHICSLIBRARY_EXPORT ScreenDesc
	{
		// width, height
		std::pair<int, int> _screenSize;

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

	struct VideoAdapterDesc
	{
		// hardware Name
		wchar_t _hardwareName[128];

		unsigned __int64 _videoMemorySize;
	};
}
