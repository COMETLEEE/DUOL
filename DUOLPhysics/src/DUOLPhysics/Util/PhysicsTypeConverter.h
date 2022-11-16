/**

	@file    PhysicsTypeConverter.h
	@brief   Wrapping�� Type�� Converter
	@details -
	@author  JKim
	@date    31.10.2022

**/
#pragma once
#ifndef _SILENCE_CXX20_CISO646_REMOVED_WARNING
#define _SILENCE_CXX20_CISO646_REMOVED_WARNING
#endif

#include "PxPhysicsAPI.h"
#include "DUOLPhysics/Util/PhysicsDataStructure.h"
#include "DUOLPhysics/Util/PhysicsDescriptions.h"

namespace DUOLPhysics
{
	using namespace physx;

	/**
		@brief	 Renderer Type�� ���� CUDA�� Interop Mode ��ȯ
		@details -
		@param   rendererType - Wrapping�� RendererType
		@retval  Renderer�� ��Ī�Ǵ� Interop Mode
	**/
	PxCudaInteropMode::Enum ConvertInteropMode(RendererType rendererType);

	/**
		@brief	 DUOLMath�� Vector�� PhysX�� Vector�� ��ȯ
		@details -
		@param   vec - DUOLMath�� Vector
		@retval  PhysX�� Vector
	**/
	PxVec3 ConvertVector3(const DUOLMath::Vector3& vec);

	/**
		@brief	 PhysX�� Vector�� DUOLMath�� Vector�� ��ȯ
		@details -
		@param   vec - PhysX�� Vector
		@retval  DUOLMath�� Vector
	**/
	DUOLMath::Vector3 ConvertVector3(const PxVec3& vec);

	/**
		@brief	 DUOLMath�� Quaternion�� PhysX�� PxQuat�� ��ȯ
		@details -
		@param   quat - DUOLMath�� Quaternion
		@retval  PhysX�� PxQuat
	**/
	PxQuat ConvertQuaternion(const DUOLMath::Quaternion& quat);

	/**
		@brief	 PhysX�� PxQuat�� DUOLMath�� Quaternion���� ��ȯ
		@details -
		@param   quat - PhysX�� PxQuat
		@retval  DUOLMath�� Quaternion
	**/
	DUOLMath::Quaternion ConvertQuaternion(const PxQuat& quat);

	/**
		@brief	 DUOLMath�� Matrix�� PhysX�� Transform���� ��ȯ
		@details -
		@param   transform - DUOLMath�� Matrix
		@retval  PhysX�� Transform
	**/
	PxTransform ConvertTransform(const DUOLMath::Matrix& transform);

	/**
		@brief	 PhysX�� Transform�� GlobalPose�� ��ȯ
		@details -
		@param   transform - PhysX�� Transform
		@retval  PhysicsPose
	**/
	PhysicsPose ConvertTransform(const PxTransform& transform);

	/**
		@brief	 Physics Shape���� Flag�� ���� Converter
		@details -
		@param   flag - Wrapping�� ShapeFlag
		@retval  PxShapeFlags�� ��� ��ȯ
	**/
	PxShapeFlags ConvertShapeFlags(ShapeFlag flag);
}