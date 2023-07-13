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
			_currentGameScene = GameScene::Middle;
		else if (currentSceneName == TEXT("StageA"))
			_currentGameScene = GameScene::StageA;
		else if (currentSceneName == TEXT("StageB"))
			_currentGameScene = GameScene::StageB;
		else if (currentSceneName == TEXT("StageC"))
			_currentGameScene = GameScene::StageC;
	}

	void MiniMap::SetPlayerPos()
	{
		if (_playerPosRect == nullptr)
			return;

		auto parent = _miniMapImage->GetImageRectTransform();

		float width = parent->GetWidth();
		float height = parent->GetHeight();

		// {0,0}
		float playerPosX = parent->GetPosX() - ((width / 2) * parent->GetScale().x);
		float playerPosY = parent->GetPosY() - ((height / 2) * parent->GetScale().y);

		_playerPosInMiniMap = DUOLMath::Vector2{ playerPosX ,playerPosY };
	}

	void MiniMap::OnUpdate(float deltaTime)
	{
		switch (_currentGameScene)
		{
		case GameScene::Middle:
		{
			MiddleUpdate();
			break;
		}
		case GameScene::StageA:
		{
			break;
		}
		case GameScene::StageB:
		{
			break;
		}
		case GameScene::StageC:
		{
			break;
		}
		}

	}

	void MiniMap::MiddleUpdate()
	{
		auto parent = _miniMapImage->GetImageRectTransform();

		float width = parent->GetWidth();
		float height = parent->GetHeight();

		// now Player Pos
		auto nowPlayerPos = _player->GetTransform()->GetLocalPosition();

		float fianlX = ((width * parent->GetScale().x) / MIDDLESCENEX) * (nowPlayerPos.x - MIDDLEXGAP);
		float fianlY = ((height * parent->GetScale().y) / MIDDLESCENEY) * (nowPlayerPos.z - MIDDLEYGAP);

		_playerPosRect->SetRectX(_playerPosInMiniMap.x + fianlX + (_playerPosRect->GetWidth() / 2));
		_playerPosRect->SetRectY(_playerPosInMiniMap.y + fianlY + (_playerPosRect->GetHeight() / 2));
	}

	void MiniMap::SceneAUpdate()
	{
	}

	void MiniMap::SceneBUpdate()
	{
	}

	void MiniMap::SceneCUpdate()
	{
	}


}
