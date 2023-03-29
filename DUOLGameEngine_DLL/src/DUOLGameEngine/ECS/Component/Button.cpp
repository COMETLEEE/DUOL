#include "DUOLGameEngine/ECS/Component/Button.h"

#include "DUOLGameEngine/ECS/Component/Canvas.h"
#include "DUOLGameEngine/Manager/GraphicsManager.h"
#include "DUOLGameEngine/Manager/ResourceManager.h"
#include "DUOLGameEngine/Manager/UIManager.h"
#include "DUOLCommon/MetaDataType.h"

#include <rttr/registration>

#include "DUOLCommon/Log/LogHelper.h"
#include "DUOLGameEngine/ECS/Component/Image.h"
#include "DUOLGameEngine/Manager/InputManager.h"
#include "DUOLGameEngine/ECS/Component/RectTransform.h"

using namespace rttr;

RTTR_PLUGIN_REGISTRATION
{
	rttr::registration::class_<DUOLGameEngine::Button>("Button")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.property("Color", &DUOLGameEngine::Button::GetRGB,&DUOLGameEngine::Button::SetRGB)
	(
		metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float3)
	)
	.property("None Click Image", &DUOLGameEngine::Button::_downSprite)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		,metadata(DUOLCommon::MetaDataType::SerializeByString, true)
		, metadata(DUOLCommon::MetaDataType::MappingType, DUOLCommon::MappingType::Resource)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::UIFileName)
	);
}

DUOLGameEngine::Button::Button() :
	BehaviourBase()
	, _canvas(nullptr)
	, _rectTransform(nullptr)
	, _rgb(DUOLMath::Vector3(255.f, 255.f, 255.f))
	, _spriteName(L"")
	, _clickSpriteName(L"")
	, _isMouseClick(false)
{
	Initialize();
}

DUOLGameEngine::Button::Button(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
	BehaviourBase(owner, name)
	, _canvas(nullptr)
	, _rectTransform(nullptr)
	, _rgb(DUOLMath::Vector3(255.f, 255.f, 255.f))
	, _spriteName(L"")
	, _clickSpriteName(L"")
	, _isMouseClick(false)
{
	Initialize();
}

DUOLGameEngine::Button::~Button()
{
	// �̺�Ʈ�� ���ư��� ���ᵵ ����� �Ѵ�.
	DUOLGameEngine::EventManager::GetInstance()->RemoveEventFunction<void>(TEXT("SceneEditModeUpdating"), _updateID);
}

void DUOLGameEngine::Button::OnUpdate(float deltaTime)
{

	int a = 0;

	if (_image == nullptr)
		return;

	// Raycast�� ���������� �۵� X
	if (!_image->GetRaycastTarget())
		return;

	// DownSprite������ �۵� X
	if (!_downSprite)
		return;

	// moustpos�� �����´�.
	// ���콺�� �ű� �ִ��� üũ�ϰ� ��ư�� ������ üũ�Ѵ�. 
	// ���Ӻ信 �°� ����ؾ��Ѵ�.
	// ���콺 ��ġ�� �����ɴϴ�.
	auto mousepos = DUOLGameEngine::InputManager::GetInstance()->GetMousePositionInScreen();

	// botton�� �����ϴ� rect�� �����´�. 
	auto buttonpos = _image->GetImageRectTransform()->GetCalculateRect();

	//DUOL_INFO(DUOL_CONSOLE, " 1. mouse pos {} {}", mousepos.x, mousepos.y);
	//DUOL_INFO(DUOL_CONSOLE, " 1. Rect pos left : {} right : {}\n top : {} bottom : {}\n", buttonpos.left, buttonpos.right, buttonpos.top, buttonpos.bottom);

	// ���߿� ��������Ʈ�� ������ �ִٰ� �ٲٴ� �������� �ص��ɵ�
	if (buttonpos.left <= mousepos.x && mousepos.x <= buttonpos.right)
	{
		//DUOL_INFO(DUOL_CONSOLE, " 1. mouse pos {} {}\n", mousepos.x, mousepos.y);

		if (buttonpos.top <= mousepos.y && mousepos.y <= buttonpos.bottom)
		{
			//DUOL_INFO(DUOL_CONSOLE, " 2. mouse pos {} {}\n", mousepos.x, mousepos.y);

			if (!_isMouseClick)
			{
				LoadTexture(_downSprite->GetName());
				_isMouseClick = true;
			}

			if (DUOLGameEngine::InputManager::GetInstance()->GetMouseButtonDown(DUOLGameEngine::MouseCode::Left) || DUOLGameEngine::InputManager::GetInstance()->GetMouseButtonPressed(DUOLGameEngine::MouseCode::Left))
			{

				// Next �̺�Ʈ
			}
		}
		else
		{
			if (_isMouseClick)
			{
				LoadTexture(_spriteName);
				_isMouseClick = false;
			}
		}
	}
	else
	{
		if (_isMouseClick)
		{
			LoadTexture(_spriteName);
			_isMouseClick = false;
		}
	}
}

void DUOLGameEngine::Button::Initialize()
{
	GameObject* object = DUOLGameEngine::UIManager::GetInstance()->GetCanvas();

	GameObject* pickImage = DUOLGameEngine::UIManager::GetInstance()->GetPickingGameObject();
	Image* image = nullptr;

	if (pickImage)
		image = pickImage->GetComponent<Image>();

	_updateID = DUOLGameEngine::EventManager::GetInstance()->AddEventFunction(TEXT("SceneEditModeUpdating"), [this]()
		{
			OnUpdate(1.0f);
		});

	if (object == nullptr)
		return;

	SetCanvas(object->GetComponent<Canvas>()->GetCanvas());

	_canvasRectTransform = object->GetComponent<RectTransform>();

	if (image)
	{
		_image = image;

		_spriteName = _image->GetSpritePathName();

		_rectTransform = _image->GetGameObject()->GetComponent<RectTransform>();
	}
}

void DUOLGameEngine::Button::LoadScene(DUOLGameEngine::Image* image)
{
	_image = image;

	_spriteName = _image->GetSprite()->GetName();

	_rectTransform = _image->GetGameObject()->GetComponent<RectTransform>();

	GameObject* canvasObject = DUOLGameEngine::UIManager::GetInstance()->GetCanvas();

	if (canvasObject == nullptr)
		return;

	SetCanvas(canvasObject->GetComponent<Canvas>()->GetCanvas());

	_canvasRectTransform = canvasObject->GetComponent<RectTransform>();

}

void DUOLGameEngine::Button::SetRGB(DUOLMath::Vector3& rgb)
{
	_rgb = rgb;
}

void DUOLGameEngine::Button::SetDownSprite(const DUOLCommon::tstring& textureID)
{
	if (_downSprite == nullptr)
		ResourceManager::GetInstance()->InsertSprite(textureID);

	_downSpriteName = textureID;

	_downSprite = ResourceManager::GetInstance()->GetSprite(textureID);
}

void DUOLGameEngine::Button::LoadTexture(const DUOLCommon::tstring& textureID)
{

	_image->LoadTexture(textureID);
}


