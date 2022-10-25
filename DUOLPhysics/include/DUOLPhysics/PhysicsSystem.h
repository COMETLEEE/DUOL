/**

	@file    PhysicsSystem.h
	@brief   NVIDIA PhysX Wrapping Ŭ����
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
		@brief   NVIDIA PhysX Wrapping Ŭ����
		@details -

	**/
	class PhysicsSystem
	{
	public:
		/**
			@brief   PhysicsSystem Ŭ���� ������
			@details -
		**/
		PhysicsSystem();

		/**
			@brief   PhysicsSystem Ŭ���� default �Ҹ���
			@details -
		**/
		~PhysicsSystem() = default;

	private:
		std::shared_ptr<PhysicsSystemImpl> _impl;

		std::map<DUOLCommon::tstring, PhysicsScene> _scenes;

		std::map<DUOLCommon::tstring, PhysicsMaterial> _materials;

		std::map<DUOLCommon::tstring, PhysicsPlane> _planes;

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
		**/
		void CreateScene(const DUOLCommon::tstring& keyName, const PhysicsSceneDesc& sceneDesc);

		/**
			@brief	 Physics Material ����
			@details -
			@param   keyName      - Material�� Name
			@param   materialDesc - Material ������ �ʿ��� ��
		**/
		void CreateMaterial(const DUOLCommon::tstring& keyName, const PhysicsMaterialDesc& materialDesc);

		/**
			@brief	 Physics Plane ����
			@details -
			@param   keyName      - Plane�� Name
			@param   sceneName    - Plane�� ������ Scene�� Name
			@param   materialName - ������ Plane�� Material Name
			@param   planeDesc    - Plane ������ �ʿ��� ��
		**/
		void CreatePlane(const DUOLCommon::tstring& keyName, const DUOLCommon::tstring& sceneName, const DUOLCommon::tstring& materialName, const PhysicsPlaneDesc& planeDesc);
	};
}