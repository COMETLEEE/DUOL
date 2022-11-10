#pragma once
#include "Collider.h"

class SphereCollider : public Collider
{
public:
	SphereCollider();

public:
	void OnCollisionEnter(const Collider& other) override;

	void OnCollisionStay(const Collider& other) override;

	void OnCollisionExit(const Collider& other) override;
};