#include "DUOLClient/Script/InteractiveScript.h"

#include "DUOLClient/Manager/GameManager.h"
#include "DUOLClient/Manager/SystemManager.h"
#include "DUOLClient/Script/ScriptInfo.h"
#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Image.h"
#include "DUOLGameEngine/Manager/InputManager.h"

namespace DUOLClient
{

	DUOLClient::InteractiveScript::InteractiveScript()
	{
	}

	DUOLClient::InteractiveScript::~InteractiveScript()
	{
		for (auto ptr : _scriptList)
		{
			delete ptr.second;
		}

		_scriptList.clear();
	}

	void InteractiveScript::OnStart(const DUOLCommon::tstring& currentSceneName)
	{
		_scriptList.clear();

		_systemManager = DUOLClient::SystemManager::GetInstance();

		if (currentSceneName == TEXT("TotalScene"))
			InitializeStageTotal();
		else if (currentSceneName == TEXT("Middle"))
			InitializeMiddle();
		else if (currentSceneName == TEXT("StageA"))
			InitializeStageA();
		else if (currentSceneName == TEXT("StageB"))
			InitializeStageB();
		else if (currentSceneName == TEXT("StageC"))
			InitializeStageC();
		else if (currentSceneName == TEXT("StageBoss"))
			InitializeBoss();
		else
			InitializeStage();
	}

#pragma region

	void InteractiveScript::InitializeStageTotal()
	{
		auto& gameObjects = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

		for (auto gameObject : gameObjects)
		{
			if (gameObject->GetName() == TEXT("NarrativeWin"))
			{
				_scriptObject = gameObject;
				_scriptObject->SetIsActiveSelf(false);
			}
		}

		//TODO::: 이상한코드?
		_infoObject = nullptr;

		_scriptList.insert(std::make_pair(L"DialogueText_01.png", new ScriptInfo(L"DialogueText_01.png", L"DialogueText_02.png", L"NULL", 8.0f, true, false)));
		_scriptList.insert(std::make_pair(L"DialogueText_02.png", new ScriptInfo(L"DialogueText_02.png", L"DialogueText_03.png", L"NULL", 12.0f, true, false)));
		_scriptList.insert(std::make_pair(L"DialogueText_03.png", new ScriptInfo(L"DialogueText_03.png", L"NULL", L"NULL", 7.0f, true, false)));

		_playScriptNextKey = L"DialogueText_01.png";
		_playScriptKey = L"NULL";

		_playInfoKey = L"NULL";

		_playInfoNextKey = _playInfoKey;
	}

	void InteractiveScript::InitializeMiddle()
	{
		auto& gameObjects = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

		for (auto gameObject : gameObjects)
		{
			if (gameObject->GetName() == TEXT("NarrativeWin"))
			{
				_scriptObject = gameObject;
				_scriptObject->SetIsActiveSelf(false);
			}
			if (gameObject->GetName() == TEXT("TutorialWin"))
			{
				_infoObject = gameObject;
				_infoObject->SetIsActiveSelf(false);
			}
		}

		_scriptList.insert(std::make_pair(L"DialogueText_04.png", new ScriptInfo(L"DialogueText_04.png", L"DialogueText_05.png", L"Info00.png", 8.0f, true, false)));
		_scriptList.insert(std::make_pair(L"DialogueText_05.png", new ScriptInfo(L"DialogueText_05.png", L"NULL", L"NULL", 7.0f, true, false)));
		_scriptList.insert(std::make_pair(L"Info00.png", new ScriptInfo(L"Info00.png", L"NULL.png", L"Info01.png", 5.0f, true, false)));
		_scriptList.insert(std::make_pair(L"Info01.png", new ScriptInfo(L"Info01.png", L"DialogueText_06.png", L"NULL", 5.0f, true, false)));
		_scriptList.insert(std::make_pair(L"DialogueText_06.png", new ScriptInfo(L"DialogueText_06.png", L"NULL", L"Info02.png", 6.0f, true, false)));
		_scriptList.insert(std::make_pair(L"Info02.png", new ScriptInfo(L"Info02.png", L"DialogueText_11.png", L"Info03.png", 5.0f, true, false)));
		_scriptList.insert(std::make_pair(L"DialogueText_11.png", new ScriptInfo(L"DialogueText_11.png", L"DialogueText_12.png", L"Info05.png", 11.0f, true, false)));
		_scriptList.insert(std::make_pair(L"Info03.png", new ScriptInfo(L"Info03.png", L"NULL", L"NULL", 7.0f, true, false)));
		_scriptList.insert(std::make_pair(L"DialogueText_12.png", new ScriptInfo(L"DialogueText_12.png", L"NULL", L"NULL", 11.0f, true, false)));
		_scriptList.insert(std::make_pair(L"Info05.png", new ScriptInfo(L"Info05.png", L"NULL", L"NULL", 11.0f, true, false)));
		_scriptList.insert(std::make_pair(L"Info06.png", new ScriptInfo(L"Info06.png", L"DialogueText_07.png", L"NULL", 10.f, true, false)));
		_scriptList.insert(std::make_pair(L"DialogueText_07.png", new ScriptInfo(L"DialogueText_07.png", L"END", L"END", 7.0f, true, false)));

		_playScriptKey = L"DialogueText_04.png";

		_playInfoKey = L"NULL";

		_playScriptNextKey = _playScriptKey;
		_playInfoNextKey = _playInfoKey;
	}

	void InteractiveScript::InitializeStageA()
	{
		auto& gameObjects = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

		for (auto gameObject : gameObjects)
		{
			if (gameObject->GetName() == TEXT("NarrativeWin"))
			{
				_scriptObject = gameObject;
				_scriptObject->SetIsActiveSelf(false);
			}
			if (gameObject->GetName() == TEXT("TutorialWin"))
			{
				_infoObject = gameObject;
				_infoObject->SetIsActiveSelf(false);
			}
		}

		if (!_systemManager->GetFirstMonster())
		{
			_scriptList.insert(std::make_pair(L"DialogueText_08.png", new ScriptInfo(L"DialogueText_08.png", L"DialogueText_09.png", L"NULL", 9.0f, true, false)));
			_scriptList.insert(std::make_pair(L"DialogueText_09.png", new ScriptInfo(L"DialogueText_09.png", L"DialogueText_10.png", L"Info07.png", 14.0f, true, false)));
			_scriptList.insert(std::make_pair(L"DialogueText_10.png", new ScriptInfo(L"DialogueText_10.png", L"NULL", L"NULL", 11.0f, true, false)));
			_scriptList.insert(std::make_pair(L"Info07.png", new ScriptInfo(L"Info07.png", L"NULL", L"NULL", 11.0f, true, false)));
		}

		_scriptList.insert(std::make_pair(L"DialogueText_13.png", new ScriptInfo(L"DialogueText_13.png", L"NULL", L"NULL", 10.0f, true, false)));
		_scriptList.insert(std::make_pair(L"Info08.png", new ScriptInfo(L"Info08.png", L"END", L"END", 5.0f, true, false)));

		_scriptList.insert(std::make_pair(L"DialogueText_19.png", new ScriptInfo(L"DialogueText_19.png", L"NULL", L"NULL", 13.0f, true, false)));

		_playScriptKey = L"NULL";
		_playInfoKey = L"NULL";

		_playScriptNextKey = _playScriptKey;
		_playInfoNextKey = _playInfoKey;
	}

	void InteractiveScript::InitializeStageB()
	{
		auto& gameObjects = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

		for (auto gameObject : gameObjects)
		{
			if (gameObject->GetName() == TEXT("NarrativeWin"))
			{
				_scriptObject = gameObject;
				_scriptObject->SetIsActiveSelf(false);
			}
			if (gameObject->GetName() == TEXT("TutorialWin"))
			{
				_infoObject = gameObject;
				_infoObject->SetIsActiveSelf(false);
			}
		}
		if (!_systemManager->GetFirstMonster())
		{
			_scriptList.insert(std::make_pair(L"DialogueText_08.png", new ScriptInfo(L"DialogueText_08.png", L"DialogueText_09.png", L"NULL", 9.0f, true, false)));
			_scriptList.insert(std::make_pair(L"DialogueText_09.png", new ScriptInfo(L"DialogueText_09.png", L"DialogueText_10.png", L"Info07.png", 14.0f, true, false)));
			_scriptList.insert(std::make_pair(L"DialogueText_10.png", new ScriptInfo(L"DialogueText_10.png", L"NULL", L"NULL", 11.0f, true, false)));
			_scriptList.insert(std::make_pair(L"Info07.png", new ScriptInfo(L"Info07.png", L"NULL", L"NULL", 11.0f, true, false)));
		}

		_scriptList.insert(std::make_pair(L"DialogueText_14.png", new ScriptInfo(L"DialogueText_14.png", L"NULL", L"Info09.png", 8.0f, true, false)));
		_scriptList.insert(std::make_pair(L"Info09.png", new ScriptInfo(L"Info09.png", L"NULL", L"NULL", 5.0f, true, false)));
		_scriptList.insert(std::make_pair(L"DialogueText_15.png", new ScriptInfo(L"DialogueText_15.png", L"NULL", L"NULL", 9.0f, true, false)));
		_scriptList.insert(std::make_pair(L"DialogueText_15A.png", new ScriptInfo(L"DialogueText_15A.png", L"NULL", L"Info11.png", 9.0f, true, false)));
		_scriptList.insert(std::make_pair(L"Info11.png", new ScriptInfo(L"Info11.png", L"NULL", L"NULL", 5.0f, true, false)));
		_scriptList.insert(std::make_pair(L"DialogueText_16.png", new ScriptInfo(L"DialogueText_16.png", L"END", L"END", 7.0f, true, false)));


		_scriptList.insert(std::make_pair(L"DialogueText_19.png", new ScriptInfo(L"DialogueText_19.png", L"NULL", L"NULL", 13.0f, true, false)));

		_playScriptKey = L"NULL";
		_playInfoKey = L"NULL";

		_playScriptNextKey = _playScriptKey;
		_playInfoNextKey = _playInfoKey;
	}

	void InteractiveScript::InitializeStageC()
	{
		auto& gameObjects = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

		for (auto gameObject : gameObjects)
		{
			if (gameObject->GetName() == TEXT("NarrativeWin"))
			{
				_scriptObject = gameObject;
				_scriptObject->SetIsActiveSelf(false);
			}
			if (gameObject->GetName() == TEXT("TutorialWin"))
			{
				_infoObject = gameObject;
				_infoObject->SetIsActiveSelf(false);
			}
		}

		if (!_systemManager->GetFirstMonster())
		{
			_scriptList.insert(std::make_pair(L"DialogueText_08.png", new ScriptInfo(L"DialogueText_08.png", L"DialogueText_09.png", L"NULL", 9.0f, true, false)));
			_scriptList.insert(std::make_pair(L"DialogueText_09.png", new ScriptInfo(L"DialogueText_09.png", L"DialogueText_10.png", L"Info07.png", 14.0f, true, false)));
			_scriptList.insert(std::make_pair(L"DialogueText_10.png", new ScriptInfo(L"DialogueText_10.png", L"NULL", L"NULL", 11.0f, true, false)));
			_scriptList.insert(std::make_pair(L"Info07.png", new ScriptInfo(L"Info07.png", L"NULL", L"NULL", 11.0f, true, false)));
		}

		_scriptList.insert(std::make_pair(L"Info14.png", new ScriptInfo(L"Info14.png", L"NULL", L"NULL", 7.0f, true, false)));
		_scriptList.insert(std::make_pair(L"DialogueText_17.png", new ScriptInfo(L"DialogueText_17.png", L"DialogueText_18.png", L"NULL", 8.0f, true, false)));
		_scriptList.insert(std::make_pair(L"DialogueText_18.png", new ScriptInfo(L"DialogueText_18.png", L"NULL", L"Info12.png", 8.0f, true, false)));
		_scriptList.insert(std::make_pair(L"Info12.png", new ScriptInfo(L"Info12.png", L"NULL", L"NULL", 5.0f, true, false)));

		_scriptList.insert(std::make_pair(L"DialogueText_19.png", new ScriptInfo(L"DialogueText_19.png", L"NULL", L"NULL", 13.0f, true, false)));

		_playScriptKey = L"NULL";
		_playInfoKey = L"NULL";

		_playScriptNextKey = _playScriptKey;
		_playInfoNextKey = _playInfoKey;
	}

	void InteractiveScript::InitializeStage()
	{
		auto& gameObjects = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

		for (auto gameObject : gameObjects)
		{
			if (gameObject->GetName() == TEXT("NarrativeWin"))
			{
				_scriptObject = gameObject;
				_scriptObject->SetIsActiveSelf(false);
			}
			if (gameObject->GetName() == TEXT("TutorialWin"))
			{
				_infoObject = gameObject;
				_infoObject->SetIsActiveSelf(false);
			}
		}
	}

	void InteractiveScript::InitializeBoss()
	{
		auto& gameObjects = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

		for (auto gameObject : gameObjects)
		{
			if (gameObject->GetName() == TEXT("NarrativeWin"))
			{
				_scriptObject = gameObject;
				_scriptObject->SetIsActiveSelf(false);
			}
			if (gameObject->GetName() == TEXT("TutorialWin"))
			{
				_infoObject = gameObject;
				_infoObject->SetIsActiveSelf(false);
			}
		}

		_scriptList.insert(std::make_pair(L"DialogueText_20.png", new ScriptInfo(L"DialogueText_20.png", L"DialogueText_21.png", L"NULL", 16.f, true, false)));
		_scriptList.insert(std::make_pair(L"DialogueText_21.png", new ScriptInfo(L"DialogueText_21.png", L"NULL", L"NULL", 11.5f, true, false)));
		_scriptList.insert(std::make_pair(L"DialogueText_22.png", new ScriptInfo(L"DialogueText_22.png", L"NULL", L"NULL", 10.0f, true, false)));
		_scriptList.insert(std::make_pair(L"FinalBossCutscene01.png", new ScriptInfo(L"FinalBossCutscene01.png", L"NULL", L"NULL", 6.0f, true, false)));
		_scriptList.insert(std::make_pair(L"FinalBossCutscene02.png", new ScriptInfo(L"FinalBossCutscene02.png", L"NULL", L"NULL", 12.0f, true, false)));
		_scriptList.insert(std::make_pair(L"FinalBossCutscene03.png", new ScriptInfo(L"FinalBossCutscene03.png", L"NULL", L"NULL", 15.0f, true, false)));
		_scriptList.insert(std::make_pair(L"DialogueText_23.png", new ScriptInfo(L"DialogueText_23.png", L"DialogueText_24.png", L"NULL", 14.0f, true, false)));
		_scriptList.insert(std::make_pair(L"DialogueText_24.png", new ScriptInfo(L"DialogueText_24.png", L"DialogueText_25.png", L"NULL", 11.0f, true, false)));
		_scriptList.insert(std::make_pair(L"DialogueText_25.png", new ScriptInfo(L"DialogueText_25.png", L"DialogueText_26.png", L"NULL", 7.0f, true, false)));
		_scriptList.insert(std::make_pair(L"DialogueText_26.png", new ScriptInfo(L"DialogueText_26.png", L"NULL", L"NULL", 9.0f, true, false)));
		_scriptList.insert(std::make_pair(L"Info13.png", new ScriptInfo(L"Info13.png", L"NULL", L"NULL", 15.0f, true, false)));

		_scriptList.insert(std::make_pair(L"DialogueText_19.png", new ScriptInfo(L"DialogueText_19.png", L"NULL", L"NULL", 13.0f, true, false)));

		_playScriptKey = L"DialogueText_20.png";
		_playInfoKey = L"NULL";

		_playInfoNextKey = _playInfoKey;
	}


#pragma endregion Init
	bool InteractiveScript::Show()
	{
		if (_playScriptKey == L"NULL")
		{
			if (_playScriptKey != _playScriptNextKey)
				_playScriptKey = _playScriptNextKey;
			else
				return false;
		}

		if (_playScriptKey == L"END" || _playScriptKey == L"NULL")
			return false;

		if (_scriptList[_playScriptKey]->_isShow || _scriptList.empty())
			return false;

		if (_playScriptKey != _playScriptNextKey)
			_playScriptKey = _playScriptNextKey;

		_scriptList[_playScriptKey]->_isShow = true;
		_scriptObject->GetComponent<DUOLGameEngine::Image>()->LoadTexture(_scriptList[_playScriptKey]->_nowPath);
		_scriptObject->SetIsActiveSelf(true);
		_currentScriptTime = 0.f;

		return true;
	}

	bool InteractiveScript::ShowTotalSceneScript()
	{
		// 개발자용 나중에 막기
		if (DUOLGameEngine::InputManager::GetInstance()->GetKeyDown(DUOLGameEngine::KeyCode::Space))
			Next();

		if (Show())
		{
			_systemManager->PlayTotalScene(_playScriptKey);
			return true;
		}
		else
			return false;
	}

	void InteractiveScript::ShowMiddleSceneScript()
	{
		if (Show())
			_systemManager->PlayMiddleScene(_playScriptKey);

		ShowInfo();

		// all script and info finish middle event no action
		if (_playScriptKey == L"END" && _playInfoKey == L"END")
			_systemManager->SetMiddleScene();
	}

	void InteractiveScript::ShowASceneScript()
	{
		if (Show())
			_systemManager->PlayStageAScene(_playScriptKey);

		ShowInfo();
	}

	void InteractiveScript::ShowBSceneScript()
	{
		if (Show())
			_systemManager->PlayStageBScene(_playScriptKey);

		ShowInfo();
	}

	void InteractiveScript::ShowCSceneScript()
	{
		if (Show())
			_systemManager->PlayStageCScene(_playScriptKey);

		ShowInfo();
	}

	void InteractiveScript::ShowBossSceneScript()
	{
		if (Show())
			_systemManager->PlayStageBossScene(_playScriptKey);

		ShowInfo();
	}

	bool InteractiveScript::ShowInfo()
	{
		if (_playInfoKey == L"NULL")
		{
			if (_playInfoKey != _playInfoNextKey)
				_playInfoKey = _playInfoNextKey;
			else
				return false;
		}

		if (_playInfoKey == L"END" || _playInfoKey == L"NULL")
			return false;

		if (_scriptList[_playInfoKey]->_isShow || _scriptList.empty())
			return false;

		if (_playInfoKey != _playInfoNextKey)
			_playInfoKey = _playInfoNextKey;

		_scriptList[_playInfoKey]->_isShow = true;
		_infoObject->GetComponent<DUOLGameEngine::Image>()->LoadTexture(_scriptList[_playInfoKey]->_nowPath);
		_infoObject->SetIsActiveSelf(true);
		_currentInfoTime = 0.f;

		return true;
	}

	void InteractiveScript::Play(float deltaTime)
	{
		ScriptPlay(deltaTime);
		InfoPlay(deltaTime);
	}

	void InteractiveScript::ScriptPlay(float deltaTime)
	{
		if (_playScriptKey == L"NULL" || _playScriptKey == L"END")
			return;

		if (!_scriptList[_playScriptKey]->_isShow || !_scriptList[_playScriptKey]->_isActive)
			return;

		_currentScriptTime += deltaTime;

  		if (_scriptList[_playScriptKey]->_lifeTime <= _currentScriptTime)
		{
			_scriptList[_playScriptKey]->_isActive = false;
			_playScriptNextKey = _scriptList[_playScriptKey]->_nextPath[0];
			_playInfoNextKey = _scriptList[_playScriptKey]->_nextPath[1];
			_playScriptKey = _playScriptNextKey;
			_scriptObject->SetIsActiveSelf(false);

			_currentScriptTime = 0.f;
		}
	}

	void InteractiveScript::InfoPlay(float deltaTime)
	{
		if (_playInfoKey == L"NULL" || _playInfoKey == L"END")
			return;

		if (!_scriptList[_playInfoKey]->_isShow || !_scriptList[_playInfoKey]->_isActive)
			return;

		_currentInfoTime += deltaTime;

		if (_scriptList[_playInfoKey]->_lifeTime <= _currentInfoTime)
		{
			_scriptList[_playInfoKey]->_isActive = false;
			_playScriptNextKey = _scriptList[_playInfoKey]->_nextPath[0];
			_playInfoNextKey = _scriptList[_playInfoKey]->_nextPath[1];
			_playInfoKey = _playInfoNextKey;
			_infoObject->SetIsActiveSelf(false);

			_currentInfoTime = 0.f;
		}
	}

	void InteractiveScript::SetPlayScriptKey(DUOLCommon::tstring key)
	{
		if (_playScriptNextKey == key)
			return;

		_playScriptKey = key;
		_playScriptNextKey = key;
	}

	void InteractiveScript::SetPlayInfoKey(DUOLCommon::tstring key)
	{
		if (_playInfoNextKey == key)
			return;

		_playInfoKey = key;
		_playInfoNextKey = key;
	}

	void InteractiveScript::Stop()
	{
	}

	void InteractiveScript::Next()
	{
		_playInfoKey = _playInfoNextKey;
	}


}
