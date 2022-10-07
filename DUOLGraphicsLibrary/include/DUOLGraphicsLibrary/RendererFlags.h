#pragma once
#include <utility>

namespace DUOLGraphicsLibrary
{

	struct RendererDesc
	{

	};

	struct ScreenDesc
	{
		std::pair<int, int> _screenSize; //width, height

		bool _isFullscreen;
	};

	struct FrameRateDesc
	{
		int  _refreshRate = 60;

		int _interval = 1;
	};

	struct RenderContextDesc
	{

		ScreenDesc _screenDesc;

		FrameRateDesc _frameRate;
	};
}
