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
	.property("_radius", &DUOLGameEngine::NavMeshAgent::GetRadius, &DUOLGameEngine::NavMeshAgent::SetRadius)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("_height", &DUOLGameEngine::NavMeshAgent::GetHeight, &DUOLGameEngine::NavMeshAgent::SetHeight)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	);
}

namespace DUOLGameEngine
{
	NavMeshAgent::NavMeshAgent(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		BehaviourBase(owner, name)
		, _radius(0.6f)
		, _height(2.0f)
		, _maxAcceleration(8.f)
		, _maxSpeed(3.5f)
		, _obstacleAvoidanceType(3)
		, _separationWeight(2.f)
		, _anticipateTurns(true)
		, _optimizeVis(true)
		, _optimizeTopo(true)
		, _separation(false)
		, _primitiveAgent(nullptr)
	{

	}

	NavMeshAgent::~NavMeshAgent()
	{

	}

	DUOLMath::Vector3 NavMeshAgent::ConvertForFBXBinaryExporter(float x, float y, float z)
	{
		static DUOLMath::Quaternion q = DUOLMath::Quaternion::CreateFromYawPitchRoll(180.0f * DUOLMath::PI / 180.0f, -90.0f * DUOLMath::PI / 180.0f, 0.f);

		static DUOLMath::Matrix rotMatrix = DUOLMath::Matrix::Identity * DUOLMath::Matrix::CreateFromQuaternion(q);

		DUOLMath::Vector3 retPos = { x, z, y };

		return DUOLMath::Vector3::Transform(retPos, rotMatrix);
	}

	float NavMeshAgent::GetRadius() const
	{
		return _radius;
	}

	void NavMeshAgent::SetRadius(float value)
	{
		_radius = value;
	}

	float NavMeshAgent::GetHeight() const
	{
		return _height;
	}

	void NavMeshAgent::SetHeight(float value)
	{
		_height = value;
	}

	float NavMeshAgent::GetMaxAcceleration() const
	{
		return _maxAcceleration;
	}

	void NavMeshAgent::SetMaxAcceleration(float value)
	{
		_maxAcceleration = value;
	}

	float NavMeshAgent::GetMaxSpeed() const
	{
		return _maxSpeed;
	}

	void NavMeshAgent::SetMaxSpeed(float value)
	{
		_maxSpeed = value;
	}

	unsigned char NavMeshAgent::GetObstacleAvoidanceType() const
	{
		return _obstacleAvoidanceType;
	}

	void NavMeshAgent::SetObstacleAvoidanceType(unsigned char value)
	{
		_obstacleAvoidanceType = value;
	}

	float NavMeshAgent::GetSeparationWeight() const
	{
		return _separationWeight;
	}

	void NavMeshAgent::SetSeparationWeight(float value)
	{
		_separationWeight = value;
	}

	bool NavMeshAgent::GetAnticipateTurns() const
	{
		return _anticipateTurns;
	}

	void NavMeshAgent::SetAnticipateTurns(bool value)
	{
		_anticipateTurns = value;
	}

	bool NavMeshAgent::GetOptimizeVis() const
	{
		return _optimizeVis;
	}

	void NavMeshAgent::SetOptimizeVis(bool value)
	{
		_optimizeVis = value;
	}

	bool NavMeshAgent::GetOptimizeTopo() const
	{
		return _optimizeTopo;
	}

	void NavMeshAgent::SetOptimizeTopo(bool value)
	{
		_optimizeTopo = value;
	}

	bool NavMeshAgent::GetSeparation() const
	{
		return _separation;
	}

	void NavMeshAgent::SetSeparation(bool value)
	{
		_separation = value;

		// TODO : Agent Option 바꾸는 법
		// _crowd->updateAgentParam();
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

	void NavMeshAgent::OnEnable()
	{
		BehaviourBase::OnEnable();
	}

	void NavMeshAgent::OnDisable()
	{
		BehaviourBase::OnDisable();
	}

	void NavMeshAgent::OnUpdate(float deltaTime)
	{
		// TODO : Agent 가 이동 중이라면 위치를 받아와서 좌표계 변환 후, 트랜스폼의 위치 설정합니다.
		if ((_primitiveAgent != nullptr) && (_primitiveAgent->active))
		{
			// WALKING 
			if (_primitiveAgent->state == DT_CROWDAGENT_STATE_WALKING)
				GetTransform()->SetPosition(ConvertForFBXBinaryExporter(_primitiveAgent->npos[0], _primitiveAgent->npos[1], _primitiveAgent->npos[2]), Space::World);
		}
	}
}