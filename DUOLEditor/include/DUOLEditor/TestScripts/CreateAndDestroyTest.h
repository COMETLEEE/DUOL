#pragma once
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLEditor
{
	class CreateAndDestroyTest : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		CreateAndDestroyTest(DUOLGameEngine::GameObject* owner);

		virtual ~CreateAndDestroyTest() override;

	private:
		float _createTick;

		float _destroyTick;

	public:
		virtual void OnStart() override;

		virtual void OnUpdate(float deltaTime) override;
	};
}