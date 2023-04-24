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
	.property("None Click Image", &DUOLGameEngine::Button::_downSpriteName)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::UIFileName)
	)
	.property("On Click()", &DUOLGameEngine::Button::_onClicks)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::ButtonEvent)
	)
	.method("LoadScene",&DUOLGameEngine::Button::LoadScene);
}

DUOLGameEngine::Button::Button() :
	BehaviourBase()
	, _canvas(nullptr)
	, _rectTransform(nullptr)
	, _rgb(DUOLMath::Vector3(255.f, 255.f, 255.f))
	, _spriteName(L"")
	, _clickSpriteName(L"")
	, _isMouseClick(false)
	, _loadSceneName(L"")
	, _downSpriteName(L"None(Sprite)")
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
	, _loadSceneName(L"")
	, _downSpriteName(L"None(Sprite)")
{
	Initialize();
}

DUOLGameEngine::Button::~Button()
{
	// 이벤트가 돌아가면 종료도 해줘야 한다.
	// DUOLGameEngine::EventManager::GetInstance()->RemoveEventFunction<void>(TEXT("SceneEditModeUpdating"), _updateID);
}

void DUOLGameEngine::Button::OnAwake()
{
	for (auto click : _onClicks)
	{
		click->OnAwake();
	}
}

void DUOLGameEngine::Button::OnUpdate(float deltaTime)
{
	if (_image == nullptr)
		return;

	// Raycast가 꺼져있으면 작동 X
	if (!_image->GetRaycastTarget())
		return;

	// moustpos를 가져온다.
	// 마우스가 거기 있는지 체크하고 버튼이 눌린지 체크한다. 
	// 게임뷰에 맞게 계산해야한다.
	// 마우스 위치를 가져옵니다.
	auto mousepos = DUOLGameEngine::InputManager::GetInstance()->GetMousePosition();
	auto screensize = GraphicsManager::GetInstance()->GetScreenSize();

	// gameview(screensize기준) - screensize
	const auto gamescreenviewpos = DUOLGameEngine::UIManager::GetInstance()->GetGameViewPosition();

	// gameView size 비율을 찾기위해 가져옴
	const auto gameviewsize = DUOLGameEngine::UIManager::GetInstance()->GetGameViewSize();
	auto buttonpos = _image->GetImageRectTransform()->GetCalculateRect();

	auto mouseposX = (gameviewsize.x / screensize.x * mousepos.x) - gamescreenviewpos.x;
	auto mouseposY = (gameviewsize.y / screensize.y * mousepos.y) - gamescreenviewpos.y;

	// y축은 맞는데 x축이 왜 조금 밀리는지 모르겠다. 일단 screen에서 exe까지 사이즈도 좀 차이가 나고 그래서 그런듯
	// 10정도가 차이나서 일단 이렇게 해놓음
	float left = (gameviewsize.x / screensize.x * buttonpos.left) + gamescreenviewpos.x - 10;
	float right = (gameviewsize.x / screensize.x * buttonpos.right) + gamescreenviewpos.x - 10;
	float top = (gameviewsize.y / screensize.y * buttonpos.top) + gamescreenviewpos.y;
	float bottom = (gameviewsize.y / screensize.y * buttonpos.bottom) + gamescreenviewpos.y;

	//DUOL_INFO(DUOL_CONSOLE, " 1. mouse pos {} {}", mousepos.x, mousepos.y);
	//DUOL_INFO(DUOL_CONSOLE, " 1. Rect pos left : {} right : {}\n top : {} bottom : {}\n", left, right, top, bottom);

	// 나중에 스프라이트를 가지고 있다가 바꾸는 형식으로 해도될듯
	if (left <= mousepos.x && mousepos.x <= right)
	{
		if (top <= mousepos.y && mousepos.y <= bottom)
		{
			//DUOL_INFO(DUOL_CONSOLE, " 1. mouse pos {} {}\n", mousepos.x, mousepos.y);
			//DUOL_INFO(DUOL_CONSOLE, " 1. Rect pos left : {} right : {}\n top : {} bottom : {}\n", left,right, top, bottom);

			//DUOL_INFO(DUOL_CONSOLE, " 2. mouse pos {} {}\n", mousepos.x, mousepos.y);

			if (!_isMouseClick)
			{
				LoadTexture(_downSpriteName);
				_isMouseClick = true;
			}

			if (DUOLGameEngine::InputManager::GetInstance()->GetMouseButtonDown(DUOLGameEngine::MouseCode::Left) || DUOLGameEngine::InputManager::GetInstance()->GetMouseButtonPressed(DUOLGameEngine::MouseCode::Left))
			{
				// Next 이벤트
				OnClicks();
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

	for (auto onclick : _onClicks)
		onclick->OnUpdate(deltaTime);
}

void DUOLGameEngine::Button::Initialize()
{
	GameObject* object = DUOLGameEngine::UIManager::GetInstance()->GetCanvas();

	GameObject* pickImage = DUOLGameEngine::UIManager::GetInstance()->GetPickingGameObject();
	Image* image = nullptr;

	if (pickImage)
		image = pickImage->GetComponent<Image>();

	/*_updateID = DUOLGameEngine::EventManager::GetInstance()->AddEventFunction(TEXT("SceneEditModeUpdating"), [this]()
		{
			OnUpdate(1.0f);
		});*/

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

void DUOLGameEngine::Button::LoadScene(std::string filename)
{

	int test = 0;

}

void DUOLGameEngine::Button::CreateOnClick()
{
	auto newClick = new OnClick();

	newClick->Initialize();

	_onClicks.emplace_back(newClick);
}

void DUOLGameEngine::Button::DeleteOnClick()
{
	if (_onClicks.empty())
		return;

	_onClicks.pop_back();
}

void DUOLGameEngine::Button::OnClicks()
{
	for (auto onclick : _onClicks)
	{
		onclick->Invoke();
	}
}

void DUOLGameEngine::Button::SetRGB(DUOLMath::Vector3& rgb)
{
	_rgb = rgb;
}

void DUOLGameEngine::Button::SetDownSprite(const DUOLCommon::tstring& textureID)
{
	_downSpriteName = textureID;
}

void DUOLGameEngine::Button::SetLoadSceneName(DUOLCommon::tstring& scenename)
{
	_loadSceneName = scenename;
}

void DUOLGameEngine::Button::SetLoadSceneImage(DUOLGameEngine::Image* image)
{
	_image = image;

	_spriteName = _image->GetSpritePathName();

	_rectTransform = _image->GetGameObject()->GetComponent<RectTransform>();

	GameObject* canvasObject = DUOLGameEngine::UIManager::GetInstance()->GetCanvas();

	if (canvasObject == nullptr)
		return;

	SetCanvas(canvasObject->GetComponent<Canvas>()->GetCanvas());

	_canvasRectTransform = canvasObject->GetComponent<RectTransform>();
}

void DUOLGameEngine::Button::LoadTexture(const DUOLCommon::tstring& textureID)
{
	_image->LoadTexture(textureID);
}


