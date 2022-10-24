/**

	@file    PhysicsSystem.h
	@brief   NVIDIA PhysX Wrapping 클래스
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
		@brief   NVIDIA PhysX Wrapping 클래스
		@details -

	**/
	class PhysicsSystem
	{
		/**
			@struct  PhysicsDescription
			@brief	 PhysX 초기화할 때 필요한 Arguments 목록
			@details -
		**/
		struct PhysicsDescription
		{
			// Renderer Type
			PxCudaInteropMode::Enum	_rendererType = PxCudaInteropMode::NO_INTEROP;

			// Renderer가 D3D인 경우 GraphicsDevice Pointer가 필요
			void* _graphicsDevice = nullptr;

			// Physics Visual Debugger 사용 여부
			bool _usePvd = false;

			// 정적 마찰 계수
			float _staticFriction = 0.5f;

			// 동적 마찰 계수
			float _dynamicFriction = 0.5f;

			// 반발 계수
			float _restitution = 0.6f;

			// 중력
			float _gravity = -9.81f;
		};

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
		std::shared_ptr<PhysicsImplementation> _impl;

	public:
		/**
			@brief	 PhysX 설정 초기화
			@details -
			@param   desc - PhysX 초기화할 때 필요한 Arguments 목록
			@retval  초기화에 성공하면 true, 아니라면 false
		**/
		bool Init(PhysicsDescription desc);

		/**
			@brief	 PhysX 할당 해제
			@details -
		**/
		void Release();
	};
}