/**

	@file    PhysicsUserData.h
	@brief	 UserData Wrapping 클래스
	@details 충돌 Event를 등록하고 관리한다.
	@author  JKim
	@date    11.11.2022

**/
#pragma once
/* etc */
#include "DUOLPhysics/Util/PhysicsDataStructure.h"

#include <vector>
#include <memory>
#include <functional>

namespace DUOLPhysics
{
	/**

		@class   PhysicsUserData
		@brief	 UserData Wrapping 클래스
		@details -

	**/
	class PhysicsUserData
	{
	public:
		/**
			@brief   PhysicsUserData 클래스 생성자
			@details -
		**/
		PhysicsUserData();

		/**
			@brief   PhysicsUserData 클래스 default 소멸자
			@details -
		**/
		~PhysicsUserData() = default;

	private:
		void* _userData;

		TriggerEvent _onTriggerEnter;

		TriggerEvent _onTriggerStay;

		TriggerEvent _onTriggerExit;

		CollisionEvent _onCollisionEnter;

		CollisionEvent _onCollisionStay;

		CollisionEvent _onCollisionExit;

	public:
		/**
			@brief	 Physics 외부와 연결된 User Data의 Getter
			@details -
			@retval  _userData
		**/
		void* GetUserData() { return _userData; }

		/**
			@brief	 Physics 외부와 연결된 User Data의 Setter
			@details -
			@param   userData - 연결할 User Data
		**/
		void SetUserData(void* userData) { _userData = userData; }

		/**
			@brief	 Trigger와 Actor의 충돌 발생 시점의 Event
			@details -
			@param   data - 이 객체와 충돌한 다른 객체의 정보
		**/
		void OnTriggerEnter(const std::shared_ptr<Trigger>& data);

		/**
			@brief	 Trigger와 Actor의 충돌 지속 시의 Event
			@details -
			@param   data - 이 객체와 충돌한 다른 객체의 정보
		**/
		void OnTriggerStay(const std::shared_ptr<Trigger>& data);

		/**
			@brief	 Trigger와 Actor의 충돌이 끝나는 시점의 Event
			@details -
			@param   data - 이 객체와 충돌한 다른 객체의 정보
		**/
		void OnTriggerExit(const std::shared_ptr<Trigger>& data);

		/**
			@brief	 Actor간 충돌 발생 시점의 Event
			@details -
			@param   data - 이 객체와 충돌한 다른 객체의 정보
		**/
		void OnCollisionEnter(const std::shared_ptr<Collision>& data);

		/**
			@brief	 Actor간 충돌 지속 시의 Event
			@details -
			@param   data - 이 객체와 충돌한 다른 객체의 정보
		**/
		void OnCollisionStay(const std::shared_ptr<Collision>& data);

		/**
			@brief	 Actor간 충돌이 끝나는 시점의 Event
			@details -
			@param   data - 이 객체와 충돌한 다른 객체의 정보
		**/
		void OnCollisionExit(const std::shared_ptr<Collision>& data);

		/**
			@brief	 Trigger와 Actor의 충돌 발생 시점에 호출되는 Event
			@details -
			@param   enter - Enter Event
		**/
		void SetTriggerEnterEvent(TriggerEvent enter) { _onTriggerEnter = enter; }

		/**
			@brief	 Trigger와 Actor의 충돌 지속 시에 호출되는 Event
			@details -
			@param   stay - Stay Event
		**/
		void SetTriggerStayEvent(TriggerEvent stay) { _onTriggerStay = stay; }

		/**
			@brief	 Trigger와 Actor의 충돌이 끝나는 시점에 호출되는 Event
			@details -
			@param   exit - Exit Event
		**/
		void SetTriggerExitEvent(TriggerEvent exit) { _onTriggerExit = exit; }

		/**
			@brief	 Actor간 충돌 발생 시점에 호출되는 Event
			@details -
			@param   enter - Enter Event
		**/
		void SetCollisionEnterEvent(CollisionEvent enter) { _onCollisionEnter = enter; }

		/**
			@brief	 Actor간 충돌 지속 시에 호출되는 Event
			@details -
			@param   stay - Stay Event
		**/
		void SetCollisionStayEvent(CollisionEvent stay) { _onCollisionStay = stay; }

		/**
			@brief	 Actor간 충돌이 끝나는 시점에 호출되는 Event
			@details -
			@param   exit - Exit Event
		**/
		void SetCollisionExitEvent(CollisionEvent exit) { _onCollisionExit = exit; }
	};
}