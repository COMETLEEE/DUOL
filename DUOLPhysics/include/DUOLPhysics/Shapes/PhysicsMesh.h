/**

	@file    PhysicsMesh.h
	@brief	 Physics Triangle Mesh 도형
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
		@brief	 Physics Triangle Mesh 도형
		@details -

	**/
	class PhysicsMesh : public PhysicsShapeBase
	{
	public:
		/**
			@brief   PhysicsMesh 클래스 생성자
			@details -
		**/
		PhysicsMesh();

		/**
			@brief   PhysicsMesh 클래스 default 소멸자
			@details -
		**/
		~PhysicsMesh() = default;

	public:
		/**
			@brief	 Physics Mesh 생성
			@details -
			@param   system    - Mesh 생성에 필요한 Physics를 건내받을 클래스
			@param   shapeDesc - Mesh 생성에 필요한 값
		**/
		void Create(PhysicsSystem* system, const PhysicsShapeDesc& shapeDesc) override;

		/**
			@brief	 Physics Mesh 생성
			@details -
			@param   scene     - Mesh 생성에 필요한 Physics를 건내받을 클래스
			@param   shapeDesc - Mesh 생성에 필요한 값
		**/
		void Create(PhysicsScene* scene, const PhysicsShapeDesc& shapeDesc) override;
	};
}