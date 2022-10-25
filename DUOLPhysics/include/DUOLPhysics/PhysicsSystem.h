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
			@param   sceneDesc - Scene �ʱ�ȭ�� �ʿ��� ��
		**/
		void CreateScene(const DUOLCommon::tstring& keyName, const PhysicsSceneDesc& sceneDesc);

		/**
			@brief	 Physics Material ����
			@details -
			@param   keyName      - Material�� Name
			@param   materialDesc - Material �ʱ�ȭ�� �ʿ��� ��
		**/
		void CreateMaterial(const DUOLCommon::tstring& keyName, const PhysicsMaterialDesc& materialDesc);
	};
}