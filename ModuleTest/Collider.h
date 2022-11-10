#pragma once
#include "DUOLPhysics/DUOLPhysics.h"

#include <string>

class Collider
{

public:
	Collider(const std::string& name);

	std::string name;

	virtual void OnCollisionEnter(const Collider& other) = 0;

	virtual void OnCollisionStay(const Collider& other) = 0;

	virtual void OnCollisionExit(const Collider& other) = 0;
};