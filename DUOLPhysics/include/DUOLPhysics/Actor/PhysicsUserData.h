#pragma once
/* etc */
#include "DUOLMath/DUOLMath.h"

#include <functional>

namespace DUOLPhysics
{
	using namespace DUOLMath;

	struct ContactData
	{
		void*	_ohter;

		Vector3	_position;

		float	_separation;

		Vector3	_normal;

		Vector3	_impulse;
	};

	class PhysicsUserData
	{
	public:
		using CollisionEvent = std::function<void(const ContactData&)>;

	public:
		PhysicsUserData();

		~PhysicsUserData();

	private:
		void* _userData;

		CollisionEvent _onEnter;

		CollisionEvent _onStay;

		CollisionEvent _onOut;

	public:

		void OnColliderEnter(const ContactData& data);

		void OnColliderStay(const ContactData& data);

		void OnColliderOut(const ContactData& data);

		void* GetUserData() { return _userData; }

		void SetUserData(void* userData) { _userData = userData; }

		void SetEnterEvent(CollisionEvent enter) { _onEnter = enter; }

		void SetStayEvent(CollisionEvent stay) { _onStay = stay; }

		void SetOutEvent(CollisionEvent out) { _onOut = out; }
	};
}