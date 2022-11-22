/**

	@file    PhysicsSystem.h
	@brief   NVIDIA PhysX Wrapping Ŭ����
	@details -
	@author  JKim
	@date    21.10.2022

**/
#pragma once
/* Scene */
#include "../Scene/PhysicsScene.h"

/* Material */
#include "../Material/PhysicsMaterial.h"

/* Shapes */
#include "../Shapes/PhysicsBox.h"
#include "../Shapes/PhysicsMesh.h"
#include "../Shapes/PhysicsPlane.h"
#include "../Shapes/PhysicsCapsule.h"
#include "../Shapes/PhysicsConvexMesh.h"

/* etc */
#include "DUOLCommon/StringHelper.h"
#include "../Util/PhysicsDefines.h"
#include "../Util/PhysicsDescriptions.h"

#include <map>
#include <memory>
#include <iostream>
#include <type_traits>

namespace DUOLPhysics
{
	using namespace DUOLCommon;

	/**

		@class   PhysicsSystem
		@brief   NVIDIA PhysX Wrapping Ŭ����
		@details -

	**/
	class PhysicsSystem
	{
		class Impl;

		friend PhysicsShapeBase;

	public:
		/**
			@brief   PhysicsSystem Ŭ���� ������
			@details ������ ȣ��� Impl ����
		**/
		PhysicsSystem();

		/**
			@brief   PhysicsSystem Ŭ���� �Ҹ���
			@details PhysX�� Scene, Material ����
		**/
		~PhysicsSystem();

	private:
		std::shared_ptr<Impl> _impl;

		std::map<tstring, std::shared_ptr<PhysicsScene>> _scenes;

		std::map<tstring, std::shared_ptr<PhysicsMaterial>> _materials;

		std::map<tstring, std::shared_ptr<PhysicsShapeBase>> _shapes;

	public:
		/**
			@brief	 PhysX ���� �ʱ�ȭ
			@details -
			@param   desc - PhysX �ʱ�ȭ�� �� �ʿ��� Arguments ���
			@retval  �ʱ�ȭ�� �����ϸ� true, �ƴ϶�� false
		**/
		bool Init(const PhysicsSystemDesc& desc);

		/**
			@brief	 PhysX �Ҵ� ����
			@details -
		**/
		void Release();

		/**
			@brief	 Physics Scene ����
			@details -
			@param   keyName   - Scene�� Name
			@param   sceneDesc - Scene ������ �ʿ��� ��
			@retval  ������ Scene ��ü
		**/
		std::weak_ptr<PhysicsScene> CreateScene(const tstring& keyName, const PhysicsSceneDesc& sceneDesc);

		/**
			@brief	 Physics Material ����
			@details -
			@param   keyName      - Material�� Name
			@param   materialDesc - Material ������ �ʿ��� ��
			@retval  ������ Material ��ü
		**/
		std::weak_ptr<PhysicsMaterial> CreateMaterial(const tstring& keyName, const PhysicsMaterialDesc& materialDesc);

		/**
			@brief	 Physics Shape ����
			@details -
			@tparam  T         - PhysicsShpaeBase�� ����� Shape��
			@param   keyName   - Shape�� Name
			@param   shapeDesc - Shape ������ �ʿ��� ��
			@retval  ������ Shape ��ü
		**/
		template<typename T>
		std::weak_ptr<T> CreateShape(const tstring& keyName, const PhysicsShapeDesc& shapeDesc);

		/**
			@brief	 Scene ����
			@details -
			@param   keyName - Scene�� Name
			@retval  Scene�� ���������� ���ŵǾ����� true �ƴϸ� false
		**/
		bool DestroyScene(const tstring& keyName);

		/**
			@brief	 Material ����
			@details -
			@param   keyName - Material�� Name
			@retval  Material�� ���������� ���ŵǾ����� true �ƴϸ� false
		**/
		bool DestroyMaterial(const tstring& keyName);

		/**
			@brief	 Shape ����
			@details -
			@param   keyName - Shape�� Name
			@retval  Shape�� ���������� ���ŵǾ����� true �ƴϸ� false
		**/
		bool DestroyShape(const tstring& keyName);
	};

	template<typename T>
	std::weak_ptr<T> PhysicsSystem::CreateShape(const tstring& keyName, const PhysicsShapeDesc& shapeDesc)
	{
		static_assert(std::is_base_of<PhysicsShapeBase, T>::value, "Shape must inherit PhysicsShapeBase.");

		try
		{
			if (_impl == nullptr)
				ERROR_THROW("No Implementation was generated.");

			auto result = _shapes.find(keyName);

			if (result != _shapes.end())
				return std::dynamic_pointer_cast<T>(result->second);

			auto newShape = std::make_shared<T>();
			newShape->Create(this, shapeDesc);

			_shapes[keyName] = newShape;

			return newShape;
		}
		catch (const std::string& errStr)
		{
			DUOL_ENGINE_ERROR(errStr.c_str());
		}
		catch (...)
		{
			DUOL_ENGINE_ERROR("Unknown Error.");
		}

		return {};
	}
}