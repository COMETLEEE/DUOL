/**

	@file    PhysicsEventDispatcher.h
	@brief	 Physics Space의 충돌 Event Dispatcher
	@details -
	@author  JKim
	@date    11.11.2022

**/
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

	/**

		@class   PhysicsEventDispatcher
		@brief	 Physics Space의 충돌 Event Dispatcher
		@details -

	**/
	class PhysicsEventDispatcher : public PxSimulationEventCallback
	{
		using TriggerStayReceiver = std::pair<PhysicsUserData*, PhysicsUserData*>;

	private:
		std::map<TriggerStayReceiver, bool> _triggerStayReciverList;

	private:
		/**
			@brief	 Actor가 Enable될 때 호출
			@details 현재는 사용하지 않음 필요할 때 구현
			@param    -
			@param    -
		**/
		void onWake(PxActor** /* actors */, PxU32 /* count */) override {}

		/**
			@brief	 Actor끼리 충돌이 발생했을 때 호출
			@details -
			@param   pairHeader   - 충돌한 두 Actor
			@param   pairs        - 충돌 지점에 대한 정보 배열
			@param   countOfPairs - pairs 배열 길이
		**/
		void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 countOfPairs) override;

		/**
			@brief	 Actor와 Trigger끼리 충돌이 발생했을 때 호출
			@details -
			@param   pairs - 충돌한 Actor와 Trigger의 정보 배열
			@param   count - pairs의 배열 길이
		**/
		void onTrigger(PxTriggerPair* pairs, PxU32 count) override;

		/**
			@brief	 제약 조건을 걸어놓고 제약이 깨질 경우 호출
			@details 현재는 사용하지 않음 필요할 때 구현
			@param    -
			@param    -
		**/
		void onConstraintBreak(PxConstraintInfo* /* constraints */, PxU32 /* count */) override {}

		/**
			@brief	 Actor가 Disable될 때 호출
			@details 현재는 사용하지 않음 필요할 때 구현
			@param    -
			@param    -
		**/
		void onSleep(PxActor** /* actors */, PxU32 /* count */)override {}

		/**
			@brief	 FetchResult와 별개로 위치 변경이 발생한 Actor의 정보를 전달
			@details 현재는 사용하지 않음 필요할 때 구현
			@param    -
			@param    -
			@param    -
		**/
		void onAdvance(const PxRigidBody* const* /* bodyBuffer */, const PxTransform* /* poseBuffer */, const PxU32 /* count */) override {}

	public:
		/**
			@brief	 Trigger Enter 이후 Exit되지 않은 모든 Pairs에게 Stay 상태 호출
			@details -
		**/
		void SendTriggerStayEvent();

		void DeleteTriggerStayUserData(PhysicsUserData* userData);
	};
}