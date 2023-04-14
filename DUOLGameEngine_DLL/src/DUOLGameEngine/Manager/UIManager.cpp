#include "DUOLGameEngine/Manager/UIManager.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Canvas.h"
#include "DUOLGameEngine/ECS/Component/Image.h"
#include <filesystem>

#include "DUOLGameEngine/ECS/Component/Button.h"
#include "DUOLGameEngine/Manager/ResourceManager.h"
#include "DUOLGameEngine/ECS/Component/Camera.h"
#include "DUOLGameEngine/Manager/ButtonEventManager.h"

namespace  DUOLGameEngine
{
	class Canvas;

	DUOLGameEngine::UIManager::UIManager()
	{
	}

	DUOLGameEngine::UIManager::~UIManager()
	{
	}

	void DUOLGameEngine::UIManager::Initialize()
	{
		// 순서가 중요함
		LoadSpriteFile();
		CreateTextureFile();

	}

	void UIManager::InitializeCurrentGameScene(const std::list<std::shared_ptr<DUOLGameEngine::GameObject>>& rootObjectsInScene)
	{
		LoadScene();
		// Image들을 Canvas에 연결해 줍니다. 
		for (auto& object : rootObjectsInScene)
		{
			if (object->GetName() == L"Canvas")
			{
				_canvasList.emplace_back(object.get());
				_isCanvas = true;
				if (object->GetTransform()->GetChildGameObjects().size() != 0)
				{
					for(auto childObjects:object->GetTransform()->GetChildGameObjects())
					{
						auto image = childObjects->GetComponent<Image>();
						auto button = childObjects->GetComponent<Button>();

						if(image)
						{
							_imageList.emplace_back(image);

							image->LoadScene();
						}
						if(button)
						{
							_buttonList.emplace_back(button);

							button->SetLoadSceneImage(image);
						}
					}
				}
			}
		}

		LoadSpriteImage();
	}

	void DUOLGameEngine::UIManager::UnInitialize()
	{
	}

	void DUOLGameEngine::UIManager::Update(float deltaTime)
	{
	}

	void UIManager::LoadScene()
	{
		_canvasList.clear();
		_imageList.clear();
		_buttonList.clear();
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

	// sprite Textur에 name을 넣어준다. 
	void UIManager::LoadSpriteImage()
	{
		for(auto image: _imageList)
		{
			image->LoadTexture(image->GetSprite()->GetName());
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

	/**
	 * \brief 각 씬에 UI를 관리하고 세팅하는 함수
	 * 각 씬이 넘어갈때는 에디터에서는 컴포넌트가 알아서 삭제되고 가져온다.
	 * 엔진에서 사용할 클래스
	 * \param uieventid
	 * \return
	 */
	  void UIManager::UIEventStatus(UIEVENTEnum uieventid,DUOLCommon::tstring& name)
	 {
	 	switch (uieventid)
	 	{
	 	case UIEVENTEnum::LoadScene:
	 		{
				// Event는 LoadScene을 한다.
			ButtonEventManager::GetInstance()->LoadScene(name);
	 		break;
	 		}
	 	case UIEVENTEnum::LoadImage:
	 		{
				// Event는 panel을 열거나 이미지등을 열거나 바꾼다. 
	 		break;
	 		}
	 	}
	 }
}
