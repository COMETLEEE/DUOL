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
		, _skipImageTime(0.f)
		, _skilFadeMode(FadeInOutMode::DONE)
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

	}

	void CutSceneManager::UnInitialize()
	{


	}

	void CutSceneManager::Update(float deltaTime)
	{

		if (_isStart)
		{
			SkipImage();

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
		_skipFade->StartBlinkIn(1, [this]()
			{
				_skilFadeMode = FadeInOutMode::FADE_OUT;
			});
		_fadeInOut->StartFadeOut(2, [this]()
			{
				_isStart = false;
				std::string path = "TotalScene";
				DUOLCommon::tstring tPath = DUOLCommon::StringHelper::ToTString(path);
				ButtonEventManager::GetInstance()->LoadScene(tPath);

			});
	
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

		auto& gameObjects = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

		for (auto gameObject : gameObjects)
		{
			if (gameObject->GetName() == TEXT("Fade"))
			{
				_fadeInOut = gameObject->GetComponent<DUOLGameEngine::FadeInOut>();
			}
			if (gameObject->GetName() == TEXT("Skip"))
			{
				_skipFade = gameObject->GetComponent<DUOLGameEngine::FadeInOut>();
			}
		}

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

			//if(<=_nowCutCount )

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
		int preChildCount = _nowChildCutCount;
		_nowChildCutCount = 0;

		_nowCutObject->SetIsActiveSelf(false);

		std::string path = _cutImageName + std::to_string(_nowCutCount) + std::to_string(_nowChildCutCount);

		auto firstImage = DUOLGameEngine::UIManager::GetInstance()->FindImage(path);

		if (firstImage == nullptr)
		{
			_nowCutObject->SetIsActiveSelf(true);

			_nowChildCutScene[preChildCount - 1]->SetIsActiveSelf(true);
			_isEnd = true;
			return false;
		}

		_nowChildCutScene.clear();

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

	void CutSceneManager::SkipImage()
	{
		if (_skipFade == nullptr)
			return;

		if (_skipFade->GetFadeMode() == FadeInOutMode::DONE || _skipFade->GetFadeMode() == FadeInOutMode::NOT)
		{
			if (_skilFadeMode == FadeInOutMode::DONE || _skilFadeMode == FadeInOutMode::FADE_IN)
			{
				_skipFade->StartBlinkIn(1, [this]()
					{
						_skilFadeMode = FadeInOutMode::FADE_OUT;
					});
			}
			else if (_skilFadeMode == FadeInOutMode::FADE_OUT)
			{
				_skipFade->StartBlinkOut(1, [this]()
					{
						_skilFadeMode = FadeInOutMode::FADE_IN;
					});
			}
		}
	}
}
