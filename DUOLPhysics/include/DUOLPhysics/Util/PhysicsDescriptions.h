/**

	@file    PhysicsDescriptions.h
	@brief   Physics Data Block ����
	@details
			 @li ShapeFlag - ���� ����

				* SIMULATION
				 - �������� �ùķ��̼� �浹�� �ش��.

				* SCENE_QUERY
				 - Ray Casts, Overlap tests, Sweeps ���� �浹�� �ش��.

				* TRIGGER
				 - �浹�� Trigger Enter, Trigger Out �̺�Ʈ�� �߻���.

				* VISUALIZATION
				 - Debug Renderer Ȱ��ȭ

			@n

			@li PhysicsShapeDesc - ���� ����

			   * _isExclusive
				- Actor���� ���� ���� ����

			   * ShapeFlag
				- ShapeFlag ����.

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
	@brief     DUOL Physics�� ���� Namespace
	@details   -
**/
namespace DUOLPhysics
{
	class PhysicsMaterial;

	/**
		@enum    DUOLPhysics::RendererType
		@brief	 Renderer Type�� ���� CUDA�� Interop ��� ����
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
		@brief	 Physics Shape���� Scene���� ��� ������ �������� ���� Flag
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
		@brief   Scene ���� �ʱ�ȭ ��
		@details -
	**/
	struct PhysicsSceneDesc
	{
		DUOLMath::Vector3 _gravity;
	};

	/**
		@struct  PhysicsMaterialDesc
		@brief   Material ���� �ʱ�ȭ ��
		@details -
	**/
	struct PhysicsMaterialDesc
	{
		// ���� ���� ���
		float _staticFriction;

		// ���� ���� ���
		float _dynamicFriction;

		// �ݹ� ���
		float _restitution;
	};

	/**
		@struct  PhysicsPlaneDesc
		@brief   Plane ���� �ʱ�ȭ ��
		@details ����� ������
	**/
	struct PhysicsPlaneDesc
	{
		DUOLMath::Vector3 _normal;

		float _distance;
	};

	/**
		@struct  PhysicsSystemDesc
		@brief   System ���� �ʱ�ȭ ��
		@details -
	**/
	struct PhysicsSystemDesc
	{
		// Physics Visual Debugger ��� ����
		bool _usePvd;
	};

	/**
		@namespace DUOLPhysics
		@brief     CUDA ���� �ʱ�ȭ ��
		@details   Renderer�� D3D�� ��� GraphicsDevice Pointer�� �ʿ�
	**/
	struct PhysicsCudaDesc
	{
		RendererType _rendererType;

		void* _graphicsDevice;
	};

	/**
		@namespace DUOLPhysics
		@brief     Shape ���� �ʱ�ȭ ��
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
		@brief     Actor ���� �ʱ�ȭ ��
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