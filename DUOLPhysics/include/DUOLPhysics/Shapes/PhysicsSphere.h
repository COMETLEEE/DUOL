/**

	@file    PhysicsSphere.h
	@brief	 Physics Sphere ����
	@details -
	@author  JKim
	@date    10.11.2022

**/
#pragma once
#include "PhysicsShapeBase.h"

namespace DUOLPhysics
{
	/**

		@class   PhysicsSphere
		@brief	 Physics Sphere ����
		@details -

	**/
	class PhysicsSphere : public PhysicsShapeBase
	{
	public:
		/**
			@brief   PhysicsSphere Ŭ���� ������
			@details -
		**/
		PhysicsSphere();

		/**
			@brief   PhysicsSphere Ŭ���� default �Ҹ���
			@details -
		**/
		~PhysicsSphere() = default;

	public:
		/**
			@brief	 Physics Sphere ����
			@details -
			@param   system    - Sphere ������ �ʿ��� Physics�� �ǳ����� Ŭ����
			@param   shapeDesc - Sphere ������ �ʿ��� ��
		**/
		void Create(PhysicsSystem* system, const PhysicsShapeDesc& shapeDesc) override;

		/**
			@brief	 Physics Sphere ����
			@details -
			@param   scene     - Sphere ������ �ʿ��� Physics�� �ǳ����� Ŭ����
			@param   shapeDesc - Sphere ������ �ʿ��� ��
		**/
		void Create(PhysicsScene* scene, const PhysicsShapeDesc& shapeDesc) override;

		/**
			@brief	 Sphere�� ũ�� ����
			@details Exclusive�� �����̰ų� Attach���� ���� ���� ��� ����
			@param   radius - ������ ��
		**/
		void SetScale(float radius);
	};
}