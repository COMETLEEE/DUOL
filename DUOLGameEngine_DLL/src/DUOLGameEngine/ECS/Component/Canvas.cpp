#include "DUOLGameEngine/ECS/Component/Canvas.h"
#include "DUOLGameEngine/Manager/GraphicsManager.h"
#include "DUOLGameEngine/Manager/ResourceManager.h"

namespace DUOLGameEngine
{
	Canvas::Canvas(const std::weak_ptr<DUOLGameEngine::GameObject>& owner, const DUOLCommon::tstring& name):
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

	void Canvas::CreateCanvas(DUOLGraphicsLibrary::CanvasRenderMode canvas)
	{
		_canvas = ResourceManager::GetInstance()->CreateCanvas(canvas);
	}
}
