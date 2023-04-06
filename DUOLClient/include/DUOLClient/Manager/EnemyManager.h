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
#include "DUOLGameEngine/ECS/GameObject.h"
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

		std::vector<EnemyData*> _enemyDatas;

		/**
		* \brief 여러가지 타입의 오브젝트를 하나의 변수로 관라하기 위한 변수.. void* 는 <타입>의 ID값.
		*/
		std::unordered_map<void*, std::unordered_map<DUOLCommon::tstring, std::queue<DUOLGameEngine::MonoBehaviourBase*>>> _obejctQueueMap;

		/**
		* \brief 큐에 들어있는 파티클을 인스펙터에서 확인하기 위한 게임 오브젝트.
		*/
		DUOLGameEngine::GameObject* _objectQueueGameObject;


	private:
		void Initialize_MonsterData();

		void Initialize_ObjectQueue();

		void Initialize();

		DUOLGameEngine::CoroutineHandler AutoReturnObejct(void* typeID, DUOLCommon::tstring key, DUOLGameEngine::MonoBehaviourBase* object, float timer);
	public:
		static EnemyManager* GetInstance();

		EnemyData* GetEnemy(EnemyCode enemyCode);

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

		//object->GetGameObject()->SetIsActiveSelf(false);
	}

	inline void EnemyManager::PushBack(DUOLCommon::tstring objectName, void* typeId, DUOLGameEngine::MonoBehaviourBase* object)
	{
		_obejctQueueMap[typeId][objectName].push(object);

		object->GetTransform()->SetParent(_objectQueueGameObject->GetTransform());

		//object->GetGameObject()->SetIsActiveSelf(false);
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
			return nullptr;

		auto& object = T_Queue.front();

		T_Queue.pop();

		object->GetTransform()->SetParent(nullptr);

		object->GetGameObject()->SetIsActiveSelf(true);

		StartCoroutine(&EnemyManager::AutoReturnObejct, id, objectName, object, timer);

		return reinterpret_cast<T*>(object);
	}
}
