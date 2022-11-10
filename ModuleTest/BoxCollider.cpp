#include "BoxCollider.h"

BoxCollider::BoxCollider() :
	Collider("Box")
{
}

void BoxCollider::OnCollisionEnter(const Collider& other)
{
	std::cout << "Collision Enter : Box -> " << other.name << std::endl;
}

void BoxCollider::OnCollisionStay(const Collider& other)
{
	std::cout << "Collision Stay : Box -> " << other.name << std::endl;
}

void BoxCollider::OnCollisionExit(const Collider& other)
{
	std::cout << "Collision Exit : Box -> " << other.name << std::endl;
}

void BoxCollider::OnTriggerEnter(const Collider& other)
{
	std::cout << "Trigger Enter : Box -> " << other.name << std::endl;
}

void BoxCollider::OnTriggerStay(const Collider& other)
{
	std::cout << "Trigger Stay : Box -> " << other.name << std::endl;
}

void BoxCollider::OnTriggerExit(const Collider& other)
{
	std::cout << "Trigger Exit : Box -> " << other.name << std::endl;
}