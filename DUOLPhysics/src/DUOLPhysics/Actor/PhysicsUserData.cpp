#include "PhysicsUserData.h"

namespace DUOLPhysics
{
	PhysicsUserData::PhysicsUserData() :
		_userData(nullptr),
		_onTriggerEnter(nullptr),
		_onTriggerStay(nullptr),
		_onTriggerExit(nullptr),
		_onCollisionEnter(nullptr),
		_onCollisionStay(nullptr),
		_onCollisionExit(nullptr)
	{

	}

	void PhysicsUserData::OnTriggerEnter(const std::shared_ptr<Trigger>& data)
	{
		if (_onTriggerEnter != nullptr)
			_onTriggerEnter(data);
	}
	
	void PhysicsUserData::OnTriggerStay(const std::shared_ptr<Trigger>& data)
	{
		if (_onTriggerStay != nullptr)
			_onTriggerStay(data);
	}

	void PhysicsUserData::OnTriggerExit(const std::shared_ptr<Trigger>& data)
	{
		if (_onTriggerExit != nullptr)
			_onTriggerExit(data);
	}

	void PhysicsUserData::OnCollisionEnter(const std::shared_ptr<Collision>& data)
	{
		if (_onCollisionEnter != nullptr)
			_onCollisionEnter(data);
	}

	void PhysicsUserData::OnCollisionStay(const std::shared_ptr<Collision>& data)
	{
		if (_onCollisionStay != nullptr)
			_onCollisionStay(data);
	}

	void PhysicsUserData::OnCollisionExit(const std::shared_ptr<Collision>& data)
	{
		if (_onCollisionExit != nullptr)
			_onCollisionExit(data);
	}
}