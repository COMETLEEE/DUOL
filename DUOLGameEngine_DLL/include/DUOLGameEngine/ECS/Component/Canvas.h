#pragma once
#include "DUOLGameEngine/ECS/Component/RendererBase.h"
#include "DUOLGraphicsLibrary/FontEngine/IFontEngine.h"

namespace DUOLGraphicsLibrary
{
	enum class CanvasRenderMode;
}

namespace DUOLGameEngine
{
	class GameObject;
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
		Canvas(DUOLGameEngine::GameObject* owner=nullptr, const DUOLCommon::tstring& name = TEXT("Canvas"));

		~Canvas() override;

	public:
		void Initialize();

		virtual void Render() override;

		void CreateCanvas(DUOLGraphicsLibrary::CanvasRenderMode canvas, const DUOLCommon::tstring& canvasName = _T(""), int width = 0, int height = 0);

		void SceneViewRender();

		//void CreateCanvas(DUOLGraphicsLibrary::CanvasRenderMode canvas, const DUOLCommon::tstring& canvasName = _T(""), DUOLGraphicsLibrary::Texture* const texture = nullptr);

		void OnResize(int width, int height);

	private:
		DUOLGraphicsLibrary::CanvasRenderMode _renderMode;

		DUOLGraphicsLibrary::ICanvas* _canvas;

		DUOLMath::Vector2 _ratio;

		DUOLMath::Vector2 _preScreenSize;
	public:
		DUOLCommon::tstring _canvasName;

		DUOLCommon::tstring& GetCanvasName() { return _canvasName; };

		DUOLMath::Vector2& GetScreenRatio() { return _ratio; }

		void SetCanvasName(DUOLCommon::tstring& canvasname) { _canvasName = canvasname; }

		void SetRatio(int width, int height);
	public:
		DUOLGraphicsLibrary::ICanvas* GetCanvas();

#pragma region FRIEND_CLASS
		friend class GameObject;

		RTTR_ENABLE(DUOLGameEngine::RendererBase)

		RTTR_REGISTRATION_FRIEND
#pragma endregion
	};
}