#include "SphereCollider.h"

SphereCollider::SphereCollider() :
	Collider("Sphere")
{

}

void SphereCollider::OnCollisionEnter(const Collider& other)
{
	std::cout << "Collision Enter : Sphere -> " << other.name << std::endl;
}

void SphereCollider::OnCollisionStay(const Collider& other)
{
	std::cout << "Collision Stay : Sphere -> " << other.name << std::endl;
}

void SphereCollider::OnCollisionExit(const Collider& other)
{
	std::cout << "Collision Exit : Sphere -> " << other.name << std::endl;
}