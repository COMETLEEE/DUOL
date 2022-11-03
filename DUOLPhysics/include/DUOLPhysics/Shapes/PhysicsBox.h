/**

	@file    PhysicsBox.h
	@brief	 Physics Box ����
	@details -
	@author  JKim
	@date    31.10.2022

**/
#pragma once
#include "PhysicsShapeBase.h"

#include <memory>

namespace DUOLPhysics
{
	class PhysicsSystem;
	class PhysicsScene;

	/**

		@class   PhysicsBox
		@brief	 Physics Box ����
		@details -

	**/
	class PhysicsBox : public PhysicsShapeBase
	{
		class Impl;

		friend PhysicsSystem;
		friend PhysicsScene;

	private:
		/**
			@brief   PhysicsBox Ŭ���� ������
			@details -
		**/
		PhysicsBox();

	public:
		/**
			@brief   PhysicsBox Ŭ���� �Ҹ���
			@details -
		**/
		~PhysicsBox();

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