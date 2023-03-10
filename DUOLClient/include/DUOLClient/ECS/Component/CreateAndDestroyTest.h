#pragma once
#include "DUOLClient/Export_Client.h"
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLClient
{
	class DUOL_CLIENT_API CreateAndDestroyTest : public DUOLGameEngine::MonoBehaviourBase
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