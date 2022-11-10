#pragma once
#include "PhysicsShapeBase.h"

namespace DUOLPhysics
{
	class PhysicsConvexMesh : public PhysicsShapeBase
	{
	public:
		PhysicsConvexMesh();

		~PhysicsConvexMesh();

	public:
		/**
			@brief	 Physics Convex Mesh ����
			@details -
			@param   system    - Convex Mesh ������ �ʿ��� Physics�� �ǳ����� Ŭ����
			@param   shapeDesc - Convex Mesh ������ �ʿ��� ��
		**/
		void Create(PhysicsSystem* system, const PhysicsShapeDesc& shapeDesc) override;

		/**
			@brief	 Physics Convex Mesh ����
			@details -
			@param   scene     - Convex Mesh ������ �ʿ��� Physics�� �ǳ����� Ŭ����
			@param   shapeDesc - Convex Mesh ������ �ʿ��� ��
		**/
		void Create(PhysicsScene* scene, const PhysicsShapeDesc& shapeDesc) override;
	};
}