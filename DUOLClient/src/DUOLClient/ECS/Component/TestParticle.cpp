#include "DUOLClient/ECS/Component/TestParticle.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include <rttr/registration.h>

#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"

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
		auto obj1 =  DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->CreateFromParticleData(L"HitEffect.dfx");;
		obj1->GetTransform()->SetPosition(GetTransform()->GetWorldPosition());

		obj1 = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->CreateFromParticleData(L"testParticle.dfx");;
		obj1->GetTransform()->SetPosition(GetTransform()->GetWorldPosition());
		_playOnce = true;
	}


}
