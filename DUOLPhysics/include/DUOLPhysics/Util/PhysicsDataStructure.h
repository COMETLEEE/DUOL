/**

	@file    PhysicsDataStructure.h
	@brief	 Physics���� ���Ǵ� �ڷ� ����ü
	@details -
	@author  JKim
	@date    11.11.2022

**/
#pragma once
#include "DUOLMath/DUOLMath.h"

#include <memory>

namespace DUOLPhysics
{
	using namespace DUOLMath;

	/**
		@enum    DUOLPhysics::AxisLock
		@brief	 Actor�� ���� ��ż� �̵��̳� ȸ���� ���´�.
		@details -
	**/
	enum class AxisLock
	{
		NONE = 0,
		LINEAR_X = (1 << 0),
		LINEAR_Y = (1 << 1),
		LINEAR_Z = (1 << 2),
		ANGULAR_X = (1 << 3),
		ANGULAR_Y = (1 << 4),
		ANGULAR_Z = (1 << 5),
	};

	/**
		@struct  ContactData
		@brief	 �浹 ���� ���� ����
		@details -
	**/
	struct ContactData
	{
		Vector3	_position;

		Vector3	_normal;

		Vector3	_impulse;

		float	_separation;
	};

	/**
		@struct  Collision
		@brief	 �浹�� �ٸ� ��ü�� �浹 ������ ���� ����
		@details -
	**/
	struct Collision
	{
		void* _other;

		std::vector<ContactData> _data;
	};

	using CollisionEvent = std::function<void(const std::shared_ptr<Collision>&)>;

	/**
		@struct  Trigger
		@brief	 �浹�� �ٸ� ��ü�� ����
		@details -
	**/
	struct Trigger
	{
		void* _other;
	};

	using TriggerEvent = std::function<void(const std::shared_ptr<Trigger>&)>;

	/**
		@struct  PhysicsPose
		@brief	 Physics Space���� ����ϴ� ��ġ�� ȸ�� ����
		@details -
	**/
	struct PhysicsPose
	{
		DUOLMath::Vector3 _position;

		DUOLMath::Quaternion _quaternion;
	};

	/**
		@struct  PhysicsBoundingBox
		@brief	 ��ü�� ���δ� Box�� �ּ� �� Vertex�� �ִ� �� Vertex ����
		@details -
	**/
	struct PhysicsBoundingBox
	{
		DUOLMath::Vector3 _min;

		DUOLMath::Vector3 _max;
	};

	/**
		@struct  RaycastHit
		@brief	 Ray �浹 ����
		@details -
	**/
	struct RaycastHit
	{
		bool _isBlocking;

		DUOLMath::Vector3 _hitPosition;

		DUOLMath::Vector3 _hitNormal;

		float _hitDistance;

		void* _userData;
	};

	/**
		@struct  SceneDebugData
		@brief	 Scene�� Collider Vertex ����
		@details -
	**/
	struct SceneDebugData
	{
		struct VertexData
		{
			DUOLMath::Vector3 _position0;

			unsigned _color0;

			DUOLMath::Vector3 _position1;

			unsigned _color1;
		};

		const VertexData* _data;

		unsigned _count;
	};
}