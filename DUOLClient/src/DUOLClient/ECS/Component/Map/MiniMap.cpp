#include "DUOLClient/ECS/Component/Map/MiniMap.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"
#include "DUOLGameEngine/ECS/Component/Image.h"

#include <rttr/registration>

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
				_playerPosImage = gameObject->GetComponent<DUOLGameEngine::Image>();
			}
		}

		if (_miniMapImage == nullptr)
			DUOL_CRITICAL("NO MiniMap");

		DUOLGameEngine::Scene* currentScene = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene();

		const DUOLCommon::tstring& currentSceneName = currentScene->GetName();

		std::string minimap = DUOLCommon::StringHelper::ToString(currentSceneName) + "MiniMap.png";

		_miniMapImage->LoadTexture(DUOLCommon::StringHelper::ToTString(minimap));

	}

}
