#include "DUOLClient/ECS/Component/JumpPlatform.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Transform.h"

#include <rttr/registration>

#include "DUOLClient/Player/Player.h"
#include "DUOLCommon/MetaDataType.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/Manager/TimeManager.h"

using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::class_<DUOLClient::JumpPlatform>("JumpingBoard")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.property("DestinationPoint", &DUOLClient::JumpPlatform::_destinationPoint)
	(
		metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float3)
		, metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("MaxHeight", &DUOLClient::JumpPlatform::_maxHeight)
	(
		metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
		, metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
		.property("MoveTime", &DUOLClient::JumpPlatform::_moveTime)
	(
		metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
		, metadata(DUOLCommon::MetaDataType::Serializable, true)
	).property("CoolTime", &DUOLClient::JumpPlatform::_cooltime)
	(
		metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
		, metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("StepOnTime", &DUOLClient::JumpPlatform::_stepOnTime)
	(
		metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
		, metadata(DUOLCommon::MetaDataType::Serializable, true)
	);

}

DUOLClient::JumpPlatform::JumpPlatform(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
	MonoBehaviourBase(owner, name),
	_maxHeight(25.f)
	, _destinationPoint()
	, _moveTime(5)
	, _cooltime(3)
	, _stepOnTime(2.f)

{
}

DUOLClient::JumpPlatform::~JumpPlatform()
{

}

void DUOLClient::JumpPlatform::CalculateDistance(float deltaT)
{
	_deltaT += deltaT;

	if (_deltaT > _moveTime)
	{
		_usingPlayer->JumpEnd();
		_isUsingJumpingBoard = false;
		_usingPlayer = nullptr;
		_deltaT = 0;
		_currentGravity = 0;
		_currentHorizontalSpeed = 0;
		_currentVerticalSpeed = 0;
		_jumpAnimCount = 0.f;
		return;
	}

	float verticalMoveDist = _currentVerticalSpeed * _deltaT + (_currentGravity * _deltaT * _deltaT / 2.f);
	float horizontalMoveDist = _currentHorizontalSpeed * _deltaT;

	DUOLMath::Vector3 moveDist = { _distanceVector.x * horizontalMoveDist, verticalMoveDist, _distanceVector.z * horizontalMoveDist };
	_usingPlayer->GetTransform()->SetPosition(_startPoint + moveDist);
}

void DUOLClient::JumpPlatform::SetTarget(DUOLGameEngine::GameObject* object)
{
	_usingPlayer = object->GetComponent<Player>();

	_startPoint = object->GetTransform()->GetWorldPosition();

	_distanceVector = _destinationPoint - _startPoint;
	auto heightOffset = _distanceVector.y;
	_distanceVector.y = 0;
	auto length = _distanceVector.Length();
	_distanceVector.Normalize();
	object->GetTransform()->LookAt(_startPoint + _distanceVector);
	DUOLMath::Vector2 topPoint{ length / 2, _maxHeight };
	//_moveTime/2 동안 _maxHeight를 갈 수 있는 처음시작속도는..

	_currentHorizontalSpeed = length / _moveTime;
	_currentGravity = -2 * _maxHeight / powf(_moveTime / 2.f, 2.f);
	_currentVerticalSpeed = -_currentGravity * _moveTime / 2.f;

	_deltaT = 0;
}

void DUOLClient::JumpPlatform::OnTriggerStay(const std::shared_ptr<DUOLPhysics::Trigger>& trigger)
{
	auto gameObject = static_cast<DUOLGameEngine::GameObject*>(trigger->_other);
	if (_isUsingJumpingBoard == false)
	{
		if (gameObject->GetLayer() == TEXT("Player"))
		{
			if (_currentCoolTime > _cooltime)
			{
				_currentStepOnTime += DUOLGameEngine::TimeManager::GetInstance()->GetDeltaTime();

				if (_currentStepOnTime > _stepOnTime)
				{
					_jumpBoardAnimator->SetCurrentLoopCount(0);

					//관련 숫자들 초기화
					_currentCoolTime = 0;
					_currentStepOnTime = 0;
					SetTarget(gameObject);
					_usingPlayer->Jump(JUMP_ANIM_TIME, _moveTime);
					_isUsingJumpingBoard = true;
				}
			}
		}
	}
}

void DUOLClient::JumpPlatform::OnTriggerExit(const std::shared_ptr<DUOLPhysics::Trigger>& trigger)
{
	//트리거를 나갔을때는 steponTime을 초기화합니다.
	_currentStepOnTime = 0;

}

void DUOLClient::JumpPlatform::OnUpdate(float deltaTime)
{
	if (_isUsingJumpingBoard)
	{
		_jumpAnimCount += deltaTime;
		if (_jumpAnimCount > JUMP_ANIM_TIME)
		{
			CalculateDistance(deltaTime);
		}
	}

	if (_currentCoolTime <= _cooltime)
	{
		_currentCoolTime += deltaTime;
	}
}

void DUOLClient::JumpPlatform::OnAwake()
{
	auto children = GetGameObject()->GetTransform()->GetChildGameObjects();

	for (auto& child : children)
	{
		_jumpBoardAnimator = child->GetComponent<DUOLGameEngine::Animator>();
	}
}
