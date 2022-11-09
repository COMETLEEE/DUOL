#pragma once
#include "Collider.h"

class SphereCollider : public Collider
{
public:
	SphereCollider();

public:
	void OnCollisionEnter(const Collider& other) override;
};