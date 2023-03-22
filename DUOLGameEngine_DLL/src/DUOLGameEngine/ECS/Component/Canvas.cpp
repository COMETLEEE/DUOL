#include "DUOLGameEngine/ECS/Component/Canvas.h"
#include "DUOLGameEngine/Manager/GraphicsManager.h"
#include "DUOLGameEngine/Manager/ResourceManager.h"
#include "DUOLGameEngine/Manager/UIManager.h"

namespace DUOLGameEngine
{
	Canvas::Canvas(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		RendererBase(owner, name)
	{
	}

	Canvas::~Canvas()
	{
		DUOLGameEngine::UIManager::GetInstance()->RemoveCanvas(this->GetGameObject());
	}

	void Canvas::Render()
	{
		GraphicsManager::GetInstance()->ReserveCanvas(_canvas);
	}

	void Canvas::SceneViewRender()
	{
	}

	void Canvas::OnResize(int width, int height)
	{
		ResourceManager::GetInstance()->DeleteCanvas(_canvasName);

		CreateCanvas(DUOLGraphicsLibrary::CanvasRenderMode::Texture, _canvasName, width, height);
	}

	void Canvas::CreateCanvas(DUOLGraphicsLibrary::CanvasRenderMode canvas, const DUOLCommon::tstring& canvasName, int width, int height)
	{
		_canvasName = canvasName;

		_canvas = ResourceManager::GetInstance()->CreateCanvas(canvas, canvasName, width, height);
	}
}
