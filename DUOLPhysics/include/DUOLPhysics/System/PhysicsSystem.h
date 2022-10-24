/**

	@file    PhysicsSystem.h
	@brief   NVIDIA PhysX Wrapping Ŭ����
	@details -
	@author  JKim
	@date    21.10.2022

**/
#pragma once
#define _SILENCE_CXX20_CISO646_REMOVED_WARNING

#include <memory>

namespace DUOLPhysics
{
	class PhysicsImplementation;

	/**

		@class   PhysicsSystem
		@brief   NVIDIA PhysX Wrapping Ŭ����
		@details -

	**/
	class PhysicsSystem
	{
		/**
			@struct  PhysicsDescription
			@brief	 PhysX �ʱ�ȭ�� �� �ʿ��� Arguments ���
			@details -
		**/
		struct PhysicsDescription
		{
			// Renderer Type
			PxCudaInteropMode::Enum	_rendererType = PxCudaInteropMode::NO_INTEROP;

			// Renderer�� D3D�� ��� GraphicsDevice Pointer�� �ʿ�
			void* _graphicsDevice = nullptr;

			// Physics Visual Debugger ��� ����
			bool _usePvd = false;

			// ���� ���� ���
			float _staticFriction = 0.5f;

			// ���� ���� ���
			float _dynamicFriction = 0.5f;

			// �ݹ� ���
			float _restitution = 0.6f;

			// �߷�
			float _gravity = -9.81f;
		};

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
		std::shared_ptr<PhysicsImplementation> _impl;

	public:
		/**
			@brief	 PhysX ���� �ʱ�ȭ
			@details -
			@param   desc - PhysX �ʱ�ȭ�� �� �ʿ��� Arguments ���
			@retval  �ʱ�ȭ�� �����ϸ� true, �ƴ϶�� false
		**/
		bool Init(PhysicsDescription desc);

		/**
			@brief	 PhysX �Ҵ� ����
			@details -
		**/
		void Release();
	};
}