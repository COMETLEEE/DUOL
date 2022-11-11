#pragma once
#include "DUOLPhysics/DUOLPhysics.h"

#include <string>

using namespace DUOLPhysics;

class Collider
{
public:
	std::string name;

	Collider(const std::string& name);

	void SetEvent(std::weak_ptr<PhysicsActorBase> actor);

	virtual void OnCollisionEnter(const Collider& /* other */) = 0;

	virtual void OnCollisionStay(const Collider& /* other */) = 0;

	virtual void OnCollisionExit(const Collider& /* other */) = 0;

	virtual void OnTriggerEnter(const Collider& /* other */) = 0;

	virtual void OnTriggerStay(const Collider& /* other */) = 0;

	virtual void OnTriggerExit(const Collider& /* other */) = 0;

	virtual void OnRaycastHit() = 0;
};