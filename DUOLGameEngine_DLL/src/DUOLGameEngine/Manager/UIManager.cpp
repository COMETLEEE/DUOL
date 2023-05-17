#include "DUOLGameEngine/Manager/UIManager.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Canvas.h"
#include "DUOLGameEngine/ECS/Component/Image.h"
#include "DUOLGameEngine/ECS/Component/Text.h"
#include <filesystem>

#include "DUOLGameEngine/ECS/Component/Button.h"
#include "DUOLGameEngine/Manager/ResourceManager.h"
#include "DUOLGameEngine/ECS/Component/Camera.h"
#include "DUOLGameEngine/Manager/ButtonEventManager.h"

namespace  DUOLGameEngine
{
	class Canvas;

	GameObject* DUOLGameEngine::UIManager::_canvas = nullptr;

	DUOLGameEngine::UIManager::UIManager() :
		_isCanvas(false)
		, _imageList()
		, _buttonList()
		, _imageFileNames()
		, _fileNames()
		, _nowPickingObject(nullptr)
	{
	}

	DUOLGameEngine::UIManager::~UIManager()
	{
		ResetScene();
	}

	void DUOLGameEngine::UIManager::Initialize()
	{
		// ������ �߿���
		LoadSpriteFile();
		CreateTextureFile();

		// font�� �ҷ��ͼ� ���� ������ش�. 
		CreateFontType();
	}

	void UIManager::InitializeCurrentGameScene(const std::list<std::shared_ptr<DUOLGameEngine::GameObject>>& rootObjectsInScene)
	{
		ResetScene();
		// Image���� Canvas�� ������ �ݴϴ�. 
		for (auto& object : rootObjectsInScene)
		{
			if (object->GetName() == L"Canvas")
			{
				_canvas = object.get();
				_isCanvas = true;
				if (object->GetTransform()->GetChildGameObjects().size() != 0)
				{
					// �ڽ��� �ڽ��� �ڽ��� �ڽı��� ��ȯ�� ���ؼ� ������ ����
					for (auto childObjects : object->GetTransform()->GetChildGameObjects())
					{
						LoadComponent(*childObjects);
					}

				}
			}
		}
	}

	// ��͸� ���鼭 �ڽİ�ü���� ��� ã���ش�. 
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
		_canvas = nullptr;
		_imageList.clear();
		_buttonList.clear();
		_isCanvas = false;
	}

	void DUOLGameEngine::UIManager::CreateCanvas(GameObject* object)
	{
		_canvas = object;
	}

	void UIManager::CreateImage(Image* image)
	{
		_imageList.emplace_back(image);
	}

	// �׳� canvas �� ���ϳ��� ������ ����°ɷ� ����
	//void UIManager::RemoveCanvas(GameObject* canvas)
	//{
	//	_canvasList.erase(remove(_canvasList.begin(), _canvasList.end(), canvas), _canvasList.end());

	//	_nowPickingObject = nullptr;

	//	if (_canvasList.size() == 0)
	//		_isCanvas = false;
	//}

	void UIManager::RemoveImage(Image* image)
	{
		_imageList.erase(remove(_imageList.begin(), _imageList.end(), image), _imageList.end());

		_nowPickingObject = nullptr;
	}

	void UIManager::OnResize(int width, int height)
	{
		if (_canvas->GetComponent<Canvas>())
			_canvas->GetComponent<Canvas>()->OnResize(width, height);

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
		return _canvas;
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


	// ��� UI Texture, Sprite�� �̸� ����� ���´�. 
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

	void UIManager::MainUI(std::vector<GameObject*> object)
	{

	}

	void UIManager::DestoryButtonTargetGameObject(DUOLGameEngine::GameObject* gameobject)
	{
		// object�� �������� button������ �����ش�. 
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
