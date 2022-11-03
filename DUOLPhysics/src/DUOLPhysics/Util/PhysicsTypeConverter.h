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
#include "DUOLPhysics/PhysicsDescriptions.h"

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

	PxU8 operator &(ShapeFlag flag, PxU8 mask);

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
	DUOLMath::Vector3 ConvertPxVec3(const PxVec3& vec);

	/**
		@brief	 DUOLMath�� Matrix�� PhysX�� Transform���� ��ȯ
		@details -
		@param   transform - DUOLMath�� Matrix
		@retval  PhysX�� Transform
	**/
	PxTransform ConvertTransform(const DUOLMath::Matrix& transform);

	/**
		@brief	 Physics Shape���� Flag�� ���� Converter
		@details -
		@param   flag - Wrapping�� ShapeFlag
		@retval  PxFlags�� ��� ��ȯ
	**/
	PxShapeFlags ConvertShapeFlags(ShapeFlag flag);
}