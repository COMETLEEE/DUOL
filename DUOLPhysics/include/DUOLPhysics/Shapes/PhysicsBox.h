/**

	@file    PhysicsBox.h
	@brief	 Physics Box 도형
	@details -
	@author  JKim
	@date    31.10.2022

**/
#pragma once
#include "PhysicsShapeBase.h"

namespace DUOLPhysics
{
	/**

		@class   PhysicsBox
		@brief	 Physics Box 도형
		@details -

	**/
	class PhysicsBox : public PhysicsShapeBase
	{
	public:
		/**
			@brief   PhysicsBox 클래스 생성자
			@details -
		**/
		PhysicsBox();

		/**
			@brief   PhysicsBox 클래스 default 소멸자
			@details -
		**/
		~PhysicsBox() = default;

	public:
		/**
			@brief	 Physics Box 생성
			@details -
			@param   system    - Box 생성에 필요한 Physics를 건내받을 클래스
			@param   shapeDesc - Box 생성에 필요한 값
		**/
		void Create(PhysicsSystem* system, const PhysicsShapeDesc& shapeDesc) override;

		/**
			@brief	 Physics Box 생성
			@details -
			@param   scene     - Box 생성에 필요한 Physics를 건내받을 클래스
			@param   shapeDesc - Box 생성에 필요한 값
		**/
		void Create(PhysicsScene* scene, const PhysicsShapeDesc& shapeDesc) override;

		/**
			@brief	 Box의 크기 변경
			@details -
			@param   halfExtentX - 가로의 절반 값
			@param   halfExtentY - 높이의 절반 값
			@param   halfExtentZ - 세로의 절반 값
		**/
		void SetScale(float halfExtentX, float halfExtentY, float halfExtentZ);

		/**
			@brief	 Box의 크기 변경
			@details -
			@param   halfScale - x, y, z Half값
		**/
		void SetScale(const DUOLMath::Vector3& halfScale);
	};
}