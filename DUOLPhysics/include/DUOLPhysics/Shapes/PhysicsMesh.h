/**

	@file    PhysicsMesh.h
	@brief	 Physics Triangle Mesh ����
	@details -
	@author  JKim
	@date    11.11.2022

**/
#pragma once
#include "PhysicsShapeBase.h"

namespace DUOLPhysics
{
	/**

		@class   PhysicsMesh
		@brief	 Physics Triangle Mesh ����
		@details -

	**/
	class PhysicsMesh : public PhysicsShapeBase
	{
	public:
		/**
			@brief   PhysicsMesh Ŭ���� ������
			@details -
		**/
		PhysicsMesh();

		/**
			@brief   PhysicsMesh Ŭ���� default �Ҹ���
			@details -
		**/
		~PhysicsMesh() = default;

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