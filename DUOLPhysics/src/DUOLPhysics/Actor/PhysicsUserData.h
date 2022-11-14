/**

	@file    PhysicsUserData.h
	@brief	 UserData Wrapping Ŭ����
	@details �浹 Event�� ����ϰ� �����Ѵ�.
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
		@brief	 UserData Wrapping Ŭ����
		@details -

	**/
	class PhysicsUserData
	{
	public:
		/**
			@brief   PhysicsUserData Ŭ���� ������
			@details -
		**/
		PhysicsUserData();

		/**
			@brief   PhysicsUserData Ŭ���� default �Ҹ���
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
			@brief	 Physics �ܺο� ����� User Data�� Getter
			@details -
			@retval  _userData
		**/
		void* GetUserData() { return _userData; }

		/**
			@brief	 Physics �ܺο� ����� User Data�� Setter
			@details -
			@param   userData - ������ User Data
		**/
		void SetUserData(void* userData) { _userData = userData; }

		/**
			@brief	 Trigger�� Actor�� �浹 �߻� ������ Event
			@details -
			@param   data - �� ��ü�� �浹�� �ٸ� ��ü�� ����
		**/
		void OnTriggerEnter(const std::shared_ptr<Trigger>& data);

		/**
			@brief	 Trigger�� Actor�� �浹 ���� ���� Event
			@details -
			@param   data - �� ��ü�� �浹�� �ٸ� ��ü�� ����
		**/
		void OnTriggerStay(const std::shared_ptr<Trigger>& data);

		/**
			@brief	 Trigger�� Actor�� �浹�� ������ ������ Event
			@details -
			@param   data - �� ��ü�� �浹�� �ٸ� ��ü�� ����
		**/
		void OnTriggerExit(const std::shared_ptr<Trigger>& data);

		/**
			@brief	 Actor�� �浹 �߻� ������ Event
			@details -
			@param   data - �� ��ü�� �浹�� �ٸ� ��ü�� ����
		**/
		void OnCollisionEnter(const std::shared_ptr<Collision>& data);

		/**
			@brief	 Actor�� �浹 ���� ���� Event
			@details -
			@param   data - �� ��ü�� �浹�� �ٸ� ��ü�� ����
		**/
		void OnCollisionStay(const std::shared_ptr<Collision>& data);

		/**
			@brief	 Actor�� �浹�� ������ ������ Event
			@details -
			@param   data - �� ��ü�� �浹�� �ٸ� ��ü�� ����
		**/
		void OnCollisionExit(const std::shared_ptr<Collision>& data);

		/**
			@brief	 Trigger�� Actor�� �浹 �߻� ������ ȣ��Ǵ� Event
			@details -
			@param   enter - Enter Event
		**/
		void SetTriggerEnterEvent(TriggerEvent enter) { _onTriggerEnter = enter; }

		/**
			@brief	 Trigger�� Actor�� �浹 ���� �ÿ� ȣ��Ǵ� Event
			@details -
			@param   stay - Stay Event
		**/
		void SetTriggerStayEvent(TriggerEvent stay) { _onTriggerStay = stay; }

		/**
			@brief	 Trigger�� Actor�� �浹�� ������ ������ ȣ��Ǵ� Event
			@details -
			@param   exit - Exit Event
		**/
		void SetTriggerExitEvent(TriggerEvent exit) { _onTriggerExit = exit; }

		/**
			@brief	 Actor�� �浹 �߻� ������ ȣ��Ǵ� Event
			@details -
			@param   enter - Enter Event
		**/
		void SetCollisionEnterEvent(CollisionEvent enter) { _onCollisionEnter = enter; }

		/**
			@brief	 Actor�� �浹 ���� �ÿ� ȣ��Ǵ� Event
			@details -
			@param   stay - Stay Event
		**/
		void SetCollisionStayEvent(CollisionEvent stay) { _onCollisionStay = stay; }

		/**
			@brief	 Actor�� �浹�� ������ ������ ȣ��Ǵ� Event
			@details -
			@param   exit - Exit Event
		**/
		void SetCollisionExitEvent(CollisionEvent exit) { _onCollisionExit = exit; }
	};
}