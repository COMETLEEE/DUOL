
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
	class PhysicsSphere : public PhysicsShapeBase
	{
		class Impl;

		friend PhysicsSystem;
		friend PhysicsScene;

	public:
		PhysicsSphere();

		~PhysicsSphere();

	public:
		/**
			@brief	 Physics Sphere 생성
			@details -
			@param   system    - Sphere 생성에 필요한 Physics를 건내받을 클래스
			@param   shapeDesc - Sphere 생성에 필요한 값
		**/
		void Create(PhysicsSystem* system, const PhysicsShapeDesc& shapeDesc) override;

		/**
			@brief	 Physics Sphere 생성
			@details -
			@param   scene     - Sphere 생성에 필요한 Physics를 건내받을 클래스
			@param   shapeDesc - Sphere 생성에 필요한 값
		**/
		void Create(PhysicsScene* scene, const PhysicsShapeDesc& shapeDesc) override;
	};
}