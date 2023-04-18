#include "DUOLClient/ECS/Component/Enemy/EnemyAirborneCheck.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/BoxCollider.h"

#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"

using namespace rttr;

RTTR_REGISTRATION
{
	rttr::registration::class_<DUOLClient::EnemyAirborneCheck>("EnemyAirborneCheck")
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
	EnemyAirborneCheck::EnemyAirborneCheck(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		DUOLGameEngine::MonoBehaviourBase(owner, name), _grounds(), _boxCollider(nullptr)
	{

	}

	bool EnemyAirborneCheck::GetIsAirborne()
	{
		if (_grounds.empty())
			return true;

		return false;
	}

	void EnemyAirborneCheck::Initialize(float boxSize, float boxHeight)
	{
		_boxCollider = GetGameObject()->AddComponent<DUOLGameEngine::BoxCollider>();

		_boxCollider->SetSize(DUOLMath::Vector3(boxSize, boxSize, boxSize));

		_boxCollider->SetIsTrigger(true);

		_boxCollider->SetCenter(DUOLMath::Vector3(0, boxHeight, 0));

		GetGameObject()->SetLayer(TEXT("EnemyBottomCheck"));

		GetGameObject()->GetTransform()->SetLocalPosition(DUOLMath::Vector3(0, 0, 0));
	}

	void EnemyAirborneCheck::OnDisable()
	{
		_grounds.clear();
	}

	void EnemyAirborneCheck::OnTriggerEnter(const std::shared_ptr<DUOLPhysics::Trigger>& trigger)
	{
		auto otherGameObjet = reinterpret_cast<DUOLGameEngine::GameObject*>(trigger->_other);

		auto iter = std::find(_grounds.begin(), _grounds.end(), otherGameObjet);

		if (iter == _grounds.end())
			_grounds.push_back(otherGameObjet);

	}

	void EnemyAirborneCheck::OnTriggerExit(const std::shared_ptr<DUOLPhysics::Trigger>& trigger)
	{
		auto otherGameObjet = reinterpret_cast<DUOLGameEngine::GameObject*>(trigger->_other);

		auto iter = std::find(_grounds.begin(), _grounds.end(), otherGameObjet);

		if (iter != _grounds.end())
			_grounds.erase(iter);
	}
}
