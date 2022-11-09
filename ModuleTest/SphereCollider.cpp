#include "SphereCollider.h"

SphereCollider::SphereCollider() :
	Collider("Sphere")
{

}

void SphereCollider::OnCollisionEnter(const Collider& other)
{
	std::cout << "Collision Enter : Sphere -> " << other.name << std::endl;
}