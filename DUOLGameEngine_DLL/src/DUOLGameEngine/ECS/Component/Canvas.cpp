#include "DUOLGameEngine/ECS/Component/Canvas.h"
#include "DUOLGameEngine/Manager/GraphicsManager.h"
#include "DUOLGameEngine/Manager/ResourceManager.h"
#include "DUOLGameEngine/Manager/UIManager.h"
#include "DUOLCommon/MetaDataType.h"
#include "DUOLGameEngine/ECS/GameObject.h"


#include <rttr/registration>

#include "DUOLGameEngine/Manager/SceneManagement/Scene.h"

using namespace rttr;

RTTR_PLUGIN_REGISTRATION
{
	rttr::registration::class_<DUOLGameEngine::Canvas>("Canvas")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	)
/*	.property("Cavnas Name",&DUOLGameEngine::Canvas::GetCanvasName,&DUOLGameEngine::Canvas::SetCanvasName)
	(
		metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::String)
	)*/;
}

namespace DUOLGameEngine
{
	Canvas::Canvas(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		RendererBase(owner, name)
		, _renderMode()
		, _canvas(nullptr)
		, _ratio(1.f,1.f)
		, _canvasName(L"Canvas")
	{
		Initialize();
	}

	Canvas::~Canvas()
	{
		DUOLGameEngine::UIManager::GetInstance()->RemoveCanvas(this->GetGameObject());
	}

	void Canvas::Initialize()
	{
		auto screensize = DUOLGameEngine::GraphicsManager::GetInstance()->GetScreenSize();
		_preScreenSize = UIManager::GetInstance()->GetPreScreenSize();

		// Scene을 불러올때 null인 경우가 있다. 이런경우 넣어준다.
		// Debug용은 CreateCanvas로 만들어줘야한다. 
		if (_canvas == nullptr)
		{
			std::string canvasName = "Canvas" + std::to_string(Scene::_canvasCount);
			CreateCanvas(DUOLGraphicsLibrary::CanvasRenderMode::Texture, DUOLCommon::StringHelper::ToTString(canvasName), screensize.x, screensize.y);
			_canvasName = DUOLCommon::StringHelper::ToTString(canvasName);
			Scene::_canvasCount++;
		}

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
		_ratio.x = width / _preScreenSize.x;
		_ratio.y = height / _preScreenSize.y; 

		//ResourceManager::GetInstance()->DeleteCanvas(_canvasName);

		//CreateCanvas(DUOLGraphicsLibrary::CanvasRenderMode::Texture, _canvasName, width, height);

		GetGameObject()->GetComponent<RectTransform>()->SetRectZ(width);
		GetGameObject()->GetComponent<RectTransform>()->SetRectW(height);

		_preScreenSize = DUOLMath::Vector2(width, height);
		DUOLGameEngine::UIManager::GetInstance()->SetPreScreenSize(_preScreenSize);
	}

	void Canvas::SetRatio(int width, int height)
	{
		_ratio.x = width / _preScreenSize.x;
		_ratio.y = height / _preScreenSize.y;
	}

	DUOLGraphicsLibrary::ICanvas* Canvas::GetCanvas()
	{
		if (!_canvas)
		{
			auto screensize = DUOLGameEngine::GraphicsManager::GetInstance()->GetScreenSize();

			_canvas = ResourceManager::GetInstance()->CreateCanvas(DUOLGraphicsLibrary::CanvasRenderMode::Texture, L"Canvas", screensize.x, screensize.y);
		}

		return _canvas;
	}

	void Canvas::CreateCanvas(DUOLGraphicsLibrary::CanvasRenderMode canvas, const DUOLCommon::tstring& canvasName, int width, int height)
	{
		_canvasName = canvasName;

		_canvas = ResourceManager::GetInstance()->CreateCanvas(canvas, canvasName, width, height);
	}
}
