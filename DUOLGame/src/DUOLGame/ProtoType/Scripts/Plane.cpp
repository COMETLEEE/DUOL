#include "DUOLGame/ProtoType/Scripts/Plane.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/BoxCollider.h"
#include "DUOLGameEngine/ECS/Component/Rigidbody.h"

namespace DUOLGame
{
	Plane::Plane(const std::weak_ptr<GameObject>& owner) :
		MonoBehaviourBase(owner)
	{

	}

	Plane::~Plane()
	{

	}

	void Plane::OnStart()
	{
		GetGameObject()->GetComponent<BoxCollider>()->SetSize({ 100.0f, 5.0f, 100.0f });
	}
}