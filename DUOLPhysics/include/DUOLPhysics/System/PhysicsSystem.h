/**

	@file    PhysicsSystem.h
	@brief   NVIDIA PhysX Wrapping 클래스
	@details -
	@author  JKim
	@date    21.10.2022

**/
#pragma once
#include "../PhysicsDescriptions.h"
#include "DUOLCommon/StringHelper.h"

#include "../Scene/PhysicsScene.h"
#include "../PhysicsMaterial.h"

#include <map>
#include <memory>

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
		**/
		std::weak_ptr<PhysicsScene> CreateScene(const tstring& keyName, const PhysicsSceneDesc& sceneDesc);

		/**
			@brief	 Physics Material 생성
			@details -
			@param   keyName      - Material의 Name
			@param   materialDesc - Material 생성에 필요한 값
		**/
		std::weak_ptr<PhysicsMaterial> CreateMaterial(const tstring& keyName, const PhysicsMaterialDesc& materialDesc);

		/**
			@brief	 Physics Plane 생성
			@details -
			@param   keyName      - Plane의 Name
			@param   sceneName    - Plane을 생성할 Scene의 Name
			@param   materialName - 생성할 Plane의 Material Name
			@param   planeDesc    - Plane 생성에 필요한 값
		**/
		void CreatePlane(const tstring& keyName, const tstring& sceneName, const tstring& materialName, const PhysicsPlaneDesc& planeDesc);
	

		void CreateDynamic(const tstring& keyName, const tstring& materialName, const tstring& shapeName);
	};
}