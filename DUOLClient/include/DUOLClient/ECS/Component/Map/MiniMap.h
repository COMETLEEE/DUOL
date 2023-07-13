#pragma once
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

#define MIDDLESCENEX 29.5
#define MIDDLESCENEY 33

#define MIDDLEXGAP 0.5
#define MIDDLEYGAP 7

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
		GameScene _currentGameScene;

		DUOLGameEngine::Image* _miniMapImage;

		DUOLGameEngine::RectTransform* _playerPosRect;

		Player* _player;

		DUOLMath::Vector2 _playerPosInMiniMap;

		RTTR_REGISTRATION_FRIEND

		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)
	};
	
}
