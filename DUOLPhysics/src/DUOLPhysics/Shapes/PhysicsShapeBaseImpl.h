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
		PxShape* GetShape();

		/**
			@brief	 Shape �Ҵ� ����
			@details -
		**/
		void Release();
	};
}