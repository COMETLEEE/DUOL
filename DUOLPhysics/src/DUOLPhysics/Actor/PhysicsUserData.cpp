#include "DUOLPhysics/Actor/PhysicsUserData.h"

namespace DUOLPhysics
{
	PhysicsUserData::PhysicsUserData() :
		_userData(nullptr),
		_onEnter(nullptr),
		_onStay(nullptr),
		_onOut(nullptr)
	{

	}

	PhysicsUserData::~PhysicsUserData()
	{

	}

	void PhysicsUserData::OnColliderEnter(const ContactData& data)
	{
		if (_onEnter != nullptr)
			_onEnter(data);
	}

	void PhysicsUserData::OnColliderStay(const ContactData& data)
	{
		if (_onStay != nullptr)
			_onStay(data);
	}

	void PhysicsUserData::OnColliderOut(const ContactData& data)
	{
		if (_onOut != nullptr)
			_onOut(data);
	}
}