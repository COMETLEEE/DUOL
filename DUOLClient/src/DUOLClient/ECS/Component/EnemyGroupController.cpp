#include "DUOLClient/ECS/Component/EnemyGroupController.h"

#include <rttr/registration>

#include "DUOLClient/ECS/Component/AI_Enemy.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/NavMeshAgent.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"
#include "DUOLGameEngine/ECS/Component/CapsuleCollider.h"
#include "DUOLGameEngine/ECS/Component/Animator.h"

#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"

#include "DUOLCommon/MetaDataType.h"
#include "DUOLGameEngine/Manager/ResourceManager.h"

using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::class_<DUOLClient::EnemyGroupController>("EnemyGroupController")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.property("_radius",&DUOLClient::EnemyGroupController::_radius)
	(
			metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
	)
	.property("_count",&DUOLClient::EnemyGroupController::_count)
	(
			metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Int)
	);
}



DUOLClient::EnemyGroupController::EnemyGroupController(DUOLGameEngine::GameObject* owner,
	const DUOLCommon::tstring& name) :MonoBehaviourBase(owner, name),
	_Enemys(), _radius(0), _count(0), _targetPos(), _isGroupCheck(false)
{
}


void DUOLClient::EnemyGroupController::Initialize(float radius, float count)
{
	_radius = radius;
	_count = count;
	_targetPos = GetTransform()->GetWorldPosition();
}

void DUOLClient::EnemyGroupController::CreateEnemy()
{
	auto scene = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene();

	_Enemys.resize(_count);

	_targetPos = GetTransform()->GetWorldPosition();

	for (int i = 0; i < _count; i++)
	{
		//auto gameObj = scene->CreateEmpty();
		auto gameObj = scene->CreateFromFBXModel(TEXT("Standard Idle"));

		auto animator = gameObj->GetComponent<DUOLGameEngine::Animator>();

		animator->SetAnimatorController(DUOLGameEngine::ResourceManager::GetInstance()->GetAnimatorController(TEXT("ProtoAnimCon")));

		auto collider = gameObj->AddComponent<DUOLGameEngine::CapsuleCollider>();

		auto rigidbody = gameObj->AddComponent<DUOLGameEngine::Rigidbody>();

		auto navMesh = gameObj->AddComponent<DUOLGameEngine::NavMeshAgent>();

		rigidbody->SetIsFreezePosition(true);

		rigidbody->SetIsKinematic(true);

		navMesh->SetBaseOffset(DUOLMath::Vector3(0, 1.0f, 0));

		_Enemys[i] = gameObj->AddComponent<AI_Enemy>();

		_Enemys[i]->SetGroupController(this);

		DUOLMath::Vector3 randVec = DUOLMath::Vector3(
			DUOLMath::MathHelper::RandF(-_radius, _radius),
			0,
			DUOLMath::MathHelper::RandF(-_radius, _radius));

		gameObj->GetTransform()->SetPosition(_targetPos + randVec);
	}
}

void DUOLClient::EnemyGroupController::OnAwake()
{
}

void DUOLClient::EnemyGroupController::OnStart()
{

}

void DUOLClient::EnemyGroupController::OnUpdate(float deltaTime)
{
	if (!_testinit)
	{
		_testinit = true;
		CreateEnemy();
	}
}
