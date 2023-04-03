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
#include "DUOLGameEngine/ECS/Object/Sprite.h"

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
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		// 이름 검색 
		, metadata(DUOLCommon::MetaDataType::SerializeByString, true)
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
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		,metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Bool)
	)
	.property("Order In Layer", &DUOLGameEngine::Image::GetLayer,&DUOLGameEngine::Image::SetLayer)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		,metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Int)
	);
}

namespace DUOLGameEngine
{
	Image::Image() :
		BehaviourBase(nullptr, TEXT("Image"))
		, _canvas(nullptr)
		, _orderInLayer(0)
		, _sprite(nullptr)
		, _rectTransform(nullptr)
		, _rgb(DUOLMath::Vector3(255.f, 255.f, 255.f))
		, _raycastTarget(true)
	{
		Initialize();
	}

	Image::Image(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		BehaviourBase(owner, name)
		, _canvas(nullptr)
		, _orderInLayer(0)
		, _sprite(nullptr)
		, _rectTransform(nullptr)
		, _rgb(DUOLMath::Vector3(255.f, 255.f, 255.f))
		, _raycastTarget(true)
	{
		Initialize();
		_sprite = ResourceManager::GetInstance()->GetSprite(L"Default");

	}

	Image::~Image()
	{
		// 이벤트가 돌아가면 종료도 해줘야 한다.
		DUOLGameEngine::EventManager::GetInstance()->RemoveEventFunction<void>(TEXT("SceneEditModeUpdating"), _updateID);
		DUOLGameEngine::UIManager::GetInstance()->RemoveImage(this);
	}

	void Image::OnUpdate(float deltaTime)
	{
		if (!_rectTransform)
			return;

		if (!_sprite)
			return;

		_sprite->GetSprite()->_rect = _rectTransform->CalculateRect(GraphicsManager::GetInstance()->GetScreenSize());
		_sprite->GetSprite()->_offset = _rectTransform->GetPivot();

		_canvas->DrawSprite(_sprite->GetSprite(), _orderInLayer);
	}

	void Image::Initialize()
	{
		GameObject* object = DUOLGameEngine::UIManager::GetInstance()->GetCanvas();

		DUOLGameEngine::UIManager::GetInstance()->CreateImage(this);

		//auto temp = ResourceManager::GetInstance()->GetSprite();
		//LoadTexture(_sprite->GetName());

		_updateID = DUOLGameEngine::EventManager::GetInstance()->AddEventFunction(TEXT("SceneEditModeUpdating"), [this]()
			{
				OnUpdate(1.0f);
			});

		if (object == nullptr)
			return;

		SetCanvas(object->GetComponent<Canvas>()->GetCanvas());

		if (this->GetGameObject() == nullptr)
			return;

		_rectTransform = this->GetGameObject()->GetComponent<RectTransform>();

		_canvasRectTransform = object->GetComponent<RectTransform>();
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

	void Image::SetLayer(int layer)
	{
		if (layer < 0)
			layer = 0;

		_orderInLayer = layer;
	}

	void Image::OnResize()
	{
		GameObject* object = DUOLGameEngine::UIManager::GetInstance()->GetCanvas();

		if (object == nullptr)
			return;

		SetCanvas(object->GetComponent<Canvas>()->GetCanvas());

		_canvasRectTransform = object->GetComponent<RectTransform>();

		//_rectTransform = this->GetGameObject()->GetComponent<RectTransform>();

		/*_sprite->GetSprite()->_rect.left -= (GraphicsManager::GetInstance()->GetScreenSize().x - _sprite->GetSprite()->_rect.left) / 2;
		_sprite->GetSprite()->_rect.right += (GraphicsManager::GetInstance()->GetScreenSize().x - _sprite->GetSprite()->_rect.right) / 2;
		_sprite->GetSprite()->_rect.top -= (GraphicsManager::GetInstance()->GetScreenSize().y - _sprite->GetSprite()->_rect.top) / 2;
		_sprite->GetSprite()->_rect.bottom += (GraphicsManager::GetInstance()->GetScreenSize().y - _sprite->GetSprite()->_rect.bottom) / 2;*/

	}

	void Image::LoadTexture(const DUOLCommon::tstring& textureID)
	{
		if (_sprite == nullptr)
			ResourceManager::GetInstance()->InsertSprite(textureID);

		_sprite = ResourceManager::GetInstance()->GetSprite(textureID);

		_spriteName = textureID;

		_sprite->SetSpriteName(textureID);
	}

	void Image::LoadScene()
	{
		GameObject* object = DUOLGameEngine::UIManager::GetInstance()->GetCanvas();

		if (object == nullptr)
			return;

		SetCanvas(object->GetComponent<Canvas>()->GetCanvas());

		_rectTransform = this->GetGameObject()->GetComponent<RectTransform>();

		_canvasRectTransform = object->GetComponent<RectTransform>();
	}
}
