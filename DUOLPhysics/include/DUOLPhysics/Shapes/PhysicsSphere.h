
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
	class PhysicsSphere : public PhysicsShapeBase
	{
		class Impl;

		friend PhysicsSystem;
		friend PhysicsScene;

	public:
		PhysicsSphere();

		~PhysicsSphere();

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
	};
}