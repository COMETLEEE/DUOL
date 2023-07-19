#pragma once
#include <map>
#include <vector>

#include "DUOLClient/Export_Client.h"
#include "DUOLCommon/StringHelper.h"

namespace DUOLGameEngine
{
	class GameObject;
}

namespace DUOLClient
{
	class SystemManager;
	class ScriptInfo;
	enum class GameScene;


}

namespace DUOLClient
{
	class DUOL_CLIENT_API InteractiveScript final
	{
	public:
		InteractiveScript();

		~InteractiveScript();


	public:
		void OnStart(const DUOLCommon::tstring& currentSceneName);

		void InitializeMiddle();

		void InitializeStageTotal();

		void InitializeStageA();

		void InitializeStageB();

		void InitializeStageC();

		void InitializeStage();

		bool Show();

		bool ShowTotalSceneScript();

		void ShowMiddleSceneScript();

		//void ShowASceneScript();

		void ShowBSceneScript();


		bool ShowInfo();


		void Play(float deltaTime);

		void Stop();

		void Next();

		void ScriptPlay(float deltaTime);

		void InfoPlay(float deltaTime);


		void SetPlayScriptKey(DUOLCommon::tstring key);

		void SetPlayInfoKey(DUOLCommon::tstring key);

		DUOLCommon::tstring& GetNowScriptKey() { return _playScriptKey; }

		DUOLCommon::tstring& GetNowInfoKey() { return _playInfoKey; }


	private:
		DUOLGameEngine::GameObject* _scriptObject;

		DUOLGameEngine::GameObject* _infoObject;

		DUOLClient::SystemManager* _systemManager;

		std::map<DUOLCommon::tstring, ScriptInfo*> _scriptList;

		DUOLCommon::tstring _playScriptKey;

		DUOLCommon::tstring _playScriptNextKey;

		DUOLCommon::tstring _playInfoKey;

		DUOLCommon::tstring _playInfoNextKey;

		float _currentScriptTime;

		float _currentInfoTime;

	};

}