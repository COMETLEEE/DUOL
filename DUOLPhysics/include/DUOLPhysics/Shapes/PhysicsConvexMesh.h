/**

	@file    PhysicsConvexMesh.h
	@brief	 Physics Convex Mesh 도형
	@details -
	@author  JKim
	@date    11.11.2022

**/
#pragma once
#include "PhysicsShapeBase.h"

namespace DUOLPhysics
{
	/**

		@class   PhysicsConvexMesh
		@brief	 Physics Convex Mesh 도형
		@details -

	**/
	class PhysicsConvexMesh : public PhysicsShapeBase
	{
	public:
		/**
			@brief   PhysicsConvexMesh 클래스 생성자
			@details -
		**/
		PhysicsConvexMesh();

		/**
			@brief   PhysicsConvexMesh 클래스 default 소멸자
			@details -
		**/
		~PhysicsConvexMesh() = default;

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