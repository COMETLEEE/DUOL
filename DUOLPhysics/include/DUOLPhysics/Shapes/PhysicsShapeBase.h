/**

	@file    PhysicsShapeBase.h
	@brief	 Shape���� �θ� Ŭ����
	@details Shape���� ��� �����ϰ� ������ ������ ���� ��� ���� ���
	@author  JKim
	@date    31.10.2022

**/
#pragma once
#include "../PhysicsDescriptions.h"

#include <memory>

namespace DUOLPhysics
{
	class PhysicsSystem;
	class PhysicsScene;
	class PhysicsDynamicActor;

	/**

		@class   PhysicsShapeBase
		@brief	 Shape���� �θ� Ŭ����
		@details -

	**/
	class PhysicsShapeBase
	{
	protected:
		class Impl;

	private:
		friend PhysicsSystem;
		friend PhysicsDynamicActor;

	protected:
		/**
			@brief   PhysicsShapeBase Ŭ���� ������
			@details ������ ȣ��� Impl ����
		**/
		PhysicsShapeBase();

		/**
			@brief   PhysicsShapeBase Ŭ���� �Ҹ���
			@details -
		**/
		~PhysicsShapeBase();

	protected:
		std::shared_ptr<Impl> _impl;

	public:
		/**
			@brief	 Shape ����
			@details -
			@param   system    - System�� ���Ե� Physics�� ���� ���� ����
			@param   shapeDesc - Shape ������ �ʿ��� ��
		**/
		virtual void Create(PhysicsSystem* system, const PhysicsShapeDesc& shapeDesc) = 0;

		/**
			@brief	 Shape ����
			@details Scene���� �����ϰ� �ٷ� Actor�� ���� �� ������
			@param   scene    - Scene�� ���Ե� Physics�� ���� ���� ����
			@param   shapeDesc - Shape ������ �ʿ��� ��
		**/
		virtual void Create(PhysicsScene* scene, const PhysicsShapeDesc& shapeDesc) = 0;

		/**
			@brief	 Actor�� ����
			@details -
			@param   actor - ������ Actor
		**/
		virtual void Attachment(PhysicsDynamicActor* actor) final;

		/**
			@brief	 Actor���� Ż��
			@details -
			@param   actor             - Ż���� Actor
			@param   isWakeOnLostTouch -
		**/
		virtual void Detachment(PhysicsDynamicActor* actor, bool isWakeOnLostTouch) final;

		/**
			@brief	 Shape �Ҵ� ����
			@details -
		**/
		virtual void Release() final;
	};
}