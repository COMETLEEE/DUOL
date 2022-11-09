#pragma once
#include "DUOLMath/DUOLMath.h"

namespace DUOLPhysics
{
	using namespace DUOLMath;

	struct ContactData
	{
		Vector3	_position;

		Vector3	_normal;

		Vector3	_impulse;

		float	_separation;
	};

	struct Collision
	{
		void* _other;

		std::vector<ContactData> _data;
	};

	using CollisionEvent = std::function<void(const std::shared_ptr<Collision>&)>;

	struct Trigger
	{
		void* _other;
	};

	using TriggerEvent = std::function<void(const std::shared_ptr<Trigger>&)>;

	struct GlobalPose
	{
		DUOLMath::Vector3 _position;

		DUOLMath::Quaternion _quaternion;
	};

	struct PhysicsBoundingBox
	{
		DUOLMath::Vector3 _min;

		DUOLMath::Vector3 _max;
	};
}