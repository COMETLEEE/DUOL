#pragma once
/* etc */
#include "DUOLPhysics/Util/PhysicsDataStructure.h"

#include <vector>
#include <memory>
#include <functional>

namespace DUOLPhysics
{
	class PhysicsUserData
	{
	public:
		PhysicsUserData();

		~PhysicsUserData();

	private:
		void* _userData;

		TriggerEvent _onTriggerEnter;

		TriggerEvent _onTriggerStay;

		TriggerEvent _onTriggerExit;

		CollisionEvent _onCollisionEnter;

		CollisionEvent _onCollisionStay;

		CollisionEvent _onCollisionExit;

	public:

		void* GetUserData() { return _userData; }

		void OnTriggerEnter(const std::shared_ptr<Trigger>& data);

		void OnTriggerStay(const std::shared_ptr<Trigger>& data);

		void OnTriggerExit(const std::shared_ptr<Trigger>& data);

		void OnCollisionEnter(const std::shared_ptr<Collision>& data);

		void OnCollisionStay(const std::shared_ptr<Collision>& data);

		void OnCollisionExit(const std::shared_ptr<Collision>& data);

		void SetUserData(void* userData) { _userData = userData; }

		void SetTriggerEnterEvent(TriggerEvent enter) { _onTriggerEnter = enter; }

		void SetTriggerStayEvent(TriggerEvent stay) { _onTriggerStay = stay; }

		void SetTriggerExitEvent(TriggerEvent exit) { _onTriggerExit = exit; }

		void SetCollisionEnterEvent(CollisionEvent enter) { _onCollisionEnter = enter; }

		void SetCollisionStayEvent(CollisionEvent stay) { _onCollisionStay = stay; }

		void SetCollisionExitEvent(CollisionEvent exit) { _onCollisionExit = exit; }
	};
}