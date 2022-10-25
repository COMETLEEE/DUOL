/**

	@file    PhysicsSystem.h
	@brief   NVIDIA PhysX Wrapping 클래스
	@details -
	@author  JKim
	@date    21.10.2022

**/
#pragma once
#ifndef _SILENCE_CXX20_CISO646_REMOVED_WARNING
#define _SILENCE_CXX20_CISO646_REMOVED_WARNING
#endif

#include "PhysicsDescriptions.h"
#include "DUOLCommon/StringHelper.h"

#include <map>

namespace DUOLPhysics
{
	class PhysicsSystemImpl;

	class PhysicsScene;
	class PhysicsMaterial;

	/**

		@class   PhysicsSystem
		@brief   NVIDIA PhysX Wrapping 클래스
		@details -

	**/
	class PhysicsSystem
	{
	public:
		/**
			@brief   PhysicsSystem 클래스 생성자
			@details -
		**/
		PhysicsSystem();

		/**
			@brief   PhysicsSystem 클래스 default 소멸자
			@details -
		**/
		~PhysicsSystem() = default;

	private:
		std::shared_ptr<PhysicsSystemImpl> _impl;

		std::map<DUOLCommon::tstring, PhysicsScene> _scenes;

		std::map<DUOLCommon::tstring, PhysicsMaterial> _materials;

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
			@param   sceneDesc - Scene 초기화에 필요한 값
		**/
		void CreateScene(const DUOLCommon::tstring& keyName, const PhysicsSceneDesc& sceneDesc);

		/**
			@brief	 Physics Material 생성
			@details -
			@param   keyName      - Material의 Name
			@param   materialDesc - Material 초기화에 필요한 값
		**/
		void CreateMaterial(const DUOLCommon::tstring& keyName, const PhysicsMaterialDesc& materialDesc);
	};
}