#include "DUOLClient/Manager/ParticleManager.h"
#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/ParticleRenderer.h"
#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"

using namespace rttr;

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
	);

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

		auto scene = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene();

		constexpr int queueCount = 10;

		for (int i = 0; i < std::size(ParticlePath); i++)
		{
			for (int j = 0; j < queueCount; j++)
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
			return DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->CreateEmpty()->AddComponent<ParticleManager>();
	}

	void ParticleManager::PushBack(ParticleData* particleData)
	{
		if (particleData->_particleEnum != ParticleEnum::Empty)
		{
			auto test = particleData->GetGameObject()->GetIsActive();

			particleData->GetGameObject()->SetIsActive(false);

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

				particleData->GetGameObject()->SetIsActive(true);

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

	void ParticleManager::OnStart()
	{
		if (!_instance)
			_instance = this;
		else
			Destroy(this);
	}

	void ParticleManager::OnUpdate(float deltaTime)
	{
		if (!_isStart)
		{
			_isStart = true;

			_particleQueueGameObject = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->CreateEmpty();

			_particleQueueGameObject->GetTransform()->SetParent(this->GetGameObject()->GetTransform());

			_particleQueueGameObject->SetName(TEXT("ParticleQueue"));

			Initialize();
		}
	}
}
