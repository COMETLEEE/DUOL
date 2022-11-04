#pragma once
#include "PhysicsShapeBase.h"

namespace DUOLPhysics
{
	class PhysicsSystem;
	class PhysicsScene;

	/**

		@class   PhysicsBox
		@brief	 Physics Box ����
		@details -

	**/
	class PhysicsCapsule : public PhysicsShapeBase
	{
		class Impl;

		friend PhysicsSystem;
		friend PhysicsScene;

	public:
		PhysicsCapsule();

		~PhysicsCapsule();

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
	};
}