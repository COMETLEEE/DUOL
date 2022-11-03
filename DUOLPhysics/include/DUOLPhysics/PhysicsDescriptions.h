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
	enum class ShapeFlag
	{
		SIMULATION = 0x01,
		SCENE_QUERY = 0x02,
		SIMULATION_AND_SCENE_QUERY = 0x03,
		TRIGGER = 0x04,
		TRIGGER_AND_SCENE_QUERY = 0x06,
		VISUALIZATION = 0x08,
		SIMULATION_AND_VISUALIZATION = 0x09,
		SCENE_QUERY_AND_VISUALIZATION = 0x0A,
		SIMULATION_AND_SCENE_QUERY_AND_VISUALIZATION = 0x0B,
		TRIGGER_AND_VISUALIZATION = 0x0C,
		TRIGGER_AND_SCENE_QUERY_AND_VISUALIZATION = 0x0E,
	};

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
			struct
			{
				float _x;

				float _y;

				float _z;
			} _box;

			struct
			{
				float _radius;

				float _halfHeight;
			} _capsule;

			struct
			{
				float _radius;
			} _sphere;
		};

		std::weak_ptr<PhysicsMaterial> _material;

		bool _isExclusive;

		ShapeFlag _flag;
	};

	/**
		@namespace DUOLPhysics
		@brief     Dynamic ���� �ʱ�ȭ ��
		@details   -
	**/
	struct PhysicsDynamicDesc
	{
		DUOLMath::Matrix _transform;
	};


	struct GlobalPose
	{
		DUOLMath::Vector3 _position;

		DUOLMath::Quaternion _quaternion;
	};


	struct PhysicsBoundingBox
	{
		DUOLMath::Vector3 _min;

		DUOLMath::Vector3 _max;
	};
}