#pragma once
#include "DUOLClient/Export_Client.h"
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLClient
{
	class DUOL_CLIENT_API OpenDoor : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		OpenDoor(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("CharacterBase"));

		virtual ~OpenDoor() override;


	public:
		void OnCollisionStay(const std::shared_ptr<DUOLPhysics::Collision>& collision) override;
		void OnCollisionExit(const std::shared_ptr<DUOLPhysics::Collision>& collision) override;

		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)

		RTTR_REGISTRATION_FRIEND
	};

}