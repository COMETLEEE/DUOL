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

	void OnTriggerEnter(const Collider& other) override;

	void OnTriggerStay(const Collider& other) override;

	void OnTriggerExit(const Collider& other) override;
};