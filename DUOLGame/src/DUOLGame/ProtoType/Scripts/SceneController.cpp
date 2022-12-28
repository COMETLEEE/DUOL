#include "DUOLGame/ProtoType/Scripts/SceneController.h"

#include "DUOLGameEngine/Manager/InputManager.h"

namespace DUOLGame
{
	SceneController::SceneController(const std::weak_ptr<GameObject>& owner) :
		MonoBehaviourBase(owner)
		, _player(nullptr)
		, _isPlayerControl(true)
	{

	}

	SceneController::~SceneController()
	{

	}

	void SceneController::OnUpdate(float deltaTime)
	{
		if (InputManager::GetInstance()->GetKeyDown(KeyCode::Tab) == true)
		{
			_isPlayerControl = !_isPlayerControl;
			_player->SetPlayerActive(_isPlayerControl);
		}
	}

	void SceneController::SetPlayer(PlayerCharacter* player)
	{
		_player = player;
	}
}