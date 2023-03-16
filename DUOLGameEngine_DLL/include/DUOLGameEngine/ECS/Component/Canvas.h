#pragma once
#include "DUOLGameEngine/ECS/Component/RendererBase.h"
#include "DUOLGraphicsLibrary/FontEngine/IFontEngine.h"

namespace DUOLGraphicsLibrary
{
	enum class CanvasRenderMode;
}

namespace DUOLGameEngine
{
 /**

     @class   Canvas
     @brief   UI들을 그리기 위한 Canvas 컴포넌트
     @details ~
     @author  KyungMin Oh

 **/
	class DUOL_GAMEENGINE_API Canvas final : public DUOLGameEngine::RendererBase
	{
	public:
		Canvas(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name = TEXT("Canvas"));

		~Canvas() override;

	public:
		virtual void Render() override;

		void CreateCanvas(DUOLGraphicsLibrary::CanvasRenderMode canvas, const DUOLCommon::tstring& canvasName = _T(""), int width = 0, int height = 0);

	private:
		DUOLGraphicsLibrary::CanvasRenderMode _renderMode;

		DUOLGraphicsLibrary::ICanvas* _canvas;

	public:
		DUOLGraphicsLibrary::ICanvas* GetCanvas() const
		{
			return _canvas;
		}
	};
}