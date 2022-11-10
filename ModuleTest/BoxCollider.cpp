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