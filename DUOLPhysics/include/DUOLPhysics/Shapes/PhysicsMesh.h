#pragma once
#include "PhysicsShapeBase.h"

namespace DUOLPhysics
{
	class PhysicsMesh : public PhysicsShapeBase
	{
	public:
		PhysicsMesh();

		~PhysicsMesh();

	public:
		/**
			@brief	 Physics Mesh 생성
			@details -
			@param   system    - Mesh 생성에 필요한 Physics를 건내받을 클래스
			@param   shapeDesc - Mesh 생성에 필요한 값
		**/
		void Create(PhysicsSystem* system, const PhysicsShapeDesc& shapeDesc) override;

		/**
			@brief	 Physics Mesh 생성
			@details -
			@param   scene     - Mesh 생성에 필요한 Physics를 건내받을 클래스
			@param   shapeDesc - Mesh 생성에 필요한 값
		**/
		void Create(PhysicsScene* scene, const PhysicsShapeDesc& shapeDesc) override;
	};
}