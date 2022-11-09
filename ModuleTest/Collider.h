#pragma once
#include "DUOLPhysics/DUOLPhysics.h"

#include <string>

class Collider
{

public:
	Collider(const std::string& name);

	std::string name;

	virtual void OnCollisionEnter(const Collider& other) = 0;
};