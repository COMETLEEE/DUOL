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

	/**

		@class   PhysicsBox
		@brief	 Physics Box ����
		@details -

	**/
	class PhysicsBox : public PhysicsShapeBase
	{
		class Impl;

	public:
		/**
			@brief   PhysicsBox Ŭ���� ������
			@details ������ ȣ��� Impl ����
		**/
		PhysicsBox();

		/**
			@brief   PhysicsBox Ŭ���� �Ҹ���
			@details -
		**/
		~PhysicsBox();

	private:
		std::shared_ptr<Impl> _impl;

	public:
		/**
			@brief	 Physics Box ����
			@details -
			@param   system    - Box ������ �ʿ��� Physics�� �ǳ����� Ŭ����
			@param   shapeDesc - Box ������ �ʿ��� ��
		**/
		void Create(PhysicsSystem* system, const PhysicsShapeDesc& shapeDesc) override;

		/**
			@brief	 Physics Box �Ҵ� ����
			@details -
		**/
		void Release() override;
	};
}