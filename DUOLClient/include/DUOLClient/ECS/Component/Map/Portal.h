#pragma once
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLClient
{
	class Portal : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		Portal(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("Portal"));

		virtual ~Portal() override;

	private:
		DUOLCommon::tstring _nextSceneName;

	public:
		virtual void OnStart() override;

		virtual void OnTriggerEnter(const std::shared_ptr<DUOLPhysics::Trigger>& trigger) override;

		RTTR_REGISTRATION_FRIEND

		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)
	};
}