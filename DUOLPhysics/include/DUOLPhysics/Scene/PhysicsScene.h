/**

	@file    PhysicsScene.h
	@brief	 Physics 연산하는 Scene 공간
	@details -
	@author  JKim
	@date    24.10.2022

**/
#pragma once
/* Actor */
#include "../Actor/PhysicsDynamicActor.h"

/* Shapes */
#include "../Shapes/PhysicsPlane.h"
#include "../Shapes/PhysicsShapeBase.h"

/* Material */
#include "../PhysicsMaterial.h"

/* etc */
#include "../PhysicsDescriptions.h"
#include "DUOLCommon/StringHelper.h"

#include <map>
#include <memory>

namespace DUOLPhysics
{
	using namespace DUOLCommon;

	class PhysicsSystem;

	/**

		@class   PhysicsScene
		@brief	 Physics 연산하는 Scene 공간
		@details -

	**/
	class PhysicsScene
	{
		class Impl;

		friend PhysicsSystem;

	public:
		/**
			@brief   PhysicsScene 클래스 생성자
			@details 생성자 호출시 Impl 생성
		**/
		PhysicsScene();

		/**
			@brief   PhysicsScene 클래스 default 소멸자
			@details -
		**/
		~PhysicsScene() = default;

	private:
		std::shared_ptr<Impl> _impl;

		std::map<tstring, std::shared_ptr<PhysicsPlane>> _planes;

		std::map<tstring, std::shared_ptr<PhysicsDynamicActor>> _dynamicActors;

	public:
		/**
			@brief	 Plane 생성
			@details -
			@param   keyName   - Plane의 Name
			@param   material  - Plane에 적용될 Material
			@param   PlaneDesc - Plane 생성에 필요한 값
			@retval  생성된 Plane 객체
		**/
		std::weak_ptr<PhysicsPlane> CreatePlane(const tstring& keyName, std::weak_ptr<PhysicsMaterial> material, const PhysicsPlaneDesc& PlaneDesc);

		/**
			@brief	 Dynamic Actor 생성
			@details -
			@param   keyName     - Actor의 Name
			@param   dynamicDesc - Dynamic Actor 생성에 필요한 값
			@retval  생성된 Dynamic Actor 객체
		**/
		std::weak_ptr<PhysicsDynamicActor> CreateDynamicActor(const tstring& keyName, const PhysicsDynamicDesc& dynamicDesc);

		/**
			@brief
			@details -
			@param   deltaTime -
		**/
		void Simulate(float deltaTime);

		/**
			@brief	 Scene 할당 해제
			@details -
		**/
		void Release();
	};
}