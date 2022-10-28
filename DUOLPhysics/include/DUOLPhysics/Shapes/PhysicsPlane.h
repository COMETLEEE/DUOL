/**

	@file    PhysicsPlane.h
	@brief	 Physics Space�� ������ ũ���� ���
	@details -
	@author  JKim
	@date    25.10.2022

**/
#pragma once
/* etc */
#include "../PhysicsDescriptions.h"

#include <memory>

namespace DUOLPhysics
{
	class PhysicsScene;

	class PhysicsPlane
	{
		class Impl;

		friend PhysicsScene;

	public:
		/**
			@brief   PhysicsPlane Ŭ���� ������
			@details -
		**/
		PhysicsPlane();

		/**
			@brief   PhysicsPlane Ŭ���� default �Ҹ���
			@details -
		**/
		~PhysicsPlane() = default;

	private:
		 std::shared_ptr<Impl> _impl;

	public:
		/**
			@brief	 Plane �Ҵ� ����
			@details -
		**/
		void Release();
	};
}