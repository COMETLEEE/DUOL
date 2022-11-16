/**

	@file    PhysicsDescriptions.h
	@brief   Physics Data Block 모음
	@details
			 @li ShapeFlag - 유형 정보

				* SIMULATION
				 - 물리적인 시뮬레이션 충돌에 해당됨.

				* SCENE_QUERY
				 - Ray Casts, Overlap tests, Sweeps 등의 충돌에 해당됨.

				* TRIGGER
				 - 충돌시 Trigger Enter, Trigger Out 이벤트가 발생됨.

				* VISUALIZATION
				 - Debug Renderer 활성화

			@n

			@li PhysicsShapeDesc - 유형 정보

			   * _isExclusive
				- Actor간에 공유 여부 결정

			   * ShapeFlag
				- ShapeFlag 참고.

			@n

	@author  JKim
	@date    26.10.2022

**/
#pragma once
#include "DUOLMath/DUOLMath.h"
#include "PhysicsFlags.h"

#include <memory>

/**
	@namespace DUOLPhysics
	@brief     DUOL Physics의 전역 Namespace
	@details   -
**/
namespace DUOLPhysics
{
	class PhysicsMaterial;

	/**
		@enum    DUOLPhysics::RendererType
		@brief	 Renderer Type에 따라 CUDA의 Interop 모드 결정
		@details -
	**/
	enum class RendererType
	{
		NONE,
		OPENGL,
		DIRECTX10,
		DIRECTX11,
	};

	/**
		@enum    DUOLPhysics::ShapeFlag
		@brief	 Physics Shape들이 Scene에서 어떻게 동작할 것인지에 대한 Flag
		@details -
	**/
	struct ShapeFlag
	{
		enum
		{
			NONE = 0,
			COLLIDER = 0b001,
			SCENE_QUERY = 0b010,
			TRIGGER = 0b100,
		};
	};

	using ShapeFlags = PhysicsFlags<ShapeFlag, unsigned char>;

	/**
		@struct  PhysicsSceneDesc
		@brief   Scene 적용 초기화 값
		@details -
	**/
	struct PhysicsSceneDesc
	{
		DUOLMath::Vector3 _gravity;
	};

	/**
		@struct  PhysicsMaterialDesc
		@brief   Material 적용 초기화 값
		@details -
	**/
	struct PhysicsMaterialDesc
	{
		// 정적 마찰 계수
		float _staticFriction;

		// 동적 마찰 계수
		float _dynamicFriction;

		// 반발 계수
		float _restitution;
	};

	/**
		@struct  PhysicsPlaneDesc
		@brief   Plane 적용 초기화 값
		@details 평면의 방정식
	**/
	struct PhysicsPlaneDesc
	{
		DUOLMath::Vector3 _normal;

		float _distance;
	};

	/**
		@struct  PhysicsSystemDesc
		@brief   System 관련 초기화 값
		@details -
	**/
	struct PhysicsSystemDesc
	{
		// Physics Visual Debugger 사용 여부
		bool _usePvd;
	};

	/**
		@namespace DUOLPhysics
		@brief     CUDA 적용 초기화 값
		@details   Renderer가 D3D인 경우 GraphicsDevice Pointer가 필요
	**/
	struct PhysicsCudaDesc
	{
		RendererType _rendererType;

		void* _graphicsDevice;
	};

	/**
		@namespace DUOLPhysics
		@brief     Shape 적용 초기화 값
		@details   -
	**/
	struct PhysicsShapeDesc
	{
		union
		{
			// Box
			struct
			{
				float _halfExtentX;

				float _halfExtentY;

				float _halfExtentZ;

			} _box;

			// Capsule
			struct
			{
				float _radius;

				float _halfHeight;

			} _capsule;

			// Sphere
			struct
			{
				float _radius;

			} _sphere;

			// Mesh
			struct
			{
				struct
				{
					void* _buffer;

					unsigned _count;

					unsigned _stride;

				} _vertex;

				struct
				{
					void* _buffer;

					unsigned _count;

					unsigned _stride;

				} _index;

			} _mesh;

			// Convex Mesh
			struct
			{
				struct
				{
					void* _buffer;

					unsigned _count;

					unsigned _stride;

				} _vertex;

			} _convexMesh;

		};

		std::weak_ptr<PhysicsMaterial> _material;

		bool _isExclusive;

		ShapeFlags _flag;
	};

	/**
		@namespace DUOLPhysics
		@brief     Actor 적용 초기화 값
		@details   -
	**/
	struct PhysicsActorDesc
	{
		//union
		//{
		//	struct
		//	{

		//	} _dynamic;

		//	struct
		//	{

		//	} _static;
		//};

		DUOLMath::Matrix _transform;
	};
}