#include "Collider.h"

#include <iostream>

Collider::Collider(const std::string& name) :
	name(name)
{

}

void Collider::SetEvent(std::weak_ptr<PhysicsActorBase> actor)
{
	actor.lock()->SetUserData(this);

	//actor.lock()->SetCollisionEnterEvent([=](const std::shared_ptr<Collision>& other)
	//	{
	//		this->OnCollisionEnter(*reinterpret_cast<Collider*>(other->_other));
	//	});

	//actor.lock()->SetCollisionStayEvent([=](const std::shared_ptr<Collision>& other)
	//	{
	//		this->OnCollisionStay(*reinterpret_cast<Collider*>(other->_other));
	//	});

	//actor.lock()->SetCollisionExitEvent([=](const std::shared_ptr<Collision>& other)
	//	{
	//		this->OnCollisionExit(*reinterpret_cast<Collider*>(other->_other));
	//	});

	//actor.lock()->SetTriggerEnterEvent([=](const std::shared_ptr<Trigger>& other)
	//	{
	//		this->OnTriggerEnter(*reinterpret_cast<Collider*>(other->_other));
	//	});

	//actor.lock()->SetTriggerStayEvent([=](const std::shared_ptr<Trigger>& other)
	//	{
	//		this->OnTriggerStay(*reinterpret_cast<Collider*>(other->_other));
	//	});

	//actor.lock()->SetTriggerExitEvent([=](const std::shared_ptr<Trigger>& other)
	//	{
	//		this->OnTriggerExit(*reinterpret_cast<Collider*>(other->_other));
	//	});
}