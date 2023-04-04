#include "DUOLGameEngine/ECS/Component/NavMeshAgent.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Transform.h"

#include "DUOLGameEngine/Manager/NavigationManager.h"

#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"

using namespace rttr;

RTTR_PLUGIN_REGISTRATION
{
	rttr::registration::class_<DUOLGameEngine::NavMeshAgent>("NavMeshAgent")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.property("_baseOffset", &DUOLGameEngine::NavMeshAgent::GetBaseOffset, &DUOLGameEngine::NavMeshAgent::SetBaseOffset)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float3)
	)
	.property("_radius", &DUOLGameEngine::NavMeshAgent::GetRadius, &DUOLGameEngine::NavMeshAgent::SetRadius)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
	)
	.property("_height", &DUOLGameEngine::NavMeshAgent::GetHeight, &DUOLGameEngine::NavMeshAgent::SetHeight)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
	)
	.property("_maxAcceleration", &DUOLGameEngine::NavMeshAgent::GetMaxAcceleration, &DUOLGameEngine::NavMeshAgent::SetMaxAcceleration)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
	)
	.property("_maxSpeed", &DUOLGameEngine::NavMeshAgent::GetMaxSpeed, &DUOLGameEngine::NavMeshAgent::SetMaxSpeed)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
	)
	.property("_obstacleAvoidance", &DUOLGameEngine::NavMeshAgent::GetObstacleAvoidance, &DUOLGameEngine::NavMeshAgent::SetObstacleAvoidance)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Bool)
	)
	.property("_obstacleAvoidanceType", &DUOLGameEngine::NavMeshAgent::GetObstacleAvoidanceType, &DUOLGameEngine::NavMeshAgent::SetObstacleAvoidanceType)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
	)
	.property("_separation", &DUOLGameEngine::NavMeshAgent::GetSeparation, &DUOLGameEngine::NavMeshAgent::SetSeparation)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Bool)
	)
	.property("_separationWeight", &DUOLGameEngine::NavMeshAgent::GetSeparationWeight, &DUOLGameEngine::NavMeshAgent::SetSeparationWeight)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
	)
	.property("_anticipateTurns", &DUOLGameEngine::NavMeshAgent::GetAnticipateTurns, &DUOLGameEngine::NavMeshAgent::SetAnticipateTurns)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Bool)
	)
	.property("_optimizeVisibility", &DUOLGameEngine::NavMeshAgent::GetOptimizeVisibility, &DUOLGameEngine::NavMeshAgent::SetOptimizeVisibility)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Bool)
	)
	.property("_optimizeTopology", &DUOLGameEngine::NavMeshAgent::GetOptimizeTopology, &DUOLGameEngine::NavMeshAgent::SetOptimizeTopology)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Bool)
	);
}

namespace DUOLGameEngine
{
	NavMeshAgent::NavMeshAgent(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		BehaviourBase(owner, name)
		, _baseOffset(0.f)
		, _radius(0.6f)
		, _height(2.0f)
		, _maxAcceleration(8.f)
		, _maxSpeed(3.5f)
		, _obstacleAvoidance(false)
		, _obstacleAvoidanceType(3)
		, _separation(false)
		, _separationWeight(2.f)
		, _anticipateTurns(true)
		, _optimizeVisibility(true)
		, _optimizeTopology(true)
		, _primitiveAgent(nullptr)
	{

	}

	NavMeshAgent::~NavMeshAgent()
	{

	}

	DUOLMath::Vector3 NavMeshAgent::ConvertForFBXBinaryExporter(float x, float y, float z) const
	{
		static DUOLMath::Quaternion q = DUOLMath::Quaternion::CreateFromYawPitchRoll(180.0f * DUOLMath::PI / 180.0f, -90.0f * DUOLMath::PI / 180.0f, 0.f);

		static DUOLMath::Matrix rotMatrix = DUOLMath::Matrix::Identity * DUOLMath::Matrix::CreateFromQuaternion(q);

		DUOLMath::Vector3 retPos = { x, z, y };

		return DUOLMath::Vector3::Transform(retPos, rotMatrix);
	}

	void NavMeshAgent::UpdateAgentParameters()
	{
		DUOLGameEngine::NavigationManager::GetInstance()->UpdateAgentParameters(this, _primitiveAgentIndex);
	}

	const DUOLMath::Vector3& NavMeshAgent::GetBaseOffset() const
	{
		return _baseOffset;
	}

	void NavMeshAgent::SetBaseOffset(const DUOLMath::Vector3& baseOffset)
	{
		_baseOffset = baseOffset;
	}

	float NavMeshAgent::GetRadius() const
	{
		return _radius;
	}

	void NavMeshAgent::SetRadius(float value)
	{
		_radius = value;

		UpdateAgentParameters();
	}

	float NavMeshAgent::GetHeight() const
	{
		return _height;
	}

	void NavMeshAgent::SetHeight(float value)
	{
		_height = value;

		UpdateAgentParameters();
	}

	float NavMeshAgent::GetMaxAcceleration() const
	{
		return _maxAcceleration;
	}

	void NavMeshAgent::SetMaxAcceleration(float value)
	{
		_maxAcceleration = value;

		UpdateAgentParameters();
	}

	float NavMeshAgent::GetMaxSpeed() const
	{
		return _maxSpeed;
	}

	void NavMeshAgent::SetMaxSpeed(float value)
	{
		_maxSpeed = value;

		UpdateAgentParameters();
	}

	unsigned char NavMeshAgent::GetObstacleAvoidanceType() const
	{
		return _obstacleAvoidanceType;
	}

	void NavMeshAgent::SetObstacleAvoidanceType(unsigned char value)
	{
		_obstacleAvoidanceType = value;

		UpdateAgentParameters();
	}

	float NavMeshAgent::GetSeparationWeight() const
	{
		return _separationWeight;
	}

	void NavMeshAgent::SetSeparationWeight(float value)
	{
		_separationWeight = value;

		UpdateAgentParameters();
	}

	bool NavMeshAgent::GetAnticipateTurns() const
	{
		return _anticipateTurns;
	}

	void NavMeshAgent::SetAnticipateTurns(bool value)
	{
		_anticipateTurns = value;

		UpdateAgentParameters();
	}

	bool NavMeshAgent::GetOptimizeVisibility() const
	{
		return _optimizeVisibility;
	}

	void NavMeshAgent::SetOptimizeVisibility(bool value)
	{
		_optimizeVisibility = value;

		UpdateAgentParameters();
	}

	bool NavMeshAgent::GetOptimizeTopology() const
	{
		return _optimizeTopology;
	}

	void NavMeshAgent::SetOptimizeTopology(bool value)
	{
		_optimizeTopology = value;

		UpdateAgentParameters();
	}

	bool NavMeshAgent::GetSeparation() const
	{
		return _separation;
	}

	void NavMeshAgent::SetSeparation(bool value)
	{
		_separation = value;

		UpdateAgentParameters();
	}

	bool NavMeshAgent::GetObstacleAvoidance() const
	{
		return _obstacleAvoidance;
	}

	void NavMeshAgent::SetObstacleAvoidance(bool value)
	{
		_obstacleAvoidance = value;

		UpdateAgentParameters();
	}

	DUOLMath::Vector3 NavMeshAgent::GetVelocity() const
	{
		return ConvertForFBXBinaryExporter(_primitiveAgent->vel[0], _primitiveAgent->vel[1], _primitiveAgent->vel[2]);
	}

	bool NavMeshAgent::SetVelocity(const DUOLMath::Vector3& newVelo)
	{
		DUOLMath::Vector3 convertedVelo = ConvertForFBXBinaryExporter(newVelo.x, newVelo.y, newVelo.z);

		return NavigationManager::GetInstance()->RequestMoveVelocity(convertedVelo, this);
	}

	DUOLMath::Vector3 NavMeshAgent::GetDestination() const
	{
		return DUOLMath::Vector3(_destinationPos);
	}

	bool NavMeshAgent::SetDestination(const DUOLMath::Vector3& dest)
	{
		if (_primitiveAgent->state != DT_CROWDAGENT_STATE_WALKING)
			return false;

		// 좌표계 변환해서 보내자.
		DUOLMath::Vector3 convertedDest = ConvertForFBXBinaryExporter(dest.x, dest.y, dest.z);

		// 이 안에서 destinationRef, destionPos 전부 설정됩니다.
		return NavigationManager::GetInstance()->RequestMoveTarget(convertedDest, this);
	}

	DUOLMath::Vector3 NavMeshAgent::GetPosition() const
	{
		return _primitiveAgent != nullptr ?
			ConvertForFBXBinaryExporter(_primitiveAgent->npos[0], _primitiveAgent->npos[1], _primitiveAgent->npos[2]) : DUOLMath::Vector3::Zero;
	}

	void NavMeshAgent::SetPosition(const DUOLMath::Vector3& worldPos)
	{
		GetTransform()->SetPosition(worldPos, Space::World);

		DUOLMath::Vector3 newPos = ConvertForFBXBinaryExporter(worldPos.x, worldPos.y, worldPos.z);

		// 에이전트를 삭제하고 ..
		DUOLGameEngine::NavigationManager::GetInstance()->RemoveAgent(_primitiveAgentIndex);

		// 다시 생성합니다 ..
		DUOLGameEngine::NavigationManager::GetInstance()->AddAgent(this);
	}

	void NavMeshAgent::OnEnable()
	{
		BehaviourBase::OnEnable();

		// 현재 위치로 Crowd Agent 이동 ?
		if (_primitiveAgent == nullptr)
			DUOLGameEngine::NavigationManager::GetInstance()->AddAgent(this);
	}

	void NavMeshAgent::OnDisable()
	{
		BehaviourBase::OnDisable();

		// Crowd Agent 꺼주기 ..?
		DUOLGameEngine::NavigationManager::GetInstance()->RemoveAgent(_primitiveAgentIndex);

		_primitiveAgent = nullptr;

		_primitiveAgentIndex = -1;
	}

	// TODO : NavigationManager 에서 통합 업데이트를 치자.
	void NavMeshAgent::OnUpdate(float deltaTime)
	{
		// TODO : Agent 가 이동 중이라면 위치를 받아와서 좌표계 변환 후, 트랜스폼의 위치 설정합니다.
		if ((_primitiveAgent != nullptr) && (_primitiveAgent->active))
		{
			// WALKING 
			if (_primitiveAgent->state == DT_CROWDAGENT_STATE_WALKING)
			{
				GetTransform()->SetPosition(ConvertForFBXBinaryExporter(_primitiveAgent->npos[0], _primitiveAgent->npos[1], _primitiveAgent->npos[2]) + _baseOffset, Space::World);
			}
		}
	}
}