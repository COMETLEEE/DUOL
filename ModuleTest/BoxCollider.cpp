#include "BoxCollider.h"

BoxCollider::BoxCollider() :
	Collider("Box")
{
}

void BoxCollider::OnCollisionEnter(const Collider& other)
{
	std::cout << "Collision Enter : Box -> " << other.name << std::endl;
}