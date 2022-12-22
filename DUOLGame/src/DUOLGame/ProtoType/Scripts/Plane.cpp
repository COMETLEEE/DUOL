#include "DUOLGame/ProtoType/Scripts/Plane.h"

#include "DUOLGame/ProtoType/Scripts/PlayerState.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/BoxCollider.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"

#include "DUOLCommon/LogHelper.h"

namespace DUOLGame
{
	Plane::Plane(const std::weak_ptr<GameObject>& owner) :
		MonoBehaviourBase(owner)
	{

	}

	Plane::~Plane()
	{

	}

	void Plane::OnCollisionEnter(const std::shared_ptr<DUOLPhysics::Collision>& collision)
	{
		auto other = static_cast<GameObject*>(collision->_other);

		if (other->GetTag().compare(_T("Player")) == 0)
		{
			other->GetComponent<PlayerState>()->_isGround = true;

			DUOL_INFO("Player collided with Ground.");
		}
	}

	void Plane::OnCollisionExit(const std::shared_ptr<DUOLPhysics::Collision>& collision)
	{
		auto other = static_cast<GameObject*>(collision->_other);

		if (other->GetTag().compare(_T("Player")) == 0)
		{
			other->GetComponent<PlayerState>()->_isGround = false;
		}
	}

	void Plane::OnStart()
	{
		GetGameObject()->GetComponent<BoxCollider>()->SetSize({ 100.0f, 5.0f, 100.0f });
	}
}