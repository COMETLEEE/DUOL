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
			@brief	 Physics Convex Mesh 생성
			@details -
			@param   system    - Convex Mesh 생성에 필요한 Physics를 건내받을 클래스
			@param   shapeDesc - Convex Mesh 생성에 필요한 값
		**/
		void Create(PhysicsSystem* system, const PhysicsShapeDesc& shapeDesc) override;

		/**
			@brief	 Physics Convex Mesh 생성
			@details -
			@param   scene     - Convex Mesh 생성에 필요한 Physics를 건내받을 클래스
			@param   shapeDesc - Convex Mesh 생성에 필요한 값
		**/
		void Create(PhysicsScene* scene, const PhysicsShapeDesc& shapeDesc) override;
	};
}