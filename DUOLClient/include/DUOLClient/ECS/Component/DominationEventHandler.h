#pragma once
#include "DUOLClient/Export_Client.h"
#include "DUOLClient/ECS/Component/Contents/DominationArea.h"
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLGameEngine
{
	class Text;
}

namespace DUOLClient
{
	class EnemyGroupController;

	class DominationEventHandler : public DUOLGameEngine::BehaviourBase
	{
	public:
		DominationEventHandler(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("DominationEventHandler"));

		virtual ~DominationEventHandler() override;

	public:
		void OnAwake() override;

		void OnUpdate(float deltaf) override;

		std::vector<DUOLClient::DominationArea*> _dominationAreas;

		std::vector<DUOLGameEngine::Text*> _dominationAreaPercent;

		bool _isDominateAllArea;

		RTTR_ENABLE(DUOLGameEngine::BehaviourBase)

		RTTR_REGISTRATION_FRIEND
	};

}
