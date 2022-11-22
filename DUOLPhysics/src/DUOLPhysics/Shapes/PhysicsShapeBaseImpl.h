/**

	@file    PhysicsShapeBaseImpl.h
	@brief   Physics Shape Base�� Implementaion Ŭ����
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
		@brief   Physics Shape Base�� Implementaion Ŭ����
		@details -

	**/
	class PhysicsShapeBase::Impl
	{
		friend PhysicsSystem::Impl;

	public:
		/**
			@brief   Impl Ŭ���� ������
			@details -
		**/
		Impl();

		/**
			@brief   Impl Ŭ���� �Ҹ���
			@details -
		**/
		~Impl();

	private:
		static bool _usePVD;

		PxShape* _shape;

	public:
		/**
			@brief	 Physics Box ����
			@details -
			@param   system    - Box ������ �ʿ��� Physics�� �ǳ����� Ŭ����
			@param   shapeDesc - Box ������ �ʿ��� ��
		**/
		void Create(PhysicsSystem* system, const PxGeometry& geometry, const PhysicsShapeDesc& shapeDesc);

		/**
			@brief	 Physics Box ����
			@details -
			@param   scene     - Box ������ �ʿ��� Physics�� �ǳ����� Ŭ����
			@param   shapeDesc - Box ������ �ʿ��� ��
		**/
		void Create(PhysicsScene* scene, const PxGeometry& geometry, const PhysicsShapeDesc& shapeDesc);

		/**
			@brief	 Shape ����
			@details -
			@param   physics   - Physics ��ü�� ���� ����
			@param   shapeDesc - Shape ������ �ʿ��� ��
		**/
		void Create(PxPhysics* physics, const PxGeometry& geometry, const PhysicsShapeDesc& shapeDesc);

		/**
			@brief	 PxShape Getter
			@details -
			@retval  PxShape ��ü
		**/
		PxShape* GetShape() const;

		/**
			@brief	 Material Setter
			@details -
			@param   material - ��ü�� Material
		**/
		void SetMaterial(const std::weak_ptr<PhysicsMaterial>& material);

		/**
			@brief	 Physics Getter
			@details -
			@param   system - Physics�� ������ system ��ü
			@retval  PxPhysics
		**/
		PxPhysics* GetPhysics(PhysicsSystem* system) const;

		/**
			@brief	 Physics Getter
			@details -
			@param   scene - Physics�� ������ scene ��ü
			@retval  PxPhysics
		**/
		PxPhysics* GetPhysics(PhysicsScene* scene) const;

		/**
			@brief	 Cooking Getter
			@details -
			@param   system - Cooking�� ������ system ��ü
			@retval  PxCooking
		**/
		PxCooking* GetCooking(PhysicsSystem* system) const;

		/**
			@brief	 Cooking Getter
			@details -
			@param   scene - Cooking�� ������ scene ��ü
			@retval  PxCooking
		**/
		PxCooking* GetCooking(PhysicsScene* scene) const;

		/**
			@brief	 Shape �Ҵ� ����
			@details -
		**/
		void Release();
	};
}