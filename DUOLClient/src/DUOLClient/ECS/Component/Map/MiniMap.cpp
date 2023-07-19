#include "DUOLClient/ECS/Component/Map/MiniMap.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"
#include "DUOLGameEngine/ECS/Component/Image.h"
#include "DUOLClient/Player/Player.h"
#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"

#include <rttr/registration>

#include "DUOLClient/Manager/GameManager.h"

using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::class_<DUOLClient::MiniMap>("MiniMap")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	);
}

namespace DUOLClient
{
	MiniMap::MiniMap(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		MonoBehaviourBase(owner, name)
	{
	}

	MiniMap::~MiniMap()
	{
	}

	void MiniMap::OnStart()
	{
		auto& gameObjects = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

		for (auto gameObject : gameObjects)
		{
			if (gameObject->GetName() == TEXT("Minimap"))
			{
				gameObject->SetIsActiveSelf(true);
				_miniMapImage = gameObject->GetComponent<DUOLGameEngine::Image>();
				_miniMapImageRect = gameObject->GetComponent<DUOLGameEngine::RectTransform>();
			}
			if (gameObject->GetName() == TEXT("PlayerMinimap"))
			{
				_playerPosRect = gameObject->GetComponent<DUOLGameEngine::RectTransform>();
			}
			if (gameObject->GetTag() == TEXT("Player"))
			{
				_player = gameObject->GetComponent<Player>();
			}
		}

		if (_miniMapImage == nullptr)
			DUOL_CRITICAL("NO MiniMap");

		DUOLGameEngine::Scene* currentScene = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene();

		const DUOLCommon::tstring& currentSceneName = currentScene->GetName();

		std::string minimap = DUOLCommon::StringHelper::ToString(currentSceneName) + "MiniMap.png";

		_miniMapImage->LoadTexture(DUOLCommon::StringHelper::ToTString(minimap));

		SetPlayerPos();

		if (currentSceneName == TEXT("Middle"))
		{
			_currentGameScene = GameScene::Middle;
			_stageSize.x = MIDDLESCENEX;
			_stageSize.y = MIDDLESCENEY;

			_playerBottomLeft.x = MIDDLEXGAP;
			_playerBottomLeft.y = MIDDLEYGAP;
		}
		else if (currentSceneName == TEXT("StageA"))
		{
			_currentGameScene = GameScene::StageA;
			_stageSize.x = MIDDLESCENEX;
			_stageSize.y = MIDDLESCENEY;

			_playerBottomLeft.x = MIDDLEXGAP;
			_playerBottomLeft.y = MIDDLEYGAP;
		}
		else if (currentSceneName == TEXT("StageB"))
		{
			_currentGameScene = GameScene::StageB;
			_stageSize.x = StageBSCENEX;
			_stageSize.y = StageBSCENEY;

			_playerBottomLeft.x = StageBXGAP;
			_playerBottomLeft.y = StageBYGAP;
		}
		else if (currentSceneName == TEXT("StageC"))
		{
			_currentGameScene = GameScene::StageC;
			_stageSize.x = MIDDLESCENEX;
			_stageSize.y = MIDDLESCENEY;

			_playerBottomLeft.x = MIDDLEXGAP;
			_playerBottomLeft.y = MIDDLEYGAP;
		}
	}

	void MiniMap::SetPlayerPos()
	{
		if (_playerPosRect == nullptr)
			return;

		float width = _miniMapImageRect->GetWidth();
		float height = _miniMapImageRect->GetHeight();

		// {0,0}

		float playerPosX = _miniMapImageRect->GetPosX() - ((width / 2) * _miniMapImageRect->GetScale().x);
		float playerPosY = _miniMapImageRect->GetPosY() - ((height / 2) * _miniMapImageRect->GetScale().y);

		_playerPosInMiniMap = DUOLMath::Vector2{ playerPosX ,playerPosY };
	}

	void MiniMap::OnUpdate(float deltaTime)
	{
		if (_miniMapImageRect == nullptr || _playerPosRect == nullptr)
			return;

		SetPlayerPos();

		// now Player Pos
		auto nowPlayertransform = _player->GetGameObject()->GetTransform();

		auto nowPlayerPos = nowPlayertransform->GetLocalPosition();

		float fianlX = (_miniMapImageRect->GetWidth() * _miniMapImageRect->GetScale().x) * ((nowPlayerPos.x - _playerBottomLeft.x) / _stageSize.x);
		float fianlY = (_miniMapImageRect->GetHeight() * _miniMapImageRect->GetScale().y) * ((nowPlayerPos.z - _playerBottomLeft.y) / _stageSize.y);

		_playerPosRect->SetRectX(_playerPosInMiniMap.x + fianlX + (_playerPosRect->GetWidth() / 2));
		_playerPosRect->SetRectY(_playerPosInMiniMap.y + fianlY + (_playerPosRect->GetHeight() / 2));
	}

}
