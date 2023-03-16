#include "DUOLGameEngine/ECS/Component/Canvas.h"
#include "DUOLGameEngine/Manager/GraphicsManager.h"
#include "DUOLGameEngine/Manager/ResourceManager.h"

namespace DUOLGameEngine
{
	Canvas::Canvas(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name):
		RendererBase(owner, name)
	{
	}

	Canvas::~Canvas()
	{
	}

	void Canvas::Render()
	{
		GraphicsManager::GetInstance()->ReserveCanvas(_canvas);
	}

	void Canvas::CreateCanvas(DUOLGraphicsLibrary::CanvasRenderMode canvas, const DUOLCommon::tstring& canvasName, int width, int height)
	{
		_canvas = ResourceManager::GetInstance()->CreateCanvas(canvas, canvasName, width, height);
	}
}
