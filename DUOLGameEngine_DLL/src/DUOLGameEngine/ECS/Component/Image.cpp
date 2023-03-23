#include "DUOLGameEngine/ECS/Component/Image.h"
#include <rttr/registration>

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/RectTransform.h"
#include "DUOLGameEngine/Manager/GraphicsManager.h"
#include "DUOLCommon/MetaDataType.h"
#include "DUOLGameEngine/ECS/Component/Camera.h"
#include "DUOLGameEngine/Manager/ResourceManager.h"
#include "DUOLGameEngine/Manager/UIManager.h"
#include "DUOLGraphicsLibrary/Renderer/Texture.h"
#include "DUOLGameEngine/ECS/Component/Canvas.h"
#include "DUOLGameEngine/ECS/Component/Camera.h"

using namespace rttr;

RTTR_PLUGIN_REGISTRATION
{
	rttr::registration::class_<DUOLGameEngine::Image>("Image")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.property("Source Image", &DUOLGameEngine::Image::_sprite)
	(
		metadata(DUOLCommon::MetaDataType::SerializeByString, true)
		// 이름 검색 
		, metadata(DUOLCommon::MetaDataType::MappingType, DUOLCommon::MappingType::Resource)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::UIFileName)
	)
	.property("Color", &DUOLGameEngine::Image::GetRGB,&DUOLGameEngine::Image::SetRGB)
	(
		metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float3)
	)
	.property("Raycast Target", &DUOLGameEngine::Image::GetRaycastTarget,&DUOLGameEngine::Image::SetRaycastTarget)
	(
		metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Bool)
	);
}

namespace DUOLGameEngine
{
	Image::Image() :
		BehaviourBase()
		, _canvas(nullptr)
		, _orderInLayer(0)
		, _sprite(new DUOLGraphicsLibrary::Sprite())
		, _rectTransform(nullptr)
		, _rgb(DUOLMath::Vector3(255.f, 255.f, 255.f))
		, _raycastTarget(true)
	{
	}

	Image::Image(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		BehaviourBase(owner, name)
		, _canvas(nullptr)
		, _orderInLayer(0)
		, _sprite(new DUOLGraphicsLibrary::Sprite())
		, _rectTransform(nullptr)
		, _rgb(DUOLMath::Vector3(255.f, 255.f, 255.f))
		, _raycastTarget(true)
	{
		Initialize();
	}

	Image::~Image()
	{
		// 이벤트가 돌아가면 종료도 해줘야 한다.
		DUOLGameEngine::EventManager::GetInstance()->RemoveEventFunction<void>(TEXT("SceneEditModeUpdating"), _updateID);
		DUOLGameEngine::UIManager::GetInstance()->RemoveImage(this);
	}

	void Image::OnUpdate(float deltaTime)
	{
		_sprite->_rect = _rectTransform->CalculateRect(GraphicsManager::GetInstance()->GetScreenSize());
		_sprite->_offset = _rectTransform->GetPivot();

		_canvas->DrawSprite(_sprite, _orderInLayer);
	}

	void Image::Initialize()
	{
		GameObject* object = DUOLGameEngine::UIManager::GetInstance()->GetCanvas();
		
		if (object == nullptr)
			return;

		SetCanvas(object->GetComponent<Canvas>()->GetCanvas());

		_canvasRectTransform = object->GetComponent<RectTransform>();

		_rectTransform = this->GetGameObject()->GetComponent<RectTransform>();

		LoadTexture(L"Default.png");

		DUOLGameEngine::UIManager::GetInstance()->CreateImage(this);

		_updateID = DUOLGameEngine::EventManager::GetInstance()->AddEventFunction(TEXT("SceneEditModeUpdating"), [this]()
			{
				OnUpdate(1.0f);
			});

	}

	void Image::SetRGB(DUOLMath::Vector3& rgb)
	{
		_rgb = rgb;
	}

	void Image::SetSpriteName(std::string path)
	{
		auto _path = path;
	}

	void Image::SetRaycastTarget(bool israycast)
	{
		_raycastTarget = israycast;
	}

	void Image::OnResize()
	{
		GameObject* object = DUOLGameEngine::UIManager::GetInstance()->GetCanvas();

		if (object == nullptr)
			return;

		SetCanvas(object->GetComponent<Canvas>()->GetCanvas());

		_canvasRectTransform = object->GetComponent<RectTransform>();

		_rectTransform = this->GetGameObject()->GetComponent<RectTransform>();

	}

	void Image::LoadTexture(const DUOLCommon::tstring& textureID)
	{

		_spriteName = textureID;

		std::string path = "UI/" + DUOLCommon::StringHelper::ToString(textureID);

		_sprite->_texture = DUOLGameEngine::ResourceManager::GetInstance()->GetTexture(DUOLCommon::StringHelper::ToTString(path));
	}
}
