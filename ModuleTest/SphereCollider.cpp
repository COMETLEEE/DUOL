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

void SphereCollider::OnTriggerEnter(const Collider& other)
{
	std::cout << "Trigger Enter : Sphere -> " << other.name << std::endl;
}

void SphereCollider::OnTriggerStay(const Collider& other)
{
	std::cout << "Trigger Stay : Sphere -> " << other.name << std::endl;
}

void SphereCollider::OnTriggerExit(const Collider& other)
{
	std::cout << "Trigger Exit : Sphere -> " << other.name << std::endl;
}

void SphereCollider::OnRaycastHit()
{
	std::cout << "Ray Hit To Sphere" << std::endl;
}