
#pragma once
#ifndef _SILENCE_CXX20_CISO646_REMOVED_WARNING
#define _SILENCE_CXX20_CISO646_REMOVED_WARNING
#endif

#include "PxPhysicsAPI.h"
#include "../Actor/PhysicsUserData.h"

#include <map>
#include <memory>

namespace DUOLPhysics
{
	using namespace physx;

	class PhysicsEventDispatcher : public PxSimulationEventCallback
	{
    private:
        using TriggerStayReceiver = std::pair<PhysicsUserData*, PhysicsUserData*>;

        std::map<TriggerStayReceiver, bool> _triggerStayReciverList;

	private:
        
        void onWake(PxActor** /* actors */, PxU32 /* count */) override {}

        void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 countOfPairs) override;

        void onTrigger(PxTriggerPair* pairs, PxU32 count) override;

        void onConstraintBreak(PxConstraintInfo* /* constraints */, PxU32 /* count */) override {}
        
        void onSleep(PxActor** /* actors */, PxU32 /* count */) override {}
        
        void onAdvance(const PxRigidBody* const* /* bodyBuffer */, const PxTransform* /* poseBuffer */, const PxU32 /* count */) override {}
	
    public:
        void SendTriggerStayEvent();
    };
}