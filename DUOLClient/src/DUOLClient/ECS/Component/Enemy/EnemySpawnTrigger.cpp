#include "DUOLClient/ECS/Component/Enemy/EnemySpawnTrigger.h"
#include "DUOLClient/ECS/Component/Enemy/EnemyGroupController.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"
#include "DUOLGameEngine/ECS/Component/BoxCollider.h"

#include <rttr/registration>

#include "DUOLClient/Manager/EnemyManager.h"
#include "DUOLCommon/MetaDataType.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Light.h"

using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::enumeration<DUOLClient::EnemyCreateType>("EnemyCreateType")
	(
		value("Normal", DUOLClient::EnemyCreateType::Normal)
		, value("Wave", DUOLClient::EnemyCreateType::Wave)
		, value("AllDied", DUOLClient::EnemyCreateType::AllDied)
	);

	rttr::registration::class_<DUOLClient::EnemySpawnTrigger>("EnemySpawnTrigger")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.property("_enemyCreateType", &DUOLClient::EnemySpawnTrigger::_enemyCreateType)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	, metadata(DUOLCommon::MetaDataType::Inspectable, true)
	, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Enumeration)
	)
	.property("_firstCloseEnemyCount", &DUOLClient::EnemySpawnTrigger::_firstCloseEnemyCount)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	, metadata(DUOLCommon::MetaDataType::Inspectable, true)
	, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Int)
	)
	.property("_firstFarEnemyCount", &DUOLClient::EnemySpawnTrigger::_firstFarEnemyCount)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	, metadata(DUOLCommon::MetaDataType::Inspectable, true)
	, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Int)
	)
	.property("_firstWeakEliteEnemyCount", &DUOLClient::EnemySpawnTrigger::_firstWeakEliteEnemyCount)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	, metadata(DUOLCommon::MetaDataType::Inspectable, true)
	, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Int)
	)
	.property("_firstEliteEnemyCount", &DUOLClient::EnemySpawnTrigger::_firstEliteEnemyCount)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	, metadata(DUOLCommon::MetaDataType::Inspectable, true)
	, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Int)
	)
	.property("_firstBossEnemyCount", &DUOLClient::EnemySpawnTrigger::_firstBossEnemyCount)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	, metadata(DUOLCommon::MetaDataType::Inspectable, true)
	, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Int)
	)
	.property("_firstCreateWaitForSeconds", &DUOLClient::EnemySpawnTrigger::_firstCreateWaitForSeconds)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	, metadata(DUOLCommon::MetaDataType::Inspectable, true)
	, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
	)
	.property("_firstWaveCount", &DUOLClient::EnemySpawnTrigger::_firstWaveCount)
	(
			metadata(DUOLCommon::MetaDataType::Serializable, true)
			, metadata(DUOLCommon::MetaDataType::Inspectable, true)
			, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Int)
	)
	.property("_secondCloseEnemyCount", &DUOLClient::EnemySpawnTrigger::_secondCloseEnemyCount)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	, metadata(DUOLCommon::MetaDataType::Inspectable, true)
	, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Int)
	)
	.property("_secondFarEnemyCount", &DUOLClient::EnemySpawnTrigger::_secondFarEnemyCount)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	, metadata(DUOLCommon::MetaDataType::Inspectable, true)
	, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Int)
	)
	.property("_secondWeakEliteEnemyCount", &DUOLClient::EnemySpawnTrigger::_secondWeakEliteEnemyCount)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	, metadata(DUOLCommon::MetaDataType::Inspectable, true)
	, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Int)
	)
	.property("_secondEliteEnemyCount", &DUOLClient::EnemySpawnTrigger::_secondEliteEnemyCount)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	, metadata(DUOLCommon::MetaDataType::Inspectable, true)
	, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Int)
	)
	.property("_secondBossEnemyCount", &DUOLClient::EnemySpawnTrigger::_secondBossEnemyCount)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	, metadata(DUOLCommon::MetaDataType::Inspectable, true)
	, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Int)
	)
	.property("_secondCreateWaitForSeconds", &DUOLClient::EnemySpawnTrigger::_secondCreateWaitForSeconds)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	, metadata(DUOLCommon::MetaDataType::Inspectable, true)
	, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
	)
	.property("_secondWaveCount", &DUOLClient::EnemySpawnTrigger::_secondWaveCount)
	(
			metadata(DUOLCommon::MetaDataType::Serializable, true)
			, metadata(DUOLCommon::MetaDataType::Inspectable, true)
			, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Int)
	)
	.property("_enemyGroupControllerName", &DUOLClient::EnemySpawnTrigger::_enemyGroupControllerName)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	, metadata(DUOLCommon::MetaDataType::Inspectable, true)
	, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::String)
	);

}

namespace DUOLClient
{
	EnemySpawnTrigger::EnemySpawnTrigger(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		DUOLGameEngine::MonoBehaviourBase(owner, name),
		_enemyGroupController(nullptr), _enemyGroupControllerName(TEXT("")),
		_firstCloseEnemyCount(0),
		_firstFarEnemyCount(0),
		_firstWeakEliteEnemyCount(0),
		_firstEliteEnemyCount(0),
		_firstCreateWaitForSeconds(0),
		_secondCloseEnemyCount(0),
		_secondFarEnemyCount(0),
		_secondWeakEliteEnemyCount(0),
		_secondEliteEnemyCount(0),
		_secondCreateWaitForSeconds(0),
		_createPos(),
		_enemyCreateType(EnemyCreateType::Normal),
		_firstWaveCount(0),
		_secondWaveCount(0)
	{
	}

	EnemySpawnTrigger::~EnemySpawnTrigger()
	{
	}

	void EnemySpawnTrigger::OnStart()
	{
		const auto collider = GetGameObject()->GetComponent<DUOLGameEngine::ColliderBase>();

		if (!collider)
			GetGameObject()->AddComponent<DUOLGameEngine::BoxCollider>();

		auto parent = GetGameObject()->GetTransform()->GetParent();

		_createPos = parent->GetTransform()->GetWorldPosition();

		// 부모 아니면 자기 자신에게 컨트롤러가 있을 것 ..?
		while (parent)
		{
			_enemyGroupController = parent->GetGameObject()->GetComponent<EnemyGroupController>();

			parent = parent->GetTransform()->GetParent();

			if (_enemyGroupController)
				break;
		}


		//if (!_enemyGroupController) // 나한테 컨트롤러가 있니? // 나한테 있으면 안됨...! // 트리거를 밟으면 트리거 오브젝트를 삭제시키기 때문에. 다른 곳에서 보관해야함.
		//	_enemyGroupController = GetGameObject()->GetComponent<EnemyGroupController>();

		if (!_enemyGroupController) // 이름으로 오브젝트를 탐색해보자..!
			_enemyGroupController = EnemyManager::GetInstance()->GetEnemyGroupController(_enemyGroupControllerName);

		_enemyGroupController->IncreaseTriggerCount();
	}

	void EnemySpawnTrigger::OnUpdate(float deltaTime)
	{
		MonoBehaviourBase::OnUpdate(deltaTime);
	}

	void EnemySpawnTrigger::OnTriggerEnter(const std::shared_ptr<DUOLPhysics::Trigger>& trigger)
	{
		auto other = static_cast<DUOLGameEngine::GameObject*>(trigger->_other);

		if (other->GetTag() == TEXT("Player"))
		{

			EnemyCreateInfo first;
			first._closeEnemyCount = _firstCloseEnemyCount;
			first._farEnemyCount = _firstFarEnemyCount;
			first._eliteEnemyCount = _firstEliteEnemyCount;
			first._weakEliteEnemyCount = _firstWeakEliteEnemyCount;
			first._bossEnemyCount = _firstBossEnemyCount;
			first._createWaitForSeconds = _firstCreateWaitForSeconds;
			first._createPos = _createPos;
			first._enemyCreateType = _enemyCreateType;
			first._waveCount = _firstWaveCount;

			EnemyCreateInfo second;
			second._closeEnemyCount = _secondCloseEnemyCount;
			second._farEnemyCount = _secondFarEnemyCount;
			second._eliteEnemyCount = _secondEliteEnemyCount;
			second._weakEliteEnemyCount = _secondWeakEliteEnemyCount;
			second._createWaitForSeconds = _secondCreateWaitForSeconds;
			second._bossEnemyCount = _secondBossEnemyCount;
			second._createPos = _createPos;
			second._enemyCreateType = _enemyCreateType;
			second._waveCount = _secondWaveCount;

			_enemyGroupController->SetCreateInfo(first, second);
			_enemyGroupController->CreateEnemy();
			Destroy(GetGameObject());
		}
	}
}