#include "PhysicsSimulationFilterCallback.h"

namespace DUOLPhysics
{
	PxFilterFlags PhysicsSimulationFilterCallback::pairFound(PxU32 pairID, PxFilterObjectAttributes attributes0,
		PxFilterData filterData0, const PxActor* a0, const PxShape* s0, PxFilterObjectAttributes attributes1,
		PxFilterData filterData1, const PxActor* a1, const PxShape* s1, PxPairFlags& pairFlags)
	{
		auto flag = PxFilterFlag::eNOTIFY;
		return flag;
	}

	void PhysicsSimulationFilterCallback::pairLost(PxU32 pairID, PxFilterObjectAttributes attributes0,
		PxFilterData filterData0, PxFilterObjectAttributes attributes1, PxFilterData filterData1, bool objectRemoved)
	{
	}
}
