/**

	@file    PhysicsBox.h
	@brief	 Physics Box ����
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
		@brief	 Physics Box ����
		@details -

	**/
	class PhysicsBox : public PhysicsShapeBase
	{
	public:
		/**
			@brief   PhysicsBox Ŭ���� ������
			@details -
		**/
		PhysicsBox();

		/**
			@brief   PhysicsBox Ŭ���� default �Ҹ���
			@details -
		**/
		~PhysicsBox() = default;

	public:
		/**
			@brief	 Physics Box ����
			@details -
			@param   system    - Box ������ �ʿ��� Physics�� �ǳ����� Ŭ����
			@param   shapeDesc - Box ������ �ʿ��� ��
		**/
		void Create(PhysicsSystem* system, const PhysicsShapeDesc& shapeDesc) override;

		/**
			@brief	 Physics Box ����
			@details -
			@param   scene     - Box ������ �ʿ��� Physics�� �ǳ����� Ŭ����
			@param   shapeDesc - Box ������ �ʿ��� ��
		**/
		void Create(PhysicsScene* scene, const PhysicsShapeDesc& shapeDesc) override;
	};
}