#pragma once
#include "DUOLGraphicsLibrary/RendererFlags.h"

namespace DUOLGraphicsLibrary
{

	class RenderContext
	{
	private:



	protected:

		ScreenDesc _screenDesc;

		FrameRateDesc _frameRateDesc;

		RenderContext() = default;

	public:

		// 해상도를 제공합니다.
		std::pair<int, int> GetResolution() const;

		//백버퍼와 프론트버퍼를 스왑한다.
		virtual void Present() abstract;

		virtual void SetScreenDesc(const ScreenDesc& scrrenDesc) = 0;

		inline const ScreenDesc& GetScreenDesc() const { return _screenDesc; }

		virtual void SetFrameRateDesc(const FrameRateDesc& framerateDesc) = 0;

		inline const FrameRateDesc& GetFrameRateDesc() const { return _frameRateDesc; }

		//virtual void Set

	};
}