#include "DUOLClient/ECS/Component/Map/Portal.h"

#include "DUOLPhysics/Util/PhysicsDataStructure.h"

#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/ParticleRenderer.h"

#include "DUOLClient/Manager/ParticleManager.h"

#include <rttr/registration>

#include "DUOLClient/Manager/GameManager.h"
#include "DUOLCommon/MetaDataType.h"

using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::class_<DUOLClient::Portal>("Portal")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.property("_nextSceneName", &DUOLClient::Portal::_nextSceneName)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::String)
	);
}

namespace DUOLClient
{
	Portal::Portal(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		MonoBehaviourBase(owner, name)
		, _nextSceneName(TEXT(""))
	{

	}

	Portal::~Portal()
	{

	}

	void Portal::OnStart()
	{
		auto portal = DUOLClient::ParticleManager::GetInstance()->Pop(ParticleEnum::OverdriveEnter);

		portal->GetTransform()->SetPosition(GetTransform()->GetWorldPosition(), DUOLGameEngine::Space::World);
	}

	void Portal::OnTriggerEnter(const std::shared_ptr<DUOLPhysics::Trigger>& trigger)
	{
		DUOLGameEngine::GameObject* gameObject = reinterpret_cast<DUOLGameEngine::GameObject*>(trigger->_other);

		if (gameObject->GetTag() == TEXT("Player"))
		{
			GameMessage<DUOLCommon::tstring> mes = { GameMessageType::SCENE_CHANGE, TEXT("Asset/Scene/") + _nextSceneName + TEXT(".dscene") };

			DUOLClient::GameManager::GetInstance()->PushGameMessage(std::move(mes));
		}
	}
}