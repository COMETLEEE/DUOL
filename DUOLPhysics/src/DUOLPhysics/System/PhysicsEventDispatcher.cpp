#include "PhysicsEventDispatcher.h"

/* etc */
#include "../Util/PhysicsTypeConverter.h"

namespace DUOLPhysics
{
	void PhysicsEventDispatcher::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 countOfPairs)
	{
		for (PxU32 i = 0; i < countOfPairs; i++)
		{
			auto* actor1 = reinterpret_cast<PhysicsUserData*>(pairHeader.actors[0]->userData);
			auto* actor2 = reinterpret_cast<PhysicsUserData*>(pairHeader.actors[1]->userData);

			if (actor1 == nullptr || actor2 == nullptr)
				break;

			auto collision = std::make_shared<Collision>();
			collision->_data.resize(pairs[i].contactCount);

			std::vector<PxContactPairPoint> points(pairs[i].contactCount);
			pairs[i].extractContacts(points.data(), pairs[i].contactCount);

			for (int j = 0; j < pairs[i].contactCount; j++)
			{
				collision->_data[j]._position = ConvertVector3(points[j].position);
				collision->_data[j]._normal = ConvertVector3(points[j].normal);
				collision->_data[j]._impulse = ConvertVector3(points[j].impulse);
				collision->_data[j]._separation = points[j].separation;
			}

			if (pairs[i].events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
			{
				collision->_other = actor2->GetUserData();
				actor1->OnCollisionEnter(collision);

				collision->_other = actor1->GetUserData();
				actor2->OnCollisionEnter(collision);
			}

			if (pairs[i].events & PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
			{
				collision->_other = actor2->GetUserData();
				actor1->OnCollisionStay(collision);

				collision->_other = actor1->GetUserData();
				actor2->OnCollisionStay(collision);
			}

			if (pairs[i].events & PxPairFlag::eNOTIFY_TOUCH_LOST)
			{
				collision->_other = actor2->GetUserData();
				actor1->OnCollisionExit(collision);

				collision->_other = actor1->GetUserData();
				actor2->OnCollisionExit(collision);
			}
		}
	}

	void PhysicsEventDispatcher::onTrigger(PxTriggerPair* pairs, PxU32 count)
	{
		PxTriggerPairFlags triggerFlag = PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER | PxTriggerPairFlag::eREMOVED_SHAPE_OTHER;

		for (PxU32 i = 0; i < count; i++)
		{
			if (pairs[i].flags & triggerFlag)
				continue;

			auto* trigger = reinterpret_cast<PhysicsUserData*>(pairs[i].triggerActor->userData);
			auto* other = reinterpret_cast<PhysicsUserData*>(pairs[i].otherActor->userData);

			if (trigger == nullptr || other == nullptr)
				continue;

			auto triggerObj = std::shared_ptr<Trigger>();
			triggerObj->_other = other->GetUserData();

			auto otherObj = std::shared_ptr<Trigger>();
			otherObj->_other = trigger->GetUserData();

			if (pairs[i].status & PxPairFlag::eNOTIFY_TOUCH_FOUND)
			{
				trigger->OnTriggerEnter(triggerObj);
				other->OnTriggerEnter(otherObj);
			}

			if (pairs[i].status & PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
			{
				trigger->OnTriggerStay(triggerObj);
				other->OnTriggerStay(otherObj);
			}

			if (pairs[i].status & PxPairFlag::eNOTIFY_TOUCH_LOST)
			{
				trigger->OnTriggerExit(triggerObj);
				other->OnTriggerExit(otherObj);
			}
		}
	}
}