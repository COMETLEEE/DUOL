/**

	@file    PhysicsSystem.h
	@brief   NVIDIA PhysX Wrapping 클래스
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
#include "../Shapes/PhysicsSphere.h"
#include "../Shapes/PhysicsCapsule.h"

/* etc */
#include "../Util/PhysicsDescriptions.h"
#include "DUOLCommon/StringHelper.h"

#include <map>
#include <memory>
#include <iostream>
#include <type_traits>
#include <string>

#define ERROR_THROW(errStr)				\
{										\
	std::string errTemp = errStr;		\
	errTemp += " / File : ";			\
	errTemp += __FILE__;				\
	errTemp += ", Line : ";				\
	errTemp += std::to_string(__LINE__);\
	throw errTemp;						\
}

namespace DUOLPhysics
{
	using namespace DUOLCommon;

	/**

		@class   PhysicsSystem
		@brief   NVIDIA PhysX Wrapping 클래스
		@details -

	**/
	class PhysicsSystem
	{
		class Impl;

		friend PhysicsShapeBase;

	public:
		/**
			@brief   PhysicsSystem 클래스 생성자
			@details 생성자 호출시 Impl 생성
		**/
		PhysicsSystem();

		/**
			@brief   PhysicsSystem 클래스 소멸자
			@details PhysX와 Scene, Material 해제
		**/
		~PhysicsSystem();

	private:
		std::shared_ptr<Impl> _impl;

		std::map<tstring, std::shared_ptr<PhysicsScene>> _scenes;

		std::map<tstring, std::shared_ptr<PhysicsMaterial>> _materials;

		std::map<tstring, std::shared_ptr<PhysicsShapeBase>> _shapes;

	public:
		/**
			@brief	 PhysX 설정 초기화
			@details -
			@param   desc - PhysX 초기화할 때 필요한 Arguments 목록
			@retval  초기화에 성공하면 true, 아니라면 false
		**/
		bool Init(const PhysicsSystemDesc& desc);

		/**
			@brief	 PhysX 할당 해제
			@details -
		**/
		void Release();

		/**
			@brief	 Physics Scene 생성
			@details -
			@param   keyName   - Scene의 Name
			@param   sceneDesc - Scene 생성에 필요한 값
			@retval  생성된 Scene 객체
		**/
		std::weak_ptr<PhysicsScene> CreateScene(const tstring& keyName, const PhysicsSceneDesc& sceneDesc);

		/**
			@brief	 Physics Material 생성
			@details -
			@param   keyName      - Material의 Name
			@param   materialDesc - Material 생성에 필요한 값
			@retval  생성된 Material 객체
		**/
		std::weak_ptr<PhysicsMaterial> CreateMaterial(const tstring& keyName, const PhysicsMaterialDesc& materialDesc);

		/**
			@brief	 Physics Shape 생성
			@details -
			@tparam  T         - PhysicsShpaeBase를 상속한 Shape들
			@param   keyName   - Shape의 Name
			@param   shapeDesc - Shape 생성에 필요한 값
			@retval  생성된 Shape 객체
		**/
		template<typename T>
		std::weak_ptr<T> CreateShape(const tstring& keyName, const PhysicsShapeDesc& shapeDesc);
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
			std::cerr << errStr << std::endl;
		}
		catch (...)
		{
			std::cerr << "Unknown Error." << std::endl;
		}

		return {};
	}
}