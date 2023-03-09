#pragma once
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"


namespace DUOLGame
{
	/**
	 * \brief 물리 현상 시연용 모델 슈터
	 */
	class MaterialController final : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		MaterialController(DUOLGameEngine::GameObject* owner);

		virtual ~MaterialController() override;

	public:
		DUOLGameEngine::GameObject* obj;

		float _metallic;

		float _roughness;


	public:
		virtual void OnStart() override;

		virtual void OnUpdate(float deltaTime) override;
	};
}
