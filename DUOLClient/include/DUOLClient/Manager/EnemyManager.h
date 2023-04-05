/**

	@file      EnemyManager.h
	@brief     EnemyManager, 몬스터의 데이터와 할당 및 해제를 담당할 클래스.
	@details   ~
	@author    SinSeongHyeon
	@date      3.04.2023
	@copyright © SinSeongHyeon, 2023. All right reserved.

**/

#pragma once

#include "DUOLClient/Export_Client.h"
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLClient
{
	class Enemy;
	enum class EnemyCode;
	struct EnemyData;

	class DUOL_CLIENT_API EnemyManager : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		EnemyManager(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("ParticleManager"));

		virtual ~EnemyManager() override;

	private:
		static EnemyManager* _instance; // Singleton을 직접 만드는 이유는 컴포넌트이기 때문에 생성자를 커스텀하기 위함.

		bool _isStart;

		void Initialize();

		std::vector<EnemyData*> _enemyDatas;

	public:
		static EnemyManager* GetInstance();

		EnemyData* GetEnemy(EnemyCode enemyCode);
	public:
		virtual void OnStart() override;

		virtual void OnUpdate(float deltaTime) override;

		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)
			RTTR_REGISTRATION_FRIEND
	};
}
