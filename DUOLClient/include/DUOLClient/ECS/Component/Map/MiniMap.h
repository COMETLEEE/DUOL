#pragma once
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

#pragma region Middle
#define MIDDLESCENEX 29.5
#define MIDDLESCENEY 36

#define MIDDLEXGAP 1.5
#define MIDDLEYGAP 6.5
#pragma endregion

#pragma region StageA
#define StageASCENEX 180
#define StageASCENEY 110

#define StageAXGAP 170
#define StageAYGAP 20
#pragma endregion

#pragma region StageB
#define StageBSCENEX 160
#define StageBSCENEY 120

#define StageBXGAP -23.435
#define StageBYGAP 58.450
#pragma endregion


namespace DUOLGameEngine
{
	class RectTransform;
	class Image;
}

namespace DUOLClient
{
	enum class GameScene;
	class Player;

	class MiniMap : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		MiniMap(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("MiniMap"));

		virtual ~MiniMap() override;

	public:
		void OnStart() override;

		void OnUpdate(float deltaTime) override;

		void SetPlayerPos();

		void MiddleUpdate();

		void SceneAUpdate();

		void SceneBUpdate();

		void SceneCUpdate();


	private:
		DUOLMath::Vector2 _stageSize;

		DUOLMath::Vector2 _playerBottomLeft;

		GameScene _currentGameScene;

		DUOLGameEngine::Image* _miniMapImage;

		DUOLGameEngine::RectTransform* _playerPosRect;

		DUOLGameEngine::RectTransform* _miniMapImageRect;

		Player* _player;

		DUOLMath::Vector2 _playerPosInMiniMap;

		RTTR_REGISTRATION_FRIEND

		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)
	};
	
}
