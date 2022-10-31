/**

	@file    PhysicsMaterial.h
	@brief   Physics ���꿡 ���Ǵ� ��ü�� Material
	@details -
	@author  JKim
	@date    25.10.2022

**/
#pragma once
/* etc */
#include "PhysicsDescriptions.h"

#include <memory>

namespace DUOLPhysics
{
	class PhysicsSystem;
	class PhysicsScene;
	class PhysicsBox;

	/**

		@class   PhysicsMaterial
		@brief   Physics ���꿡 ���Ǵ� ��ü�� Material
		@details -

	**/
	class PhysicsMaterial
	{
		class Impl;

		friend PhysicsSystem;
		friend PhysicsScene;
		friend PhysicsBox;

	public:
		/**
			@brief   PhysicsMaterial Ŭ���� ������
			@details -
		**/
		PhysicsMaterial();

		/**
			@brief   PhysicsMaterial Ŭ���� default �Ҹ���
			@details -
		**/
		~PhysicsMaterial() = default;

	private:
		std::shared_ptr<Impl> _impl;

	public:
		/**
			@brief   Material �Ҵ� ����
			@details -
		**/
		void Release();
	};
}