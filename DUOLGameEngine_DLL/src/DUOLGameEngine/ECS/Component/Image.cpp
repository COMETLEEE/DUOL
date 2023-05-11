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
#include "DUOLMath/DUOLMath.h"

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
	.property("Source Image", &DUOLGameEngine::Image::GetSpritePathName,&DUOLGameEngine::Image::SetSpritePathName)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::UIFileName)
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
	)
	.property("Is GaugeBar", &DUOLGameEngine::Image::GetGaugeBar,&DUOLGameEngine::Image::SetGaugeBar)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		,metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Bool)
	);
}

namespace DUOLGameEngine
{
	Image::Image() :
		BehaviourBase(nullptr, TEXT("Image"))
		, _canvas(nullptr)
		, _orderInLayer(0)
		, _sprite(new Sprite())
		, _rectTransform(nullptr)
		, _raycastTarget(true)
		, _isGaugeBar(false)
		, _spriteName(L"None(Sprite)")
	{
		Initialize(nullptr);
	}

	Image::Image(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		BehaviourBase(owner, name)
		, _canvas(nullptr)
		, _orderInLayer(0)
		, _sprite(new Sprite())
		, _rectTransform(nullptr)
		, _raycastTarget(true)
		, _isGaugeBar(false)
		, _spriteName(L"None(Sprite)")
	{
		Initialize(owner);
	}

	Image::~Image()
	{
		// 이벤트가 돌아가면 종료도 해줘야 한다.
		DUOLGameEngine::EventManager::GetInstance()->RemoveEventFunction<void>(TEXT("SceneEditModeUpdating"), _updateID);
		DUOLGameEngine::UIManager::GetInstance()->RemoveImage(this);
	}

	void Image::OnUpdate(float deltaTime)
	{
		if (_isEnabled == false)
			return;

		if (!_rectTransform)
			_rectTransform = this->GetGameObject()->GetComponent<RectTransform>();

		if (!_sprite)
			return;

		if (!_canvas)
		{
			DUOL_TRACE(DUOL_CONSOLE, "Canvas Null.");

			auto object = this->GetGameObject()->GetTransform()->GetParent();
			while (object->GetParent() != nullptr)
			{
				object = object->GetParent();
			}
			SetCanvas(object->GetGameObject()->GetComponent<Canvas>()->GetCanvas());
		}

		if (_sprite->GetSprite()->_texture == nullptr)
			LoadTexture(_spriteName);

		if (this->GetGameObject() != nullptr && this->GetGameObject()->GetIsActive() == false)
			return;

		if (_isGaugeBar)
			GaugeImageRender();
		else
			ImageRender();

	}

	void Image::Initialize(DUOLGameEngine::GameObject* owner)
	{
		GameObject* object = DUOLGameEngine::UIManager::GetInstance()->GetCanvas();

		DUOLGameEngine::UIManager::GetInstance()->CreateImage(this);

		_updateID = DUOLGameEngine::EventManager::GetInstance()->AddEventFunction(TEXT("SceneEditModeUpdating"), [this]()
			{
				OnUpdate(1.0f);
			});

		if (object == nullptr)
			return;

		SetCanvas(object->GetComponent<Canvas>()->GetCanvas());

		if (owner == nullptr)
			return;

		_rectTransform = this->GetGameObject()->GetComponent<RectTransform>();

		_canvasRectTransform = object->GetComponent<RectTransform>();

	}

	void Image::SetCanvas(DUOLGraphicsLibrary::ICanvas* canvas)
	{
		_canvas = canvas;
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

	// 텍스쳐가 끊긴다. 연결해줘야함.
	void Image::OnResize()
	{
		GameObject* object = DUOLGameEngine::UIManager::GetInstance()->GetCanvas();

		if (object == nullptr)
			return;

		SetCanvas(object->GetComponent<Canvas>()->GetCanvas());

		_canvasRectTransform = object->GetComponent<RectTransform>();
	}

	void Image::LoadTexture(const DUOLCommon::tstring& textureID)
	{
		std::string path = "UI/" + DUOLCommon::StringHelper::ToString(textureID);
		auto tstringPath = DUOLCommon::StringHelper::ToTString(path);

		if (_sprite == nullptr)
			ResourceManager::GetInstance()->InsertSprite(tstringPath);

		_sprite->GetSprite()->_texture = ResourceManager::GetInstance()->GetTexture(tstringPath);

		_spriteName = textureID;

		_sprite->SetSpriteName(tstringPath);
	}

	void Image::LoadScene()
	{
		// 여기서 게임 오브젝트가 바뀐다.
		GameObject* object = DUOLGameEngine::UIManager::GetInstance()->GetCanvas();

		if (object == nullptr)
			return;

		SetCanvas(object->GetComponent<Canvas>()->GetCanvas());

		_rectTransform = this->GetGameObject()->GetComponent<RectTransform>();

		_canvasRectTransform = object->GetComponent<RectTransform>();
	}

	void Image::ImageRender()
	{
		if (_rectTransform->_dirtyFlagRotate)
		{
			_sprite->GetSprite()->_angle = _rectTransform->GetRotation().z;
			_rectTransform->_dirtyFlagRotate = false;
		}

		if (_rectTransform->_dirtyFlagScale)
		{
			_sprite->GetSprite()->_scale = DUOLMath::Vector2(_rectTransform->GetScale().x, _rectTransform->GetScale().y);
			_rectTransform->_dirtyFlagScale = false;
		}

		if (_sprite->GetSprite()->_scale.x == 0|| _sprite->GetSprite()->_scale.y == 0)
			DUOL_TRACE(DUOL_CONSOLE, "scale Null.");

		if(_sprite->GetSprite()==nullptr)
			DUOL_TRACE(DUOL_CONSOLE, "sprite Null.");

		_sprite->GetSprite()->_rect = _rectTransform->CalculateRect(GraphicsManager::GetInstance()->GetScreenSize());
		_sprite->GetSprite()->_pivot = _rectTransform->GetPivot();

		_canvas->DrawSprite(_sprite->GetSprite(), _orderInLayer);
	}

	// 이 이미지가 부모를 기준으로 가변해야한다는 가정이 들어간다.
	void Image::GaugeImageRender()
	{
		// 부모가 있다고 가정한다.
		auto parentObject = this->GetGameObject()->GetTransform()->GetParent();

		if (!parentObject)
			return;

		// 부모 이미지를 가지고 온다. 
		auto parentImage = parentObject->GetGameObject()->GetComponent<DUOLGameEngine::Image>();

		// 부모와 동일한 스케일와 사이즈를 가집니다.
		// 외부에서 수정하게 놔둬도 되고..? 일단은 그냥 부모에 종속되게 했습니다.
		// 부모 + 자기자신도 괜찮을듯?
		_sprite->GetSprite()->_angle = parentImage->GetSprite()->GetSprite()->_angle;

		_sprite->GetSprite()->_scale = parentImage->GetSprite()->GetSprite()->_scale;

		// 무조건 부모보다 높은 레이어를 가진다. 
		_orderInLayer = parentImage->GetLayer() + 1;

		// 이전과 다른 Rect 계산을 해야한다.
		// 부모를 기준으로 좌상단은 동일하게 맞춥니다.
		// 게이지는 항상 왼쪽부터 차니깐..?
		_sprite->GetSprite()->_rect.left = parentImage->GetSprite()->GetSprite()->_rect.left;
		_sprite->GetSprite()->_rect.top = parentImage->GetSprite()->GetSprite()->_rect.top;

		// Box 크기 계산만해서 넘겨줍니다. 

		_sprite->GetSprite()->_pivot = _rectTransform->GetPivot();

		_canvas->DrawSprite(_sprite->GetSprite(), _orderInLayer);
	}
}
