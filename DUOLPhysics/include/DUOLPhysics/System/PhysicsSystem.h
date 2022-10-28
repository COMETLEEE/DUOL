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
#include "../PhysicsMaterial.h"

/* etc */
#include "../PhysicsDescriptions.h"
#include "DUOLCommon/StringHelper.h"

#include <map>
#include <memory>

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
	};
}