/**

	@file      EnemySpawnTrigger.h
	@brief     플레이어와 닿으면 스폰 위치에 몬스터를 스폰하는 트리거.
	@details   ~ 우선 부모의 컴포넌트를 탐색해 EnemyController가 있다면 가져오고 없다면 EnemyManager에 등록된 이름을 탐색해서 가져온다.
	@author    SinSeongHyeon
	@date      12.04.2023
	@copyright © SinSeongHyeon, 2023. All right reserved.

**/

#include "EnemyGroupController.h"
#include "DUOLClient/Export_Client.h"
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLClient
{
	class DUOL_CLIENT_API EnemySpawnTrigger : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		EnemySpawnTrigger(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("EnemySpawnTrigger"));

		~EnemySpawnTrigger();
	private:
		EnemyGroupController* _enemyGroupController;

		DUOLCommon::tstring _enemyGroupControllerName; // 컨트롤러 탐색을 위한 이름.

		int _firstCloseEnemyCount;

		int _firstFarEnemyCount;

		int _firstWeakEliteEnemyCount;

		int _firstEliteEnemyCount;

		int _firstBossEnemyCount;

		float _firstCreateWaitForSeconds;

		int _firstWaveCount;

		int _secondCloseEnemyCount;

		int _secondFarEnemyCount;

		int _secondWeakEliteEnemyCount;

		int _secondEliteEnemyCount;

		int _secondBossEnemyCount;

		int _secondWaveCount;

		float _secondCreateWaitForSeconds;

		DUOLMath::Vector3 _createPos;

		EnemyCreateType _enemyCreateType;
	public:
		virtual void OnStart() override;

		virtual void OnUpdate(float deltaTime) override;

		virtual void OnTriggerEnter(const std::shared_ptr<DUOLPhysics::Trigger>& trigger) override;

		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)
			RTTR_REGISTRATION_FRIEND
	};

}
