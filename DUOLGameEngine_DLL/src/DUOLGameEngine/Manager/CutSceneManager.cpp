#include "DUOLGameEngine/Manager/CutSceneManager.h"

#include "DUOLGameEngine/ECS/Component/Image.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/FadeInOut.h"
#include "DUOLGameEngine/Manager/ButtonEventManager.h"
#include "DUOLGameEngine/Manager/InputManager.h"
#include "DUOLGameEngine/Manager/UIManager.h"
#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"

namespace DUOLGameEngine
{
	CutSceneManager::CutSceneManager() :
		_isStart(false)
		, _isEnd(false)
		, _currentTime(0.f)
		, _nowCutCount(0)
		, _nowChildCutCount(0)
		, _cutImageName("cut")
	{
	}

	CutSceneManager::~CutSceneManager()
	{
	}

	void CutSceneManager::Initialize()
	{
		_isStart = false;

		_isEnd = false;

		_currentTime = 0.f;

		_nowCutCount = 0;

		_nowChildCutCount = 0;

		_nowCutObject = nullptr;

		_nowChildCutScene.clear();

		/*	auto& gameObjects = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

			for (auto gameObject : gameObjects)
			{
				if (gameObject->GetTag() == TEXT("Fade"))
				{
					_fadeInOut = gameObject->GetComponent<DUOLGameEngine::FadeInOut>();
				}
			}*/
	}

	void CutSceneManager::UnInitialize()
	{


	}

	void CutSceneManager::Update(float deltaTime)
	{

		if (_isStart)
		{
			if (DUOLGameEngine::InputManager::GetInstance()->GetKeyDown(DUOLGameEngine::KeyCode::Space))
			{
				_currentTime += 10.f;
			}

			PlayCutScene(deltaTime);

			if (_isEnd)
			{
				_isStart = false;
				LoadScene();
			}
		}

		if (DUOLGameEngine::InputManager::GetInstance()->GetKeyDown(DUOLGameEngine::KeyCode::F9))
		{
			_isStart = !_isStart;
			if (_isStart)
				StartCutScene();
		}
	}

	void CutSceneManager::LoadScene()
	{
		_isStart = false;
		std::string path = "TotalScene";
		DUOLCommon::tstring tPath = DUOLCommon::StringHelper::ToTString(path);
		ButtonEventManager::GetInstance()->LoadScene(tPath);
	}

	void CutSceneManager::LoadUnityScene()
	{
		std::string path = "FullLoadScene";
		DUOLCommon::tstring tPath = DUOLCommon::StringHelper::ToTString(path);
		ButtonEventManager::GetInstance()->LoadUnityScene(tPath);
	}

	void CutSceneManager::StartCutScene()
	{

		Initialize();

		if (_fadeInOut)
			_fadeInOut->StartFadeIn(2, nullptr);

		_isStart = true;

		std::string path = _cutImageName + std::to_string(_nowCutCount) + std::to_string(_nowChildCutCount);

		auto firstImage = DUOLGameEngine::UIManager::GetInstance()->FindImage(path);

		if (firstImage == nullptr)
			DUOL_INFO(DUOL_CONSOLE, "CUTSCENE PARENTOBJECT NULLPTR / THIS current count is {}", _nowCutCount);

		auto parentObject = firstImage->GetGameObject();

		_nowCutObject = parentObject->GetComponent<Transform>()->GetParent()->GetGameObject();

		_nowCutObject->SetIsActiveSelf(true);

		auto childObject = _nowCutObject->GetComponent<Transform>()->GetAllChildGameObjects();

		for (auto child : childObject)
		{
			_nowChildCutScene.emplace_back(child);
		}

		if (!_nowChildCutScene.empty())
			_nowChildCutScene[_nowChildCutCount]->SetIsActiveSelf(true);
	}

	void CutSceneManager::PlayCutScene(float deltaTime)
	{
		if (_isEnd)
			return;

		_currentTime += deltaTime;

		if (5.0f <= _currentTime)
		{

			if (_nowChildCutScene[_nowChildCutCount]->GetComponent<Image>() == nullptr)
				return;

			// pre active is false
			_nowChildCutScene[_nowChildCutCount]->SetIsActiveSelf(false);

			_nowChildCutCount++;

			if (_nowChildCutScene.size() <= _nowChildCutCount)
			{
				if (!ChangeCutImage())
					return;
			}

			// next active is true
			_nowChildCutScene[_nowChildCutCount]->SetIsActiveSelf(true);

			_currentTime = 0.f;
		}
	}

	bool CutSceneManager::ChangeCutImage()
	{
		_nowCutCount++;
		_nowChildCutCount = 0;
		_nowChildCutScene.clear();

		_nowCutObject->SetIsActiveSelf(false);

		std::string path = _cutImageName + std::to_string(_nowCutCount) + std::to_string(_nowChildCutCount);

		auto firstImage = DUOLGameEngine::UIManager::GetInstance()->FindImage(path);

		if (firstImage == nullptr)
		{
			_isEnd = true;
			return false;
		}

		auto parentObject = firstImage->GetGameObject();

		_nowCutObject = parentObject->GetComponent<Transform>()->GetParent()->GetGameObject();

		_nowCutObject->SetIsActiveSelf(true);

		auto childObject = _nowCutObject->GetComponent<Transform>()->GetAllChildGameObjects();

		for (auto child : childObject)
		{
			_nowChildCutScene.emplace_back(child);
		}

		if (!_nowChildCutScene.empty())
			_nowChildCutScene[_nowChildCutCount]->SetIsActiveSelf(true);

		return true;
	}

	void CutSceneManager::SetStart(bool value)
	{
		_isStart = value;
	}
}
