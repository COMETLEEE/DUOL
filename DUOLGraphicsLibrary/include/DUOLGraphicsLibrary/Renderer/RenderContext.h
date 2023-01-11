#pragma once
#include "DUOLGraphicsLibrary/EntityBase.h"
#include "DUOLGraphicsLibrary/Renderer/RenderTarget.h"
#include "DUOLGraphicsLibrary/RendererFlags.h"

namespace DUOLGraphicsLibrary
{
	class RenderTarget;
	class Texture;
 /**

     @class   RenderContext
     @brief   렌더링 파이프라인에 바인딩, screensize와 framerate 조절
     @details ~
     @author  KyungMin Oh

 **/
	class DUOLGRAPHICSLIBRARY_EXPORT RenderContext : public EntityBase
	{
		DUOLGRAPHICSLIBRARY_DECLARE_ENTITY(EntityID::RenderContext);

	private:

	protected:
		ScreenDesc _screenDesc;

		FrameRateDesc _frameRateDesc;

	protected:
		RenderContext(const UINT64& guid, const ScreenDesc& screenDesc, const FrameRateDesc& frameRateDesc) :
			EntityBase(guid)
			, _screenDesc(screenDesc)
			, _frameRateDesc(frameRateDesc)
		{
		}

	public:
		// 해상도를 제공합니다.
		virtual DUOLMath::Vector2 GetResolution() { return _screenDesc._screenSize; };

		virtual RenderTarget* GetBackBufferRenderTarget() abstract;

		virtual Texture* GetBackBufferTexture() abstract;

		//백버퍼와 프론트버퍼를 스왑한다.
		virtual void Present() abstract;

		virtual void SetScreenDesc(const ScreenDesc& screenDesc) abstract;

		inline const ScreenDesc& GetScreenDesc() const { return _screenDesc; }

		virtual void SetFrameRateDesc(const FrameRateDesc& framerateDesc) abstract;

		inline const FrameRateDesc& GetFrameRateDesc() const { return _frameRateDesc; }
	};
}