#include "DUOLClient/ECS/Component/TestParticle.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include <rttr/registration.h>

#include "DUOLClient/Manager/ParticleManager.h"
#include "DUOLGameEngine/ECS/Component/ParticleRenderer.h"
#include "DUOLGameEngine/Manager/InputManager.h"
#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"

#include "DUOLGameEngine/Manager/InputManager.h"

using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::class_<DUOLClient::TestParticle>("TestParticle")
	.constructor<>()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	);
}

DUOLClient::TestParticle::TestParticle(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
	DUOLGameEngine::MonoBehaviourBase(owner, name)
	, _playOnce(false)
{
}

DUOLClient::TestParticle::~TestParticle()
{
}

void DUOLClient::TestParticle::OnStart()
{
}

void DUOLClient::TestParticle::OnUpdate(float deltaTime)
{
	if (_playOnce == false)
	{

		_playOnce = true;
	}

	auto InputManager = DUOLGameEngine::InputManager::GetInstance();

	if (InputManager->GetKeyDown(DUOLGameEngine::KeyCode::F1))
	{
		auto particleData = ParticleManager::GetInstance()->Pop(ParticleEnum::test, 3);
		particleData->GetTransform()->SetParent(GetGameObject()->GetTransform());
	}

	if (InputManager->GetKeyDown(DUOLGameEngine::KeyCode::F2))
	{
		auto particleData = ParticleManager::GetInstance()->Pop(ParticleEnum::test, 5);
		particleData->GetTransform()->SetParent(GetGameObject()->GetTransform());
	}

	if (InputManager->GetKeyDown(DUOLGameEngine::KeyCode::F3))
	{
		auto particleData = ParticleManager::GetInstance()->Pop(ParticleEnum::test);
		particleData->GetTransform()->SetParent(GetGameObject()->GetTransform());
	}

	if (InputManager->GetKeyDown(DUOLGameEngine::KeyCode::F4))
	{
		auto particleData = ParticleManager::GetInstance()->Pop(ParticleEnum::test, 1);
		particleData->GetTransform()->SetParent(GetGameObject()->GetTransform());
	}
}
