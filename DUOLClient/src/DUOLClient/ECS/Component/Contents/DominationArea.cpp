#include "DUOLClient/ECS/Component/Contents/DominationArea.h"

#include "DUOLGameEngine/ECS/Component/BoxCollider.h"
#include "DUOLGameEngine/ECS/GameObject.h"

#include <rttr/registration>

#include "DUOLCommon/MetaDataType.h"

using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::class_<DUOLClient::DominationArea>("DominationArea")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.property("destroyIncreasePerMonster", &DUOLClient::DominationArea::_destroyIncreasePerMonster)
	(
		metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
		, metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
		.property("maxClearPercent", &DUOLClient::DominationArea::_maxClearPercent)
	(
		metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
		, metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
			.property("dominationTime", &DUOLClient::DominationArea::_dominationTime)
	(
		metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
		, metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
			.property("currentClearPercent", &DUOLClient::DominationArea::_currentClearPercent)
	(
		metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
		, metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
			.property("isCleared", &DUOLClient::DominationArea::_isCleared)
	(
		metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Bool)
		, metadata(DUOLCommon::MetaDataType::Serializable, true)
	);


}
namespace DUOLClient
{

	DominationArea::DominationArea(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		MonoBehaviourBase(owner, name)
		, _destroyIncreasePerMonster(1)
		, _maxClearPercent(100)
		, _currentClearPercent(0)
		, _dominationTime(10)
		, _isCleared(false)
		, _currentEnemyCountInZone(0)
		, _playerInZone(0)
	{
	}

	DominationArea::~DominationArea()
	{
	}

	bool DominationArea::IsIsCleared() const
	{
		return _isCleared;
	}

	void DominationArea::OnEnable()
	{
	}

	void DominationArea::OnDisable()
	{
	}

	void DominationArea::OnAwake()
	{
		_areaCollider = GetGameObject()->GetComponent<DUOLGameEngine::BoxCollider>();
		_areaCollider->SetIsTrigger(true);
	}

	void DominationArea::OnStart()
	{
		//GetTGetComponent<DUOLGameEngine::BoxCollider>();
	}

	void DominationArea::OnDestroy()
	{
	}

	void DominationArea::OnUpdate(float deltaTime)
	{
		if (!_isCleared)
		{
			if (_playerInZone && _currentEnemyCountInZone == 0)
			{
				float addClear = (_maxClearPercent / _dominationTime) * deltaTime;
				_currentClearPercent += addClear;
			}
			else
			{
				float upDestroyPercent = _destroyIncreasePerMonster * _currentEnemyCountInZone;
				_currentClearPercent -= upDestroyPercent;
			}

			if(_currentClearPercent >= _maxClearPercent)
			{
				_isCleared = true;
			}
			_currentClearPercent = std::clamp(_currentClearPercent, 0.f, _maxClearPercent);
		}
	}

	void DominationArea::OnFixedUpdate(float deltaTime)
	{
		_currentEnemyCountInZone = 0;
		_playerInZone = false;
	}

	void DominationArea::OnTriggerStay(const std::shared_ptr<DUOLPhysics::Trigger>& trigger)
	{
		auto gameObject = static_cast<DUOLGameEngine::GameObject*>(trigger->_other);

		if (gameObject->GetLayer() == TEXT("EnemyRigidbody"))
		{
			_currentEnemyCountInZone++;
		}
		else if (gameObject->GetLayer() == TEXT("Player"))
		{
			_playerInZone = true;
		}
	}
}