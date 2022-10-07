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

		// �ػ󵵸� �����մϴ�.
		std::pair<int, int> GetResolution() const;

		//����ۿ� ����Ʈ���۸� �����Ѵ�.
		virtual void Present() abstract;

		virtual void SetScreenDesc(const ScreenDesc& scrrenDesc) = 0;

		inline const ScreenDesc& GetScreenDesc() const { return _screenDesc; }

		virtual void SetFrameRateDesc(const FrameRateDesc& framerateDesc) = 0;

		inline const FrameRateDesc& GetFrameRateDesc() const { return _frameRateDesc; }

		//virtual void Set

	};
}