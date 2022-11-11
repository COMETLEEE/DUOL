/**

	@file    PhysicsScene.h
	@brief	 Physics 연산하는 Scene 공간
	@details -
	@author  JKim
	@date    24.10.2022

**/
#pragma once
/* Actor */
#include "../Actor/PhysicsStaticActor.h"
#include "../Actor/PhysicsDynamicActor.h"

/* Shapes */
#include "../Shapes/PhysicsBox.h"
#include "../Shapes/PhysicsMesh.h"
#include "../Shapes/PhysicsPlane.h"
#include "../Shapes/PhysicsCapsule.h"
#include "../Shapes/PhysicsConvexMesh.h"

/* Material */
#include "../Material/PhysicsMaterial.h"

/* etc */
#include "DUOLCommon/StringHelper.h"
#include "../Util/PhysicsDescriptions.h"
#include "../Util/PhysicsDataStructure.h"

#include <map>
#include <memory>
#include <iostream>

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
		friend PhysicsShapeBase;

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

		std::map<tstring, std::shared_ptr<PhysicsStaticActor>> _staticActors;

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
			@brief	 Static Actor 생성
			@details -
			@param   keyName    - Static Actor의 Name
			@param   staticDesc - Static Actor 생성에 필요한 값
			@retval  생성된 Static Actor 객체
		**/
		std::weak_ptr<PhysicsStaticActor> CreateStaticActor(const tstring& keyName, const PhysicsActorDesc& staticDesc);

		/**
			@brief	 Static Actor 생성
			@details 생성과 동시에 Shape를 같이 생성하고 부착
			@tparam  T          - 생성하고 부착할 Shape Type
			@param   keyName    - Static Actor의 Name
			@param   staticDesc - Static Actor 생성에 필요한 값
			@param   shapeDesc  - Shape 생성에 필요한 값
			@retval  생성된 Static Actor 객체
		**/
		template<class T>
		std::weak_ptr<PhysicsStaticActor> CreateStaticActor(const tstring& keyName, const PhysicsActorDesc& staticDesc, const PhysicsShapeDesc& shapeDesc);

		/**
			@brief	 Dynamic Actor 생성
			@details -
			@param   keyName     - Dynamic Actor의 Name
			@param   dynamicDesc - Dynamic Actor 생성에 필요한 값
			@retval  생성된 Dynamic Actor 객체
		**/
		std::weak_ptr<PhysicsDynamicActor> CreateDynamicActor(const tstring& keyName, const PhysicsActorDesc& dynamicDesc);

		/**
			@brief	 Dynamic Actor 생성
			@details 생성과 동시에 Shape를 같이 생성하고 부착
			@tparam  T           - 생성하고 부착할 Shape Type
			@param   keyName     - Dynamic Actor의 Name
			@param   dynamicDesc - Dynamic Actor 생성에 필요한 값
			@param   shapeDesc   - Shape 생성에 필요한 값
			@retval  생성된 Dynamic Actor 객체
		**/
		template<class T>
		std::weak_ptr<PhysicsDynamicActor> CreateDynamicActor(const tstring& keyName, const PhysicsActorDesc& dynamicDesc, const PhysicsShapeDesc& shapeDesc);

		/**
			@brief	 Static Actor 제거
			@details -
			@param   keyName - Actor의 Name
			@retval  Static Actor가 성공적으로 제거되었으면 true 아니면 false
		**/
		bool DestroyStaticActor(const tstring& keyName);

		/**
			@brief	 Dynamic Actor 제거
			@details -
			@param   keyName - Actor의 Name
			@retval  Dynamic Actor가 성공적으로 제거되었으면 true 아니면 false
		**/
		bool DestroyDynamicActor(const tstring& keyName);

		/**
			@brief
			@details -
			@param   position    -
			@param   direction   -
			@param   maxDistance -
			@retval              -
		**/
		RaycastHit Raycast(const DUOLMath::Vector3& position, const DUOLMath::Vector3& direction, float maxDistance);

		/**
			@brief	 Scene에서 생성된 Actor간의 연산을 진행
			@details -
			@param   deltaTime - Frame 사이의 흐른 시간
		**/
		void Simulate(float deltaTime);

		/**
			@brief	 Scene 할당 해제
			@details -
		**/
		void Release();
	};

	template<class T>
	inline std::weak_ptr<PhysicsStaticActor> PhysicsScene::CreateStaticActor(const tstring& keyName, const PhysicsActorDesc& staticDesc, const PhysicsShapeDesc& shapeDesc)
	{
		static_assert(std::is_base_of<PhysicsShapeBase, T>::value, "Shape must inherit PhysicsShapeBase.");

		try
		{
			if (_impl == nullptr)
				ERROR_THROW("No Implementation was generated.");

			auto result = _staticActors.find(keyName);

			if (result != _staticActors.end())
				return std::dynamic_pointer_cast<T>(result->second);

			auto newShape = std::make_shared<T>();
			newShape->Create(this, shapeDesc);

			auto newActor = CreateStaticActor(keyName, staticDesc);

			newActor->AttachShape(newShape);

			return newActor;
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

	template<class T>
	inline std::weak_ptr<PhysicsDynamicActor> PhysicsScene::CreateDynamicActor(const tstring& keyName, const PhysicsActorDesc& dynamicDesc, const PhysicsShapeDesc& shapeDesc)
	{
		static_assert(std::is_base_of<PhysicsShapeBase, T>::value, "Shape must inherit PhysicsShapeBase.");

		try
		{
			if (_impl == nullptr)
				ERROR_THROW("No Implementation was generated.");

			auto result = _dynamicActors.find(keyName);

			if (result != _dynamicActors.end())
				return std::dynamic_pointer_cast<T>(result->second);

			auto newShape = std::make_shared<T>();
			newShape->Create(this, shapeDesc);

			auto newActor = CreateDynamicActor(keyName, dynamicDesc);

			newActor->AttachShape(newShape);

			return newActor;
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