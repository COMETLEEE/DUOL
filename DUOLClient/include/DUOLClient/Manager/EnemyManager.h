/**

	@file      EnemyManager.h
	@brief     EnemyManager, 몬스터의 데이터와 할당 및 해제를 담당할 클래스.
	@details   ~ 몬스터를 생성할 때 일정 갯수만 Awake할 때 생성하고 이후는 게임 플레이 도중 조금씩 할당한다. 우선 한프레임에 하나씩..!
	@author    SinSeongHyeon
	@date      3.04.2023
	@copyright © SinSeongHyeon, 2023. All right reserved.

**/

#pragma once

#include "DUOLClient/Export_Client.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

namespace DUOLClient
{
	class EnemyGroupController;
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

		bool _isComplete;

		int _closeEnemyAwakeCount; // 초기 생산량.

		int _farEnemyAwakeCount; // 초기 생산량.

		int _projectileAwakeCount; // 초기 생산량.

		int _closeEnemyCount; // 목표 생산량.

		int _farEnemyCount; // 목표 생산량.

		int _projectileCount; // 목표 생산량.

		std::vector<EnemyData*> _enemyDatas;

		/**
		* \brief 여러가지 타입의 오브젝트를 하나의 변수로 관라하기 위한 변수.. void* 는 <타입>의 ID값.
		*/
		std::unordered_map<void*, std::unordered_map<DUOLCommon::tstring, std::queue<DUOLGameEngine::MonoBehaviourBase*>>> _obejctQueueMap;

		/**
		* \brief 큐에 들어있는 파티클을 인스펙터에서 확인하기 위한 게임 오브젝트.
		*/
		DUOLGameEngine::GameObject* _objectQueueGameObject;

		/**
		* \brief 플레이어를 탐색하는 경우가 많으니 미리 캐싱하자.
		*/
		DUOLGameEngine::GameObject* _playerCharacterGameObject;

		/**
		* \brief EnemyGroupController 캐싱하자..! 오브젝트의 이름과 컴포넌트를 키 값으로 저장.
		*/
		std::unordered_map<DUOLCommon::tstring, EnemyGroupController*> _enemyGroupControllers;

		/**
		* \brief Object 생성을 위한 함수를 tstring, 함수객체 페어로 저장하자...! 큐에 하나도 없을 때 생성하기 위해서...!!
		*/
		std::unordered_map<DUOLCommon::tstring, std::function<void()>> _objectCreateFuncs;
	private:
		// 자주 생성되는 오브젝트들은 생성 함수를 만들어 두고 Queue에 더 이상 적재된게 없을 때 새로 생성할 수 있도록 하자..!
		void CreateCloseEnemy();

		void CreateFarEnemy();

		void CreateProjectile();

		/**
		* \brief 오브젝트 생성 함수 등록.
		*/
		void Initialize_RegisteObejctCreateFunc();

		/**
		* \brief 몬스터 데이터 초기화 함수. 나중에 Json으로 교체 가능하도록 설계
		*/
		void Initialize_MonsterData();

		/**
		* \brief 오브젝트 큐 초기화 함수.
		*/
		void Initialize_ObjectQueue();

		/**
		* \brief EnemyManager 초기화 함수.
		*/
		void Initialize();

		DUOLGameEngine::CoroutineHandler AutoReturnObejct(void* typeID, DUOLCommon::tstring key, DUOLGameEngine::MonoBehaviourBase* object, float timer);
	public:
		static EnemyManager* GetInstance();

		EnemyData* GetEnemy(EnemyCode enemyCode);

		DUOLGameEngine::GameObject* GetPlayerCharacterGameObject();

		EnemyGroupController* GetEnemyGroupController(DUOLCommon::tstring name);
	private:
		template<class T>
		void* GetTypeID();

	public:
		template<class T>
		void PushBack(DUOLCommon::tstring objectName, T* object);

		void PushBack(DUOLCommon::tstring objectName, void* typeId, DUOLGameEngine::MonoBehaviourBase* object);

		template<class T>
		T* Pop(DUOLCommon::tstring objectName, float timer = std::numeric_limits<float>::max());
	public:
		virtual void OnAwake() override;

		virtual void OnStart() override;

		virtual void OnUpdate(float deltaTime) override;

		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)
			RTTR_REGISTRATION_FRIEND
	};

	template <class T>
	void* EnemyManager::GetTypeID()
	{
		if (!std::is_base_of_v<DUOLGameEngine::MonoBehaviourBase, T>)
			assert(false); // MonoBehavior를 상속받지 않은 클래스는 사용할 수 없습니다.

		static T* id = nullptr; // static 변수로 고유한 objectID값을 지정한다.

		return &id;
	}

	template <class T>
	void EnemyManager::PushBack(DUOLCommon::tstring objectName, T* object)
	{
		auto id = GetTypeID<T>();

		_obejctQueueMap[id][objectName].push(object);

		object->GetTransform()->SetParent(_objectQueueGameObject->GetTransform());

		object->GetGameObject()->SetIsActiveSelf(false);
	}



	inline void EnemyManager::PushBack(DUOLCommon::tstring objectName, void* typeId, DUOLGameEngine::MonoBehaviourBase* object)
	{
		_obejctQueueMap[typeId][objectName].push(object);

		object->GetTransform()->SetParent(_objectQueueGameObject->GetTransform());

		object->GetGameObject()->SetIsActiveSelf(false);
	}

	template <class T>
	T* EnemyManager::Pop(DUOLCommon::tstring objectName, float timer)
	{
		auto id = GetTypeID<T>();

		if (!_obejctQueueMap.contains(id))
			return nullptr;

		auto& objectQueue = _obejctQueueMap.at(id);

		if (!objectQueue.contains(objectName))
			return nullptr;

		auto& T_Queue = objectQueue.at(objectName);

		if (T_Queue.empty())
		{
			if (_objectCreateFuncs.contains(objectName))
				_objectCreateFuncs.at(objectName)();
			else
				return nullptr;
		}

		auto& object = T_Queue.front();

		T_Queue.pop();

		object->GetGameObject()->SetIsActiveSelf(true);

		object->GetTransform()->SetParent(nullptr);

		StartCoroutine(&EnemyManager::AutoReturnObejct, id, objectName, object, timer);

		return reinterpret_cast<T*>(object);
	}
}
