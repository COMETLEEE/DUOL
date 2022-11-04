#pragma once
#include "PhysicsShapeBase.h"

namespace DUOLPhysics
{
	class PhysicsSystem;
	class PhysicsScene;

	/**

		@class   PhysicsBox
		@brief	 Physics Box 도형
		@details -

	**/
	class PhysicsCapsule : public PhysicsShapeBase
	{
		class Impl;

		friend PhysicsSystem;
		friend PhysicsScene;

	public:
		PhysicsCapsule();

		~PhysicsCapsule();

	public:
		/**
			@brief	 Physics Capsule 생성
			@details -
			@param   system    - Capsule 생성에 필요한 Physics를 건내받을 클래스
			@param   shapeDesc - Capsule 생성에 필요한 값
		**/
		void Create(PhysicsSystem* system, const PhysicsShapeDesc& shapeDesc) override;

		/**
			@brief	 Physics Capsule 생성
			@details -
			@param   scene     - Capsule 생성에 필요한 Physics를 건내받을 클래스
			@param   shapeDesc - Capsule 생성에 필요한 값
		**/
		void Create(PhysicsScene* scene, const PhysicsShapeDesc& shapeDesc) override;
	};
}