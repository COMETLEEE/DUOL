#pragma once
#include "DUOLClient/Export_Client.h"
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"

namespace DUOLClient
{
	/**
	 * \brief 물리 현상 시연용 모델 슈터
	 */
	class DUOL_CLIENT_API ModelShooter final : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		ModelShooter(DUOLGameEngine::GameObject* owner);

		virtual ~ModelShooter() override;

	public:
		bool _isShooted;

		std::vector<DUOLGameEngine::Rigidbody*> _rigids;

	public:
		virtual void OnStart() override;

		virtual void OnUpdate(float deltaTime) override;
	};
}
