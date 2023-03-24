#include "DUOLGameEngine/Manager/UIManager.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Canvas.h"
#include "DUOLGameEngine/ECS/Component/Image.h"
#include <filesystem>

#include "DUOLGameEngine/Manager/ResourceManager.h"
#include "DUOLGameEngine/ECS/Component/Camera.h"

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

	void DUOLGameEngine::UIManager::UnInitialize()
	{
	}

	void DUOLGameEngine::UIManager::Update(float deltaTime)
	{
	}

	void UIManager::LoadScene()
	{
		_canvasList.clear();
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

		if (_canvasList.size() == 0)
			_isCanvas = false;
	}

	void UIManager::RemoveImage(Image* image)
	{
		_imageList.erase(remove(_imageList.begin(), _imageList.end(), image), _imageList.end());

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
			else
				return nullptr;
		}
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


	// 모든 UI Texture를 미르 만들어 놓는다. 
	void UIManager::CreateTextureFile()
	{
		for (auto filename : _imageFileNames)
		{
			std::string path = "UI/" + DUOLCommon::StringHelper::ToString(filename);

			auto texture = DUOLGameEngine::ResourceManager::GetInstance()->CreateTexture(DUOLCommon::StringHelper::ToTString(path));
		}
	}

	/**
	 * \brief 각 씬에 UI를 관리하고 세팅하는 함수
	 * 각 씬이 넘어갈때는 에디터에서는 컴포넌트가 알아서 삭제되고 가져온다.
	 * 엔진에서 사용할 클래스
	 * \param uieventid 
	 * \return 
	 */
	// UIEVENTEnum UIManager::UIEventStatus(UIEVENTEnum uieventid)
	//{
	//	switch (uieventid)
	//	{
	//	case UIEVENTEnum::MAIN:
	//		{
	//		break;
	//		}
	//	case UIEVENTEnum::INGAME:
	//		{
	//		break;
	//		}
	//	case UIEVENTEnum::ENDDING:
	//		{
	//		break;
	//		}
	//	case UIEVENTEnum::OPTION:
	//		{
	//		break;
	//		}
	//	}
	//	
	//}
}
