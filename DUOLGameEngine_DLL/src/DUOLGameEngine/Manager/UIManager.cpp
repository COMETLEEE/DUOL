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
#include "DUOLGameEngine/Manager/GraphicsManager.h"

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

	float UIManager::GetScrollButtonData()
	{
		if (_scrollGauge == -1)
			return -1.f;

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
}
