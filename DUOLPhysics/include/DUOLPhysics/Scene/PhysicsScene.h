/**

	@file    PhysicsScene.h
	@brief	 Physics �����ϴ� Scene ����
	@details -
	@author  JKim
	@date    24.10.2022

**/
#pragma once
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
		@brief	 Physics �����ϴ� Scene ����
		@details -

	**/
	class PhysicsScene
	{
		class Impl;

		friend PhysicsSystem;

	public:
		/**
			@brief   PhysicsScene Ŭ���� ������
			@details ������ ȣ��� Impl ����
		**/
		PhysicsScene();

		/**
			@brief   PhysicsScene Ŭ���� default �Ҹ���
			@details -
		**/
		~PhysicsScene() = default;

	private:
		std::shared_ptr<Impl> _impl;

		std::map<tstring, std::shared_ptr<PhysicsPlane>> _planes;

		std::map<tstring, std::shared_ptr<PhysicsShapeBase>> _shapes;

	public:
		/**
			@brief	 Plane ����
			@details -
			@param   keyName   - Plane�� Name
			@param   material  - Plane�� ����� Material
			@param   PlaneDesc - Plane ������ �ʿ��� ��
			@retval  ������ Plane ��ü
		**/
		std::weak_ptr<PhysicsPlane> CreatePlane(const tstring& keyName, std::weak_ptr<PhysicsMaterial> material, const PhysicsPlaneDesc& PlaneDesc);

		/**
			@brief	 Scene �Ҵ� ����
			@details -
		**/
		void Release();
	};
}