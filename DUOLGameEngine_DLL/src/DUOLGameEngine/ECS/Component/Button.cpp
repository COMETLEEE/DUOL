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
	);
}

DUOLGameEngine::Button::Button() :
	BehaviourBase()
	, _canvas(nullptr)
	, _rectTransform(nullptr)
	, _rgb(DUOLMath::Vector3(255.f, 255.f, 255.f))
{
	Initialize();
}

DUOLGameEngine::Button::Button(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
	BehaviourBase(owner, name)
	, _canvas(nullptr)
	, _rectTransform(nullptr)
	, _rgb(DUOLMath::Vector3(255.f, 255.f, 255.f))
{
	Initialize();
}

DUOLGameEngine::Button::~Button()
{
	// 이벤트가 돌아가면 종료도 해줘야 한다.
	DUOLGameEngine::EventManager::GetInstance()->RemoveEventFunction<void>(TEXT("SceneEditModeUpdating"), _updateID);
}

void DUOLGameEngine::Button::OnUpdate(float deltaTime)
{
	if (!ImageCheck())
		return;

	// Raycast가 꺼져있으면 작동 X
	if (!_image->GetRaycastTarget())
		return;

	// moustpos를 가져온다.
	// 마우스가 거기 있는지 체크하고 버튼이 눌린지 체크한다. 
	// 게임뷰에 맞게 계산해야한다.
	// 마우스 위치를 가져옵니다.
	auto mousepos = DUOLGameEngine::InputManager::GetInstance()->GetMousePositionInScreen();

	// botton이 존재하는 rect를 가져온다. 
	auto buttonpos = _image->GetSprite()->_rect;

	
	DUOL_INFO(DUOL_CONSOLE, " 1. mouse pos {} {}", mousepos.x, mousepos.y);
	DUOL_INFO(DUOL_CONSOLE, " 1. Rect pos left : {} right : {}\n top : {} bottom : {}\n", buttonpos.left, buttonpos.right, buttonpos.top, buttonpos.bottom);

	// 나중에 스프라이트를 가지고 있다가 바꾸는 형식으로 해도될듯
	if (buttonpos.left <= mousepos.x && mousepos.x <= buttonpos.right)
	{
		DUOL_INFO(DUOL_CONSOLE, " 1. mouse pos {} {}\n", mousepos.x, mousepos.y);

		if (buttonpos.top <= mousepos.y && mousepos.y <= buttonpos.bottom)
		{
			DUOL_INFO(DUOL_CONSOLE, " 2. mouse pos {} {}\n", mousepos.x, mousepos.y);

			if (DUOLGameEngine::InputManager::GetInstance()->GetMouseButtonDown(DUOLGameEngine::MouseCode::Left)|| DUOLGameEngine::InputManager::GetInstance()->GetMouseButtonPressed(DUOLGameEngine::MouseCode::Left))
			{
				LoadTexture(L"Arrow2.png");
			}
			else
			{
				LoadTexture(L"Arrow.png");
			}
		}
	}
}

void DUOLGameEngine::Button::Initialize()
{
	// 현재 선택된 Object에서 그 안에 있는 Image를 가져오기위해 그 오브젝트를 가져온다. 
	GameObject* pickObject = DUOLGameEngine::UIManager::GetInstance()->GetPickingGameObject();
	GameObject* canvasObject = DUOLGameEngine::UIManager::GetInstance()->GetCanvas();


	if (pickObject == nullptr)
		return;
	
	Image* nowimage = pickObject->GetComponent<Image>();

	// Image가 없으면 그리지 않게 만든다. 
	if (nowimage == nullptr)
	{
		_rectTransform = this->GetGameObject()->GetComponent<RectTransform>();
	}
	else
	{
		_image = nowimage;

		_rectTransform = nowimage->GetGameObject()->GetComponent<RectTransform>();
	}

	if (canvasObject == nullptr)
		return;

	SetCanvas(canvasObject->GetComponent<Canvas>()->GetCanvas());

	_canvasRectTransform = canvasObject->GetComponent<RectTransform>();

	_updateID = DUOLGameEngine::EventManager::GetInstance()->AddEventFunction(TEXT("SceneEditModeUpdating"), [this]()
		{
			OnUpdate(1.0f);
		});
}


void DUOLGameEngine::Button::SetRGB(DUOLMath::Vector3& rgb)
{
	_rgb = rgb;
}

void DUOLGameEngine::Button::LoadTexture(const DUOLCommon::tstring& textureID)
{
	_spriteName = textureID;

	//std::string path = "UI/" + DUOLCommon::StringHelper::ToString(textureID);

	//_sprite->_texture = DUOLGameEngine::ResourceManager::GetInstance()->GetTexture(DUOLCommon::StringHelper::ToTString(path));

	_image->LoadTexture(textureID);
}

bool DUOLGameEngine::Button::ImageCheck()
{
	GameObject* pickObject = DUOLGameEngine::UIManager::GetInstance()->GetPickingGameObject();

	if (pickObject == nullptr)
		return false;

	Image* nowimage = pickObject->GetComponent<Image>();

	// 이미지가 없으면 작동 X
	if (nowimage == nullptr)
		return false;
	else
	{
		_image = nowimage;

		_rectTransform = nowimage->GetGameObject()->GetComponent<RectTransform>();

		return true;
	}
}

