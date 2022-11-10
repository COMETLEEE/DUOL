#pragma once
#include "PhysicsShapeBase.h"

namespace DUOLPhysics
{
	class PhysicsMesh : public PhysicsShapeBase
	{
	public:
		PhysicsMesh();

		~PhysicsMesh();

	public:
		/**
			@brief	 Physics Mesh ����
			@details -
			@param   system    - Mesh ������ �ʿ��� Physics�� �ǳ����� Ŭ����
			@param   shapeDesc - Mesh ������ �ʿ��� ��
		**/
		void Create(PhysicsSystem* system, const PhysicsShapeDesc& shapeDesc) override;

		/**
			@brief	 Physics Mesh ����
			@details -
			@param   scene     - Mesh ������ �ʿ��� Physics�� �ǳ����� Ŭ����
			@param   shapeDesc - Mesh ������ �ʿ��� ��
		**/
		void Create(PhysicsScene* scene, const PhysicsShapeDesc& shapeDesc) override;
	};
}