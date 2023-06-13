#include "DUOLGameEngine/ECS/Component/Button.h"
#include "DUOLGameEngine/ECS/Component/Button.h"
#include "DUOLGameEngine/ECS/Component/Button.h"

#include "DUOLGameEngine/ECS/Component/Canvas.h"
#include "DUOLGameEngine/Manager/GraphicsManager.h"
#include "DUOLGameEngine/Manager/ResourceManager.h"
#include "DUOLGameEngine/Manager/UIManager.h"
#include "DUOLCommon/MetaDataType.h"

#include <rttr/registration>

#include "DUOLCommon/Log/LogHelper.h"
#include "DUOLGameEngine/ECS/Component/Image.h"
#include "DUOLGameEngine/ECS/Component/Text.h"
#include "DUOLGameEngine/ECS/Component/Scrollbar.h"
#include "DUOLGameEngine/Manager/InputManager.h"
#include "DUOLGameEngine/ECS/Component/RectTransform.h"
#include "DUOLGameEngine/Manager/ButtonEventManager.h"

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
	.property("Click Image", &DUOLGameEngine::Button::_spriteName)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("On Click()", &DUOLGameEngine::Button::_onClicks)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::ButtonEvent)
	)
	.method("LoadScene",&DUOLGameEngine::Button::LoadScene)
	.method("EndGame", &DUOLGameEngine::Button::EndGame)
	.method("ScrollBar", &DUOLGameEngine::Button::Scrolling)
	.method("Resolution", &DUOLGameEngine::Button::Resolution);

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
	, _isScrollButton(false)

{
	Initialize();
}

DUOLGameEngine::Button::~Button()
{
	// �̺�Ʈ�� ���ư��� ���ᵵ ����� �Ѵ�.
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
	// �̹����� ������ �־��ְ�
	if (_image == nullptr)
	{
		//�׷��� ������ �ؽ�Ʈ�� �־��ش�. 
		if (SetImage() == false)
			SetText();
	}

	// �̹����� �ؽ�Ʈ ������ �����Ѵ�. 
	if (!_image && !_text)
		return;

	// Image�� �����ϰ� Raycast�� ���������� �۵� X
	if (_image)
	{
		if (!_image->GetRaycastTarget())
			return;
	}

	// moustpos�� �����´�.
	// ���콺�� �ű� �ִ��� üũ�ϰ� ��ư�� ������ üũ�Ѵ�. 
	// ���Ӻ信 �°� ����ؾ��Ѵ�.
	// ���콺 ��ġ�� �����ɴϴ�.
	auto mousepos = DUOLGameEngine::InputManager::GetInstance()->GetMousePosition();
	auto screensize = GraphicsManager::GetInstance()->GetScreenSize();

	// gameview(screensize����) - screensize
	const auto gamescreenviewpos = DUOLGameEngine::UIManager::GetInstance()->GetGameViewPosition();

	// gameView size ������ ã������ ������
	const auto gameviewsize = DUOLGameEngine::UIManager::GetInstance()->GetGameViewSize();
	DUOLGraphicsLibrary::Rect buttonpos;

	if (_image != nullptr)
		buttonpos = _image->GetImageRectTransform()->GetCalculateRect();
	else if (_text != nullptr)
		buttonpos = _text->GetTextRectTransform();

	auto mouseposX = (gameviewsize.x / screensize.x * mousepos.x) - gamescreenviewpos.x;
	auto mouseposY = (gameviewsize.y / screensize.y * mousepos.y) - gamescreenviewpos.y;

	// y���� �´µ� x���� �� ���� �и����� �𸣰ڴ�. �ϴ� screen���� exe���� ����� �� ���̰� ���� �׷��� �׷���
	// 10������ ���̳��� �ϴ� �̷��� �س���
	float left = (gameviewsize.x / screensize.x * buttonpos.left) + gamescreenviewpos.x - 10;
	float right = (gameviewsize.x / screensize.x * buttonpos.right) + gamescreenviewpos.x - 10;
	float top = (gameviewsize.y / screensize.y * buttonpos.top) + gamescreenviewpos.y;
	float bottom = (gameviewsize.y / screensize.y * buttonpos.bottom) + gamescreenviewpos.y;

//	DUOL_INFO(DUOL_CONSOLE, "left : {} / right.x : {} / mouseposX : {} /mouseposY : {}", left, right, mousepos.x, mousepos.y);

	if (left <= mousepos.x && mousepos.x <= right)
	{
		if (top <= mousepos.y && mousepos.y <= bottom)
		{
			if (!_isMouseClick && _image != nullptr)
			{
				LoadTexture(_downSpriteName);
				_isMouseClick = true;
			}

			if (DUOLGameEngine::InputManager::GetInstance()->GetMouseButtonDown(DUOLGameEngine::MouseCode::Left))
			{
				// Next �̺�Ʈ
				OnClicks();
			}
		}
		else
		{
			if (_isMouseClick && _image != nullptr)
			{
				LoadTexture(_spriteName);
				_isMouseClick = false;
			}
		}
	}
	else
	{
		if (_isMouseClick && _image != nullptr)
		{
			LoadTexture(_spriteName);
			_isMouseClick = false;
		}
	}
	if (_isScrollButton && DUOLGameEngine::InputManager::GetInstance()->GetMouseButtonPressed(DUOLGameEngine::MouseCode::Left))
	{
		// x�� ���̸� �����´�. 
		auto nowPosX = DUOLGameEngine::InputManager::GetInstance()->GetMousePosition().x - _downMousePos.x;

		auto parent = this->GetGameObject()->GetComponent<Transform>()->GetParent();

		auto scrollbar = parent->GetGameObject()->GetComponent<DUOLGameEngine::Scrollbar>();

		auto ratio = (scrollbar->GetMaxGauge() / (parent->GetGameObject()->GetComponent<RectTransform>()->GetWidth()-gamescreenviewpos.x));

		// DUOL_INFO(DUOL_CONSOLE, "now.x : {} / downMouse.x : {} / mouseposX : {}", DUOLGameEngine::InputManager::GetInstance()->GetMousePosition().x, _downMousePos.x, nowPosX * ratio);

		float gauge = _scrollGauge + (nowPosX * ratio);

		if (gauge < 0 || gauge > scrollbar->GetMaxGauge())
			return;

		scrollbar->SetNowGauge(gauge);

		// ���� �ٲ��ش�. 
		UIManager::GetInstance()->SetScrollGauge(gauge);
	}
	if (DUOLGameEngine::InputManager::GetInstance()->GetMouseButtonUp(DUOLGameEngine::MouseCode::Left))
	{
		_isScrollButton = false;
		// �ٸ� �����Ϳ� ������ ���� �ʰ� �������ݴϴ�. 
		UIManager::GetInstance()->ReSetScrollGauge();
	}
	for (auto onclick : _onClicks)
		onclick->OnUpdate(deltaTime);
}

void DUOLGameEngine::Button::Initialize()
{
	GameObject* object = DUOLGameEngine::UIManager::GetInstance()->GetCanvas();

	GameObject* pickImage = DUOLGameEngine::UIManager::GetInstance()->GetPickingGameObject();
	Image* image = nullptr;
	Text* text = nullptr;

	if (pickImage)
	{
		image = pickImage->GetComponent<Image>();
		text = pickImage->GetComponent<Text>();
	}
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

	if (text)
	{
		_text = text;

		_rectTransform = _text->GetGameObject()->GetComponent<RectTransform>();
	}

}

void DUOLGameEngine::Button::LoadScene(std::string filename)
{
	auto loadscene = DUOLCommon::StringHelper::ToTString(filename);
	ButtonEventManager::GetInstance()->LoadScene(loadscene);
}

void DUOLGameEngine::Button::EndGame()
{
	ButtonEventManager::GetInstance()->EndGame();
}

// ��ȯ UI ����
// �Ű����� 1���ۿ� �������Ƿ� �ߺ��Լ��� 2�� ��������(����)
void DUOLGameEngine::Button::MainUPUI(std::string filename)
{
	// �� ���ui�� ������ �´�.
	auto parentobject = this->GetGameObject()->GetTransform()->GetParent();

	// �� �ؿ� �ڽ��� ������ �´�. 
	auto childobjects = parentobject->GetChildGameObjects();

	bool check = false;

	// �� �ڽĿ� name���� �˻��Ѵ�.
	for (auto child : childobjects)
	{
		if (child->GetName() == DUOLCommon::StringHelper::ToTString(filename))
		{
			// �� �г��� ��ü �ڽ��� �ҷ��´�. 
			auto panels = child->GetTransform()->GetChildGameObjects();

			int count = 0;

			for (auto panel : panels)
			{
				if (panel->GetIsActive() == true)
				{
					panel->SetIsActiveSelf(false);
					count++;

					if (panels.size() <= count)
						count = 0;

					panels[count]->SetIsActiveSelf(true);
					break;
				}
				count++;
			}
		}
	}
}

// ��ȯ UI ����
void DUOLGameEngine::Button::MainDownUI(std::string filename)
{
	// �� ���ui�� ������ �´�.
	auto parentobject = this->GetGameObject()->GetTransform()->GetParent();

	// �� �ؿ� �ڽ��� ������ �´�. 
	auto childobjects = parentobject->GetChildGameObjects();

	bool check = false;

	// �� �ڽĿ� name���� �˻��Ѵ�.
	for (auto child : childobjects)
	{
		if (child->GetName() == DUOLCommon::StringHelper::ToTString(filename))
		{
			auto panels = child->GetTransform()->GetChildGameObjects();

			int count = 0;

			for (auto panel : panels)
			{
				if (panel->GetIsActive() == true)
				{
					panel->SetIsActiveSelf(false);
					count--;

					if (count <= 0)
						count = panels.size() - 1;

					panels[count]->SetIsActiveSelf(true);
					break;
				}
				count++;
			}
		}
	}
}

void DUOLGameEngine::Button::Resolution(int num)
{
	//// ��ȹ�ڰ� x �������ְ� X ������ �����ϱ�..
	//auto findChar = inputtext.find("x");

	//if (findChar == std::string::npos)
	//{
	//	findChar = inputtext.find("X");
	//}

	//auto screenXstring = inputtext.substr(0, findChar);
	//auto screenYstring = inputtext.substr(findChar + 1, inputtext.size());

	//int screenX = stoi(screenXstring);
	//int screenY = stoi(screenYstring);

	UIManager::GetInstance()->Resolution(num, this);


	/*DUOLMath::Vector2 screensize;
	screensize.x = screenX;
	screensize.y = screenY;
	

	DUOLGameEngine::InputManager::GetInstance()->SetWindowSize(screensize);*/
	// �ܼ��ϰ� �ػ� ����
	//DUOLGameEngine::UIManager::GetInstance()->SetGameViewSize(screensize);
	//EventManager::GetInstance()->InvokeEvent<std::any>(TEXT("Resize"), &screensize);
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

bool DUOLGameEngine::Button::SetText()
{
	auto object = this->GetGameObject();
	for (auto component : object->GetAllComponents())
	{
		if (component->GetName() == L"Text")
		{
			_text = static_cast<Text*>(component);
			return true;
		}
	}
	return false;
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

void DUOLGameEngine::Button::SetLoadSceneText(DUOLGameEngine::Text* text)
{
	_text = text;

	_rectTransform = text->GetGameObject()->GetComponent<RectTransform>();

	GameObject* canvasObject = DUOLGameEngine::UIManager::GetInstance()->GetCanvas();

	if (canvasObject == nullptr)
		return;

	SetCanvas(canvasObject->GetComponent<Canvas>()->GetCanvas());

	_canvasRectTransform = canvasObject->GetComponent<RectTransform>();
}

void DUOLGameEngine::Button::SetSpriteName(const DUOLCommon::tstring& textureID)
{
	_spriteName = textureID;
}

bool DUOLGameEngine::Button::SetImage()
{
	auto object = this->GetGameObject();
	for (auto component : object->GetAllComponents())
	{
		if (component->GetName() == L"Image")
		{
			_image = static_cast<Image*>(component);
			_spriteName = _image->GetSpritePathName();
			return true;
		}
	}
	return false;
}

void DUOLGameEngine::Button::LoadTexture(const DUOLCommon::tstring& textureID)
{
	_image->LoadTexture(textureID);
}

void DUOLGameEngine::Button::Scrolling()
{
	// Ŭ���� ������ ��ǥ�� �����Ѵ�. 
	_downMousePos = DUOLGameEngine::InputManager::GetInstance()->GetMousePosition();

	auto parent = this->GetGameObject()->GetComponent<Transform>()->GetParent();

	// �θ� ������ �Լ��� ������. 
	if (!parent)
		return;

	auto scrollbar = parent->GetGameObject()->GetComponent<DUOLGameEngine::Scrollbar>();

	// ��ũ�ѹٰ� �ƴϸ� ������. 
	if (!scrollbar)
		return;

	_scrollGauge = scrollbar->GetNowGauge();

	// ��ũ�� ��ư���� üũ�Ѵ�. 
	_isScrollButton = true;

}
