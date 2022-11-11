/**

	@file    PhysicsConvexMesh.h
	@brief	 Physics Convex Mesh ����
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
		@brief	 Physics Convex Mesh ����
		@details -

	**/
	class PhysicsConvexMesh : public PhysicsShapeBase
	{
	public:
		/**
			@brief   PhysicsConvexMesh Ŭ���� ������
			@details -
		**/
		PhysicsConvexMesh();

		/**
			@brief   PhysicsConvexMesh Ŭ���� default �Ҹ���
			@details -
		**/
		~PhysicsConvexMesh() = default;

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