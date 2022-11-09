/**

	@file    PhysicsPlane.h
	@brief	 Physics Space�� ������ ũ���� ���
	@details -
	@author  JKim
	@date    25.10.2022

**/
#pragma once
/* etc */
#include "../Util/PhysicsDescriptions.h"

#include <memory>

namespace DUOLPhysics
{
	class PhysicsScene;

	/**

		@class   PhysicsPlane
		@brief	 Physics Space�� ������ ũ���� ���
		@details -

	**/
	class PhysicsPlane
	{
		class Impl;

		friend PhysicsScene;

	public:
		/**
			@brief   PhysicsPlane Ŭ���� ������
			@details ������ ȣ��� Impl ����
		**/
		PhysicsPlane();

		/**
			@brief   PhysicsPlane Ŭ���� default �Ҹ���
			@details -
		**/
		~PhysicsPlane() = default;

	private:
		std::shared_ptr<Impl> _impl;
	};
}