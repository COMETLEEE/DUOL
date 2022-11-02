/**

	@file    PhysicsShapeBase.h
	@brief	 Shape���� �θ� Ŭ����
	@details Shape���� ��� �����ϰ� ������ ������ ���� ��� ���� ���
	@author  JKim
	@date    31.10.2022

**/
#pragma once
#include "../PhysicsDescriptions.h"

namespace DUOLPhysics
{
	class PhysicsSystem;
	class PhysicsDynamicActor;

	/**

		@class   PhysicsShapeBase
		@brief	 Shape���� �θ� Ŭ����
		@details -

	**/
	class PhysicsShapeBase
	{
		friend PhysicsSystem;

	public:
		/**
			@brief	 Shape ����
			@details -
			@param   system    - System�� ���Ե� Physics�� ���� ���� ����
			@param   shapeDesc - Shape ������ �ʿ��� ��
		**/
		virtual void Create(PhysicsSystem* system, const PhysicsShapeDesc& shapeDesc) = 0;

		/**
			@brief
			@details -
			@param   actor -
		**/
		virtual void Attachment(PhysicsDynamicActor* actor) = 0;

		/**
			@brief	 Shape �Ҵ� ����
			@details -
		**/
		virtual void Release() = 0;
	};
}