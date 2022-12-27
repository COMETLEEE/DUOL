#pragma once
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"

namespace DUOLEditor
{
	/**
	 * \brief 물리 현상 시연용 모델 슈터
	 */
	class ModelShooter final : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		ModelShooter(const std::weak_ptr<DUOLGameEngine::GameObject>& owner);

		virtual ~ModelShooter() override;

	public:
		bool _isShooted;

		std::vector<DUOLGameEngine::Rigidbody*> _rigids;

	public:
		virtual void OnStart() override;

		virtual void OnUpdate(float deltaTime) override;
	};
}
