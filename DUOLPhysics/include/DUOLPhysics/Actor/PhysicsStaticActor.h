/**

	@file    PhysicsStaticActor.h
	@brief	 Physics Space�� ���� ��ü
	@details -
	@author  JKim
	@date    4.11.2022

**/
#pragma once
#include "PhysicsActorBase.h"

#include <memory>

namespace DUOLPhysics
{
	class PhysicsScene;
	class PhysicsShapeBase;

	/**

		@class   PhysicsStaticActor
		@brief	 Physics Space�� ���� ��ü
		@details -

	**/
	class PhysicsStaticActor : public PhysicsActorBase
	{
		class Impl;

		friend PhysicsScene;
		friend PhysicsShapeBase;

	public:
		/**
			@brief   PhysicsStaticActor Ŭ���� ������
			@details ������ ȣ��� Impl ����, �θ𿡰� Impl ������ ����
		**/
		PhysicsStaticActor();

		/**
			@brief   PhysicsStaticActor Ŭ���� �Ҹ���
			@details -
		**/
		~PhysicsStaticActor();

	private:
		std::shared_ptr<Impl> _impl;
	};
}