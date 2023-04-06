#include "DUOLClient/Player/Weapon_Scaler.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Transform.h"

#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"

using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::class_<DUOLClient::Weapon_Scaler>("Weapon_Scaler")
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
	Weapon_Scaler::Weapon_Scaler(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		DUOLGameEngine::MonoBehaviourBase(owner, name)
	{
	}

	Weapon_Scaler::~Weapon_Scaler()
	{
	}

	void Weapon_Scaler::OnAwake()
	{
		MonoBehaviourBase::OnAwake();

		GetTransform()->SetLocalScale(DUOLMath::Vector3{ 100.f, 100.f ,100.f });
	}
}