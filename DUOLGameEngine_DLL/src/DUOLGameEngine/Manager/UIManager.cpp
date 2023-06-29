#include "DUOLGameEngine/Manager/UIManager.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Canvas.h"
#include "DUOLGameEngine/ECS/Component/Image.h"
#include "DUOLGameEngine/ECS/Component/Text.h"
#include <filesystem>

#include "DUOLGameEngine/ECS/Component/Button.h"
#include "DUOLGameEngine/ECS/Component/Scrollbar.h"
#include "DUOLGameEngine/Manager/ResourceManager.h"
#include "DUOLGameEngine/ECS/Component/Camera.h"
#include "DUOLGameEngine/Manager/ButtonEventManager.h"
#include "DUOLGameEngine/Manager/CutSceneManager.h"
#include "DUOLGameEngine/Manager/GraphicsManager.h"
#include "DUOLGameEngine/Manager/InputManager.h"
#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"

namespace  DUOLGameEngine
{
	class Canvas;

	DUOLGameEngine::UIManager::UIManager() :
		_isCanvas(false)
		, _canvasList()
		, _imageList()
		, _buttonList()
		, _imageFileNames()
		, _fileNames()
		, _nowPickingObject(nullptr)
		, _scrollGauge(-1)
		, _nowResolution(1600,1080)
		, _resolutions()
	{
	}

	DUOLGameEngine::UIManager::~UIManager()
	{
		ResetScene();
	}

	void DUOLGameEngine::UIManager::Initialize()
	{
		// 순서가 중요함
		LoadSpriteFile();
		CreateTextureFile();

		// font를 불러와서 전부 만들어준다. 
		CreateFontType();

		SetGameViewSize(GraphicsManager::GetInstance()->GetScreenSize());

		_resolutions.emplace_back(DUOLMath::Vector2(2560, 1440));
		_resolutions.emplace_back(DUOLMath::Vector2(1920, 1080));
		_resolutions.emplace_back(DUOLMath::Vector2(1280, 720));
	}

	void UIManager::InitializeCurrentGameScene(const std::list<std::shared_ptr<DUOLGameEngine::GameObject>>& rootObjectsInScene)
	{
		ResetScene();
		// Image들을 Canvas에 연결해 줍니다. 
		for (auto& object : rootObjectsInScene)
		{
			if (object->GetComponent<Canvas>() != nullptr)
			{
				_canvasList.emplace_back(object.get());

				// canvas도 resize해줘야한다. 
				auto screensize = GraphicsManager::GetInstance()->GetScreenSize();
				object->GetComponent<Canvas>()->SetRatio(screensize.x, screensize.y);

				_isCanvas = true;
				if (object->GetTransform()->GetChildGameObjects().size() != 0)
				{
					// 자식의 자식의 자식의 자식까지 순환을 못해서 문제가 생김
					for (auto childObjects : object->GetTransform()->GetChildGameObjects())
					{
						LoadComponent(*childObjects);
					}
				}
			}
		}
		LoadResolution();

		if (DUOLGameEngine::SceneManager::GetInstance()->GetSceneName() == L"CutScene")
			StartCutScene();
	}

	// 재귀를 돌면서 자식객체까지 모두 찾아준다. 
	void UIManager::LoadComponent(DUOLGameEngine::GameObject& gameobject)
	{
		if (gameobject.GetTransform()->GetChildGameObjects().size() != 0)
		{
			for (auto childObjects : gameobject.GetTransform()->GetChildGameObjects())
			{
				LoadComponent(*childObjects);
			}
		}

		auto image = gameobject.GetComponent<Image>();
		auto button = gameobject.GetComponent<Button>();
		auto text = gameobject.GetComponent<Text>();

		if (image)
		{
			_imageList.emplace_back(image);

			image->LoadScene();
		}
		if (button)
		{
			_buttonList.emplace_back(button);

			if (image)
				button->SetLoadSceneImage(image);
			else if (text)
				button->SetLoadSceneText(text);
		}
		if (text)
		{
			_textList.emplace_back(text);

			text->LoadScene();
		}
	}

	void DUOLGameEngine::UIManager::UnInitialize()
	{
	}

	void DUOLGameEngine::UIManager::Update(float deltaTime)
	{
		int a = 0;

		CutSceneManager::GetInstance()->Update(deltaTime);
	}

	void UIManager::StartCutScene()
	{
		DUOLGameEngine::CutSceneManager::GetInstance()->StartCutScene();
	}

	void UIManager::ResetScene()
	{
		_canvasList.clear();
		_imageList.clear();
		_buttonList.clear();
		_textList.clear();
		_isCanvas = false;
	}

	void DUOLGameEngine::UIManager::CreateCanvas(GameObject* object)
	{
		_canvasList.emplace_back(object);
	}

	void UIManager::CreateImage(Image* image)
	{
		_imageList.emplace_back(image);
	}

	void UIManager::CreateText(Text* text)
	{
		_textList.emplace_back(text);
	}

	void UIManager::RemoveCanvas(GameObject* canvas)
	{
		_canvasList.erase(remove(_canvasList.begin(), _canvasList.end(), canvas), _canvasList.end());

		_nowPickingObject = nullptr;

		if (_canvasList.size() == 0)
			_isCanvas = false;
	}

	void UIManager::RemoveImage(Image* image)
	{
		_imageList.erase(remove(_imageList.begin(), _imageList.end(), image), _imageList.end());

		_nowPickingObject = nullptr;
	}

	void UIManager::RemoveText(Text* text)
	{
		_textList.erase(remove(_textList.begin(), _textList.end(), text), _textList.end());

		_nowPickingObject = nullptr;
	}

	Image* UIManager::FindImage(std::string name)
	{
		for(auto imageUI : _imageList)
		{
			if (DUOLCommon::StringHelper::ToTString(name) == imageUI->GetGameObject()->GetName())
				return imageUI;
		}

		return nullptr;
	}

	Button* UIManager::FindButton(std::string name)
	{
		for (auto buttonUI : _buttonList)
		{
			if (DUOLCommon::StringHelper::ToTString(name) == buttonUI->GetGameObject()->GetName())
				return buttonUI;
		}

		return nullptr;
	}

	Text* UIManager::FindText(std::string name)
	{
		for (auto textUI : _textList)
		{
			if (DUOLCommon::StringHelper::ToTString(name) == textUI->GetGameObject()->GetName())
				return textUI;
		}

		return nullptr;
	}

	void UIManager::SetScrollBarUI(std::string name,float gauge)
	{
		auto findUIImage= FindImage(name);
		if (findUIImage == nullptr)
			return;

		auto scrollbar=findUIImage->GetGameObject()->GetComponent<Scrollbar>();
		if (scrollbar == nullptr)
			return;

		scrollbar->SetNowGauge(gauge);
	}

	void UIManager::SetScrollBarUIMaxGauge(std::string name, float gauge)
	{
		auto findUIImage = FindImage(name);
		if (findUIImage == nullptr)
			return;

		auto scrollbar = findUIImage->GetGameObject()->GetComponent<Scrollbar>();
		if (scrollbar == nullptr)
			return;
		scrollbar->SetMaxGauge(gauge);
	}

	void UIManager::OnResize(int width, int height)
	{
		for (auto object : _canvasList)
		{
			if (object->GetComponent<Canvas>())
				object->GetComponent<Canvas>()->OnResize(width, height);
		}

		for (auto object : _imageList)
		{
			object->OnResize();
		}

		for (auto object : _textList)
		{
			object->OnResize();
		}
	}

	GameObject* UIManager::GetCanvas()
	{
		if (_canvasList.empty())
			return nullptr;
		return _canvasList.back();
	}

	DUOLGameEngine::Image* UIManager::GetImage(DUOLCommon::UUID imageid)
	{
		for (auto object : _imageList)
		{
			if (object->GetGameObject()->GetUUID() == imageid)
				return object;
		}

		return nullptr;
	}

	void UIManager::LoadSpriteFile()
	{
		auto path = std::filesystem::current_path();

		for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator("Asset/Texture/UI"))
		{
			std::string spritename = entry.path().filename().generic_string();
			std::string spritepath = entry.path().generic_string();

			_imageFileNames.emplace_back(DUOLCommon::StringHelper::ToTString(spritename));
		}
	}

	// 모든 UI Texture, Sprite를 미리 만들어 놓는다. 
	void UIManager::CreateTextureFile()
	{
		for (auto filename : _imageFileNames)
		{
			ResourceManager::GetInstance()->InsertSprite(filename);
		}
	}

	void UIManager::CreateFontType()
	{
		auto path = std::filesystem::current_path();

		for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator("Asset/Font"))
		{
			std::string fontname = entry.path().filename().generic_string();
			std::string spritepath = entry.path().generic_string();

			DUOLGameEngine::ResourceManager::GetInstance()->CreateIFont(DUOLCommon::StringHelper::ToTString(spritepath));

			_fontList.emplace_back(DUOLCommon::StringHelper::ToTString(fontname));
		}
	}

	void UIManager::Resolution(int num,Button* button)
	{
		if (_resolutions.size() < num)
			return;

		DUOLMath::Vector2 changeResolution = _resolutions[num];

		DUOLGameEngine::InputManager::GetInstance()->SetWindowSize(changeResolution);

		_resolutions[num] = _nowResolution;
		_nowResolution = changeResolution;

		std::string changeImageName = "Resolution" + std::to_string(num);

		ChangeImage("ResolutionNowImage", changeImageName);

		Image* resolutionChange = FindImage(changeImageName);
		button->SetSpriteName(resolutionChange->GetSpritePathName());
		button->SetDownSprite(resolutionChange->GetSpritePathName());

	}

	void UIManager::ChangeImage(std::string nowimage, std::string changeimage)
	{
		Image* resolutionNowImage = FindImage(nowimage);
		Image* resolutionChange = FindImage(changeimage);

		DUOLCommon::tstring nowImageStr = DUOLCommon::StringHelper::ToTString(resolutionNowImage->GetSpritePathName());
		DUOLCommon::tstring changeImageStr = DUOLCommon::StringHelper::ToTString(resolutionChange->GetSpritePathName());

		resolutionNowImage->LoadTexture(changeImageStr);
		resolutionChange->LoadTexture(nowImageStr);
	}

	void UIManager::SetActiveImage(std::string path,bool value)
	{
		auto findImage = FindImage(path);
		if(findImage == nullptr)
			return;
		findImage->GetGameObject()->SetIsActiveSelf(value);
	}

	void UIManager::SetActiveButton(std::string path, bool value)
	{
		auto findbut = FindButton(path);
		if (findbut == nullptr)
			return;
		findbut->GetGameObject()->SetIsActiveSelf(value);
	}

	void UIManager::SetActiveText(std::string path, bool value)
	{
		auto findText = FindText(path);
		if (findText == nullptr)
			return;
		findText->GetGameObject()->SetIsActiveSelf(value);
	}

	void UIManager::SetText(std::string path, std::string text)
	{
		auto findText = FindText(path);
		if (findText == nullptr)
			return;
		DUOLCommon::tstring tText = DUOLCommon::StringHelper::ToTString(text);
		findText->SetText(tText);
	}

	void UIManager::LoadImage(std::string path)
	{

	}

	float UIManager::GetScrollButtonData()
	{
		return _scrollGauge;
	}

	void UIManager::DestoryButtonTargetGameObject(DUOLGameEngine::GameObject* gameobject)
	{
		// object가 지워질때 button에서도 지워준다. 
		for (auto object : _buttonList)
		{
			for (auto onclick : object->GetOnClick())
			{
				if (onclick->GetCalls()->GetTargetObject() == gameobject)
				{
					onclick->GetCalls()->SetTargetGameObject(nullptr);
				}
			}
		}
	}

	void UIManager::LoadResolution()
	{
		if (_nowResolution == DUOLMath::Vector2(1600, 1080))
			return;

		std::string path;

		Image* resolutionNow = FindImage("ResolutionNowImage");
		Image* resolution0 = FindImage("Resolution0");
		Image* resolution1 = FindImage("Resolution1");
		Image* resolution2 = FindImage("Resolution2");

		std::vector<Image*> resolutionList;
		resolutionList.emplace_back(resolution0);
		resolutionList.emplace_back(resolution1);
		resolutionList.emplace_back(resolution2);

		Button* button0 = FindButton("Resolution0");
		Button* button1 = FindButton("Resolution1");
		Button* button2 =FindButton("Resolution2");

		std::vector<Button*> buttonList;
		buttonList.emplace_back(button0);
		buttonList.emplace_back(button1);
		buttonList.emplace_back(button2);

		path = "02_settings_";
		int x = static_cast<int>(_nowResolution.x);
		int y = static_cast<int>(_nowResolution.y);

		path += (std::to_string(x) + "_" + std::to_string(y));
		path += ".png";

		resolutionNow->LoadTexture(DUOLCommon::StringHelper::ToTString(path));

		for(int count =0; count< _resolutions.size(); count++)
		{
			path = "02_settings_";

			x = static_cast<int>(_resolutions[count].x);
			y = static_cast<int>(_resolutions[count].y);

			path += (std::to_string(x) + "_" + std::to_string(y));
			path += ".png";

			resolutionList[count]->LoadTexture(DUOLCommon::StringHelper::ToTString(path));
			buttonList[count]->SetSpriteName(DUOLCommon::StringHelper::ToTString(path));
			buttonList[count]->SetDownSprite(DUOLCommon::StringHelper::ToTString(path));
		}
	}
}
