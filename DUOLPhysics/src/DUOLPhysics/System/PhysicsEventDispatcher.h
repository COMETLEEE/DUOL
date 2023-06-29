/**

	@file    PhysicsEventDispatcher.h
	@brief	 Physics Space�� �浹 Event Dispatcher
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
		@brief	 Physics Space�� �浹 Event Dispatcher
		@details -

	**/
	class PhysicsEventDispatcher : public PxSimulationEventCallback
	{
		using TriggerStayReceiver = std::pair<PhysicsUserData*, PhysicsUserData*>;

	private:
		std::map<TriggerStayReceiver, bool> _triggerStayReciverList;

	private:
		/**
			@brief	 Actor�� Enable�� �� ȣ��
			@details ����� ������� ���� �ʿ��� �� ����
			@param    -
			@param    -
		**/
		void onWake(PxActor** /* actors */, PxU32 /* count */) override {}

		/**
			@brief	 Actor���� �浹�� �߻����� �� ȣ��
			@details -
			@param   pairHeader   - �浹�� �� Actor
			@param   pairs        - �浹 ������ ���� ���� �迭
			@param   countOfPairs - pairs �迭 ����
		**/
		void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 countOfPairs) override;

		/**
			@brief	 Actor�� Trigger���� �浹�� �߻����� �� ȣ��
			@details -
			@param   pairs - �浹�� Actor�� Trigger�� ���� �迭
			@param   count - pairs�� �迭 ����
		**/
		void onTrigger(PxTriggerPair* pairs, PxU32 count) override;

		/**
			@brief	 ���� ������ �ɾ���� ������ ���� ��� ȣ��
			@details ����� ������� ���� �ʿ��� �� ����
			@param    -
			@param    -
		**/
		void onConstraintBreak(PxConstraintInfo* /* constraints */, PxU32 /* count */) override {}

		/**
			@brief	 Actor�� Disable�� �� ȣ��
			@details ����� ������� ���� �ʿ��� �� ����
			@param    -
			@param    -
		**/
		void onSleep(PxActor** /* actors */, PxU32 /* count */)override {}

		/**
			@brief	 FetchResult�� ������ ��ġ ������ �߻��� Actor�� ������ ����
			@details ����� ������� ���� �ʿ��� �� ����
			@param    -
			@param    -
			@param    -
		**/
		void onAdvance(const PxRigidBody* const* /* bodyBuffer */, const PxTransform* /* poseBuffer */, const PxU32 /* count */) override {}

	public:
		/**
			@brief	 Trigger Enter ���� Exit���� ���� ��� Pairs���� Stay ���� ȣ��
			@details -
		**/
		void SendTriggerStayEvent();

		void DeleteTriggerStayUserData(PhysicsUserData* userData);
	};
}