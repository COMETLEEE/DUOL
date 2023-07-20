#include "DUOLGameEngine/Manager/CutSceneManager.h"

#include "DUOLGameEngine/ECS/Component/Image.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/FadeInOut.h"
#include "DUOLGameEngine/Manager/ButtonEventManager.h"
#include "DUOLGameEngine/Manager/InputManager.h"
#include "DUOLGameEngine/Manager/UIManager.h"
#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"
#include "DUOLGameEngine/Manager/SoundManager.h"
#include "DUOLGameEngine/ECS/Component/AudioSource.h"
#include "DUOLGameEngine/ECS/Component/AudioListener.h"

namespace DUOLGameEngine
{
	CutSceneManager::CutSceneManager() :
		_isStart(false)
		, _isEnd(false)
		, _currentTime(0.f)
		, _nowCutCount(0)
		, _cutImageName("cut")
		, _skipImageTime(0.f)
		, _skilFadeMode(FadeInOutMode::DONE)
		, _soundIndex(0)
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

		_soundManager = DUOLGameEngine::SoundManager::GetInstance();

		_totalSceneClips.emplace_back(_soundManager->GetAudioClip(TEXT("Intro_CutScene1")));
		_totalSceneClips.emplace_back(_soundManager->GetAudioClip(TEXT("Intro_CutScene2")));
		_totalSceneClips.emplace_back(_soundManager->GetAudioClip(TEXT("Intro_CutScene3")));
		_totalSceneClips.emplace_back(_soundManager->GetAudioClip(TEXT("Intro_CutScene4")));

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
				_currentTime += 27.f;
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
		_skipFade->StartFadeOut(1, [this]()
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

		GameObject* parent = nullptr;

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
			if (gameObject->GetName() == TEXT("Cut"))
			{
				parent = gameObject;
				gameObject->SetIsActiveSelf(true);
			}
			if (gameObject->GetName() == TEXT("UIAudioSource"))
			{
				_audioSource = gameObject->GetComponent<DUOLGameEngine::AudioSource>();
				_audioListener = gameObject->GetComponent<DUOLGameEngine::AudioListener>();
			}
		}

		if (_audioSource == nullptr && _audioListener == nullptr)
		{
			auto object = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->CreateEmpty();
			object->SetName(TEXT("UIAudioSource"));
			auto comp = object->AddComponent<DUOLGameEngine::AudioSource>();
			auto listener = object->AddComponent<DUOLGameEngine::AudioListener>();
		}

		if (_fadeInOut)
			_fadeInOut->StartFadeIn(2, nullptr);

		_isStart = true;

		if (parent != nullptr)
		{
			for (auto child : parent->GetTransform()->GetChildGameObjects())
			{
				child->SetIsActiveSelf(true);
				_cutList.emplace_back(child);
			}
		}

		if (_cutList.size() < _nowCutCount)
			return;


		for (int i = 0; i < _cutList.size(); i++)
		{
			_cutList[i]->GetComponent<Image>()->SetLayer(_cutList.size()-i);
		}


		//if (_nowCutCount + 1 < _cutList.size())
		//{
		//	_cutList[_nowCutCount+1]->SetIsActiveSelf(false);
		//	_cutList[_nowCutCount+1]->GetComponent<Image>()->SetLayer(1);
		//}

		PlayCutSound();
		_soundIndex++;
	}

	void CutSceneManager::PlayCutScene(float deltaTime)
	{
		if (_isEnd)
			return;

		_currentTime += deltaTime;

		if (27.0f <= _currentTime)
		{

			if (_cutList[_nowCutCount]->GetComponent<Image>() == nullptr)
				return;

			CutSetting();

			PlayCutSound();

			_currentTime = 0.f;
		}
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

	void CutSceneManager::CutSetting()
	{
		if (_nowCutCount + 1 < _cutList.size())
		{
			_cutList[_nowCutCount]->SetIsActiveSelf(false);
			/*_cutList[_nowCutCount + 1]->SetIsActiveSelf(true);
			_cutList[_nowCutCount + 1]->GetComponent<Image>()->SetLayer(1);
			_cutList[_nowCutCount]->SetIsActiveSelf(false);
			_cutList[_nowCutCount + 1]->GetComponent<Image>()->SetLayer(2);*/
		}
		else
		{
			// end
			_isEnd = true;
			return;
		}

		_nowCutCount++;
		_soundIndex++;
	}

	void CutSceneManager::PlayCutSound()
	{
		if (_cutList.size() <= _soundIndex)
			return;
		_audioSource->SetAudioClip(_totalSceneClips[_soundIndex]);
		_audioSource->SetIsLoop(false);
		_audioSource->Play();
	}
}
