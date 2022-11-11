/**

	@file    PhysicsDataStructure.h
	@brief	 Physics에서 사용되는 자료 구조체
	@details -
	@author  JKim
	@date    11.11.2022

**/
#pragma once
#include "DUOLMath/DUOLMath.h"

namespace DUOLPhysics
{
	using namespace DUOLMath;

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
		@brief	 충돌 지점 관련 정보
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
		@brief	 충돌된 다른 객체와 충돌 지점에 대한 정보
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
		@brief	 충돌된 다른 객체의 정보
		@details -
	**/
	struct Trigger
	{
		void* _other;
	};

	using TriggerEvent = std::function<void(const std::shared_ptr<Trigger>&)>;

	/**
		@struct  GlobalPose
		@brief	 Physics Space에서 사용하는 위치와 회전 정보
		@details -
	**/
	struct GlobalPose
	{
		DUOLMath::Vector3 _position;

		DUOLMath::Quaternion _quaternion;
	};

	/**
		@struct  PhysicsBoundingBox
		@brief	 객체를 감싸는 Box의 최소 값 Vertex와 최대 값 Vertex 정보
		@details -
	**/
	struct PhysicsBoundingBox
	{
		DUOLMath::Vector3 _min;

		DUOLMath::Vector3 _max;
	};

	/**
		@struct  RaycastHit
		@brief	 Ray 충돌 정보
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
}