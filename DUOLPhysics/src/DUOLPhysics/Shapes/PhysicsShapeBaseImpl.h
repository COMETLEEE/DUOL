/**

	@file    PhysicsShapeBaseImpl.h
	@brief   Physics Shape Base의 Implementaion 클래스
	@details -
	@author  JKim
	@date    3.11.2022

**/
#pragma once
#ifndef _SILENCE_CXX20_CISO646_REMOVED_WARNING
#define _SILENCE_CXX20_CISO646_REMOVED_WARNING
#endif

#include "PxPhysicsAPI.h"
#include "DUOLPhysics/Shapes/PhysicsShapeBase.h"
#include "../System/PhysicsSystemImpl.h"

namespace DUOLPhysics
{
	using namespace physx;

	/**

		@class   Impl
		@brief   Physics Shape Base의 Implementaion 클래스
		@details -

	**/
	class PhysicsShapeBase::Impl
	{
		friend PhysicsSystem::Impl;

	public:
		/**
			@brief   Impl 클래스 생성자
			@details -
		**/
		Impl();

		/**
			@brief   Impl 클래스 소멸자
			@details -
		**/
		~Impl();

	private:
		static bool _usePVD;

		PxShape* _shape;

	public:
		/**
			@brief	 Physics Box 생성
			@details -
			@param   system    - Box 생성에 필요한 Physics를 건내받을 클래스
			@param   shapeDesc - Box 생성에 필요한 값
		**/
		void Create(PhysicsSystem* system, const PxGeometry& geometry, const PhysicsShapeDesc& shapeDesc);

		/**
			@brief	 Physics Box 생성
			@details -
			@param   scene     - Box 생성에 필요한 Physics를 건내받을 클래스
			@param   shapeDesc - Box 생성에 필요한 값
		**/
		void Create(PhysicsScene* scene, const PxGeometry& geometry, const PhysicsShapeDesc& shapeDesc);

		/**
			@brief	 Shape 생성
			@details -
			@param   physics   - Physics 객체를 통해 생성
			@param   shapeDesc - Shape 생성에 필요한 값
		**/
		void Create(PxPhysics* physics, const PxGeometry& geometry, const PhysicsShapeDesc& shapeDesc);

		/**
			@brief	 PxShape Getter
			@details -
			@retval  PxShape 객체
		**/
		PxShape* GetShape() const;

		/**
			@brief	 Material Setter
			@details -
			@param   material - 교체할 Material
		**/
		void SetMaterial(const std::weak_ptr<PhysicsMaterial>& material);

		/**
			@brief	 Physics Getter
			@details -
			@param   system - Physics를 가져올 system 객체
			@retval  PxPhysics
		**/
		PxPhysics* GetPhysics(PhysicsSystem* system) const;

		/**
			@brief	 Physics Getter
			@details -
			@param   scene - Physics를 가져올 scene 객체
			@retval  PxPhysics
		**/
		PxPhysics* GetPhysics(PhysicsScene* scene) const;

		/**
			@brief	 Cooking Getter
			@details -
			@param   system - Cooking을 가져올 system 객체
			@retval  PxCooking
		**/
		PxCooking* GetCooking(PhysicsSystem* system) const;

		/**
			@brief	 Cooking Getter
			@details -
			@param   scene - Cooking을 가져올 scene 객체
			@retval  PxCooking
		**/
		PxCooking* GetCooking(PhysicsScene* scene) const;

		/**
			@brief	 Shape 할당 해제
			@details -
		**/
		void Release();
	};
}