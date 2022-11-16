/**

	@file    PhysicsCapsule.h
	@brief	 Physics Capsule ����
	@details -
	@author  JKim
	@date    10.11.2022

**/
#pragma once
#include "PhysicsShapeBase.h"

namespace DUOLPhysics
{
	/**

		@class   PhysicsCapsule
		@brief	 Physics Capsule ����
		@details -

	**/
	class PhysicsCapsule : public PhysicsShapeBase
	{
	public:
		/**
			@brief   PhysicsCapsule Ŭ���� ������
			@details -
		**/
		PhysicsCapsule();

		/**
			@brief   PhysicsCapsule Ŭ���� default �Ҹ���
			@details -
		**/
		~PhysicsCapsule() = default;

	public:
		/**
			@brief	 Physics Capsule ����
			@details -
			@param   system    - Capsule ������ �ʿ��� Physics�� �ǳ����� Ŭ����
			@param   shapeDesc - Capsule ������ �ʿ��� ��
		**/
		void Create(PhysicsSystem* system, const PhysicsShapeDesc& shapeDesc) override;

		/**
			@brief	 Physics Capsule ����
			@details -
			@param   scene     - Capsule ������ �ʿ��� Physics�� �ǳ����� Ŭ����
			@param   shapeDesc - Capsule ������ �ʿ��� ��
		**/
		void Create(PhysicsScene* scene, const PhysicsShapeDesc& shapeDesc) override;

		void SetScale(float radius, float halfHeight);
	};
}