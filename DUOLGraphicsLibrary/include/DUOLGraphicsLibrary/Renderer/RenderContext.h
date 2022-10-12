#pragma once
#include "DUOLGraphicsLibrary/EntityBase.h"
#include "DUOLGraphicsLibrary/RendererFlags.h"

namespace DUOLGraphicsLibrary
{

	class DUOLGRAPHICSLIBRARY_EXPORT RenderContext : public EntityBase
	{
		DUOLGRAPHICSLIBRARY_DECLARE_ENTITY(EntityID::RenderContext);

	private:

	protected:
		ScreenDesc _screenDesc;

		FrameRateDesc _frameRateDesc;

	protected:
		RenderContext() = default;

		RenderContext(const ScreenDesc& screenDesc, const FrameRateDesc& frameRateDesc);

	public:
		// �ػ󵵸� �����մϴ�.
		std::pair<int, int> GetResolution() const;

		//����ۿ� ����Ʈ���۸� �����Ѵ�.
		virtual void Present() abstract;

		virtual void SetScreenDesc(const ScreenDesc& scrrenDesc) = 0;

		inline const ScreenDesc& GetScreenDesc() const { return _screenDesc; }

		virtual void SetFrameRateDesc(const FrameRateDesc& framerateDesc) = 0;

		inline const FrameRateDesc& GetFrameRateDesc() const { return _frameRateDesc; }
	};
}