#include "DUOLClient/Manager/EnemyManager.h"
#include <rttr/registration>

#include "DUOLClient/ECS/Component/ParticleData.h"
#include "DUOLClient/ECS/Component/Enemy/Enemy.h"
#include "DUOLCommon/MetaDataType.h"
#include "DUOLGameEngine/ECS/GameObject.h"

#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"
#include "DUOLClient/ECS/Component/Enemy/EnemyData.h"
#include "DUOLClient/ECS/Component/Enemy/EnemyAttacks.h"
using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::class_<DUOLClient::EnemyManager>("EnemyManager")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	);

}

namespace DUOLClient
{
	EnemyManager* EnemyManager::_instance = nullptr;

	EnemyManager::EnemyManager(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		DUOLGameEngine::MonoBehaviourBase(owner, name), _isStart(false)
	{
		Initialize();
	}

	EnemyManager::~EnemyManager()
	{
		_instance = nullptr;

		for (auto& iter : _enemyDatas)
			delete iter;
	}

	void EnemyManager::Initialize_MonsterData()
	{
		for (auto& iter : _enemyDatas)
			delete iter;

		// ������ ���Ͱ� 3���� �ۿ� ������ �ϵ��ڵ��� ������ �� �þ�ٸ� json�̳� ������ ���� �����ϰ� �ҷ������� ����..!
		_enemyDatas.resize(static_cast<unsigned int>(EnemyCode::Count));


		// --------------------------------------------------------------------------------------
		{
			EnemyData* data = new EnemyData();

			data->_name = TEXT("EnemyNear");
			data->_behaviorTreeName = "Enemy_MainTree";

			data->_damage = 10.0f;
			data->_maxHp = 100.0f;
			data->_attackRange = 3.0f;
			data->_patrolOffset = 13.0f;
			data->_lookRange = 20.0f;
			data->_maxSpeed = 3.5f;

			data->_animControllerName = TEXT("Monster_AnimatorController");
			data->_capsuleCenter = DUOLMath::Vector3(0, 1.0f, 0);
			data->_navBaseOffset = DUOLMath::Vector3(0, -0.3f, 0);
			data->_height = 1.0f;

			data->_attackFuncs.insert({ TEXT("Attack_Close"), Attack_Close });

			_enemyDatas[static_cast<unsigned int>(EnemyCode::Near)] = data;
		}
		// ---------------------------------------------------------------------------------------
		// --------------------------------------------------------------------------------------
		{
			EnemyData* data = new EnemyData();

			data->_name = TEXT("EnemyFar");
			data->_behaviorTreeName = "Enemy_MainTree";

			data->_damage = 10.0f;
			data->_maxHp = 100.0f;
			data->_attackRange = 13.0f;
			data->_patrolOffset = 15.0f;
			data->_lookRange = 20.0f;
			data->_maxSpeed = 3.0f;

			data->_animControllerName = TEXT("Monster_AnimatorController_Far");
			data->_capsuleCenter = DUOLMath::Vector3(0, 1.0f, 0);
			data->_navBaseOffset = DUOLMath::Vector3(0, -0.3f, 0);
			data->_height = 1.0f;

			data->_attackFuncs.insert({ TEXT("Attack_Far"), Attack_Far });

			_enemyDatas[static_cast<unsigned int>(EnemyCode::Far)] = data;
		}
		// ---------------------------------------------------------------------------------------
	}

	void EnemyManager::Initialize_ObjectQueue()
	{

	}

	void EnemyManager::Initialize()
	{

		Initialize_MonsterData();

	}


	EnemyManager* EnemyManager::GetInstance()
	{
		if (_instance)
			return _instance;
		else
		{
			_instance = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->CreateEmpty()->AddComponent<EnemyManager>();
			return _instance;
		}
	}

	EnemyData* EnemyManager::GetEnemy(EnemyCode enemyCode)
	{
		return _enemyDatas[static_cast<unsigned int>(enemyCode)];
	}

	void EnemyManager::OnStart()
	{
		if (!_instance)
			_instance = this;
		else
			Destroy(this);
	}

	void EnemyManager::OnUpdate(float deltaTime)
	{
		if (!_isStart)
		{
			_isStart = true;
			GetGameObject()->SetName(TEXT("EnemyManager"));

		}
	}
}
