#include "DUOLClient/Manager/ParticleManager.h"
#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/ParticleRenderer.h"
#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"

using namespace rttr;

/**
* \brief 큐에 적재할 Particle의 수.
*/
int DUOLClient::ParticleManager::g_ParticleCount[static_cast<unsigned long long>(DUOLClient::ParticleEnum::Empty)];

RTTR_REGISTRATION
{
	rttr::registration::class_<DUOLClient::ParticleManager>("ParticleManager")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.property("TestCount", &DUOLClient::ParticleManager::g_ParticleCount[static_cast<unsigned long long>(DUOLClient::ParticleEnum::test)])
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Int)
	)
	.property("MonsterHitCount", &DUOLClient::ParticleManager::g_ParticleCount[static_cast<unsigned long long>(DUOLClient::ParticleEnum::MonsterHit)])
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Int)
	)
	.property("RunShiftCount", &DUOLClient::ParticleManager::g_ParticleCount[static_cast<unsigned long long>(DUOLClient::ParticleEnum::RunShift)])
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Int)
	)
	.property("FistFoamCount", &DUOLClient::ParticleManager::g_ParticleCount[static_cast<unsigned long long>(DUOLClient::ParticleEnum::FistForm)])
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Int)
	)
	.property("Dash", &DUOLClient::ParticleManager::g_ParticleCount[static_cast<unsigned long long>(DUOLClient::ParticleEnum::Dash)])
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Int)
	)
	.property("OverdriveEnterCount", &DUOLClient::ParticleManager::g_ParticleCount[static_cast<unsigned long long>(DUOLClient::ParticleEnum::OverdriveEnter)])
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Int)
	)
	.property("OverdriveFistForm", &DUOLClient::ParticleManager::g_ParticleCount[static_cast<unsigned long long>(DUOLClient::ParticleEnum::OverdriveFistForm)])
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Int)
	)
	.property("CrackCount", &DUOLClient::ParticleManager::g_ParticleCount[static_cast<unsigned long long>(DUOLClient::ParticleEnum::Crack)])
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Int)
	)
	.property("DustCount", &DUOLClient::ParticleManager::g_ParticleCount[static_cast<unsigned long long>(DUOLClient::ParticleEnum::Dust)])
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Int)
	)
	.property("FistWideCount", &DUOLClient::ParticleManager::g_ParticleCount[static_cast<unsigned long long>(DUOLClient::ParticleEnum::FistWide)])
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Int)
	)
	.property("MonsterSummonCount", &DUOLClient::ParticleManager::g_ParticleCount[static_cast<unsigned long long>(DUOLClient::ParticleEnum::MonsterSummon)])
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Int)
	)
	.property("SwordTrailCount", &DUOLClient::ParticleManager::g_ParticleCount[static_cast<unsigned long long>(DUOLClient::ParticleEnum::SwordTrail)])
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Int)
	)
	.property("BigFootRushDustEffectCount", &DUOLClient::ParticleManager::g_ParticleCount[static_cast<unsigned long long>(DUOLClient::ParticleEnum::BigFootRushDustEffect)])
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Int)
	)
	.property("ShoutingCount", &DUOLClient::ParticleManager::g_ParticleCount[static_cast<unsigned long long>(DUOLClient::ParticleEnum::Shouting)])
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Int)
	)
	.property("Portal", &DUOLClient::ParticleManager::g_ParticleCount[static_cast<unsigned long long>(DUOLClient::ParticleEnum::Portal)])
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Int)
	)
	.property("UltimateSwordTrailCount", &DUOLClient::ParticleManager::g_ParticleCount[static_cast<unsigned long long>(DUOLClient::ParticleEnum::OverdriveSwordTrail)])
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Int)
	)
	.property("MonsterDashFCount", &DUOLClient::ParticleManager::g_ParticleCount[static_cast<unsigned long long>(DUOLClient::ParticleEnum::MonsterDashF)])
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Int)
	)
	.property("MonsterBigCrackCount", &DUOLClient::ParticleManager::g_ParticleCount[static_cast<unsigned long long>(DUOLClient::ParticleEnum::MonsterBigCrack)])
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Int)
	)
	;
}

namespace DUOLClient
{
	ParticleManager* ParticleManager::_instance = nullptr;

	ParticleManager::ParticleManager(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		DUOLGameEngine::MonoBehaviourBase(owner, name),
		_isStart(false), _particleQueueGameObject(nullptr), _particleQueueMap()
	{
	}

	ParticleManager::~ParticleManager()
	{
		_instance = nullptr;
	}

	DUOLGameEngine::CoroutineHandler ParticleManager::ReturnParticle(ParticleData* particleData)
	{
		co_yield std::make_shared<DUOLGameEngine::WaitForFrames>(2);

		while (true)
		{
			if (particleData->CheckCanReturnQueue())
			{
				PushBack(particleData);

				co_return;
			}

			co_yield nullptr;
		}
	}

	void ParticleManager::Initialize()
	{
		// 큐에 파티클을 적재한다.

		GetGameObject()->SetName(TEXT("ParticleManager"));

		auto scene = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene();

		for (int i = 0; i < std::size(ParticlePath); i++)
		{
			for (int j = 0; j < g_ParticleCount[i]; j++)
			{
				auto particleGameObject = scene->CreateFromParticleData(ParticlePath[i]);

				auto particleData = particleGameObject->AddComponent<ParticleData>();

				particleData->_particleEnum = static_cast<ParticleEnum>(i);

				PushBack(particleData);
			}
		}
	}

	ParticleManager* ParticleManager::GetInstance()
	{
		if (_instance)
			return _instance;
		else
			_instance = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->CreateEmpty()->AddComponent<ParticleManager>();

		return _instance;

	}

	void ParticleManager::PushBack(ParticleData* particleData)
	{
		if (particleData->_particleEnum != ParticleEnum::Empty)
		{
			auto test = particleData->GetGameObject()->GetIsActive();

			particleData->GetGameObject()->SetIsActiveSelf(false);

			particleData->GetTransform()->SetParent(_particleQueueGameObject->GetTransform());

			particleData->GetTransform()->SetLocalTM(DUOLMath::Matrix::Identity);

			_particleQueueMap[particleData->_particleEnum].push(particleData);
		}
	}

	DUOLGameEngine::ParticleRenderer* ParticleManager::Pop(ParticleEnum particleEnum, float timer)
	{
		if (particleEnum != ParticleEnum::Empty)
		{
			auto& particleQueue = _particleQueueMap[particleEnum];

			if (!particleQueue.empty())
			{
				const auto particleData = particleQueue.front();

				particleQueue.pop();

				particleData->GetTransform()->SetParent(nullptr);

				particleData->GetGameObject()->SetIsActiveSelf(true);

				particleData->_particleRenderer->Play();

				particleData->SetTimer(timer);

				// 코루틴 실행 시켜줘야함.
				StartCoroutine(&ParticleManager::ReturnParticle, particleData);
				// 큐로 다시 돌아오는...!

				return particleData->_particleRenderer;
			}
		}

		return nullptr;
	}

	void ParticleManager::OnAwake()
	{
		if (!_instance)
		{
			_instance = this;

			_particleQueueGameObject = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->CreateEmpty();

			_particleQueueGameObject->GetTransform()->SetParent(this->GetGameObject()->GetTransform());

			_particleQueueGameObject->SetName(TEXT("ParticleQueue"));

			Initialize();
		}
		else
			Destroy(this);
	}

	void ParticleManager::OnStart()
	{

	}

	void ParticleManager::OnUpdate(float deltaTime)
	{
		if (!_isStart)
		{
			_isStart = true;
		}
	}
}