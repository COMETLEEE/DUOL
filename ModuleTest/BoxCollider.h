#pragma once
#include "Collider.h"

class BoxCollider : public Collider
{
public:
	BoxCollider();

public:
	void OnCollisionEnter(const Collider& other) override;

	void OnCollisionStay(const Collider& other) override;

	void OnCollisionExit(const Collider& other) override;
};