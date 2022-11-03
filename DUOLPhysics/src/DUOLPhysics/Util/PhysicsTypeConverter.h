/**

	@file    PhysicsTypeConverter.h
	@brief   Wrapping된 Type의 Converter
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
		@brief	 Renderer Type에 따라 CUDA의 Interop Mode 반환
		@details -
		@param   rendererType - Wrapping된 RendererType
		@retval  Renderer와 매칭되는 Interop Mode
	**/
	PxCudaInteropMode::Enum ConvertInteropMode(RendererType rendererType);

	PxU8 operator &(ShapeFlag flag, PxU8 mask);

	/**
		@brief	 DUOLMath의 Vector를 PhysX의 Vector로 변환
		@details -
		@param   vec - DUOLMath의 Vector
		@retval  PhysX의 Vector
	**/
	PxVec3 ConvertVector3(const DUOLMath::Vector3& vec);

	/**
		@brief	 PhysX의 Vector를 DUOLMath의 Vector로 변환
		@details -
		@param   vec - PhysX의 Vector
		@retval  DUOLMath의 Vector
	**/
	DUOLMath::Vector3 ConvertPxVec3(const PxVec3& vec);

	/**
		@brief	 DUOLMath의 Matrix를 PhysX의 Transform으로 변환
		@details -
		@param   transform - DUOLMath의 Matrix
		@retval  PhysX의 Transform
	**/
	PxTransform ConvertTransform(const DUOLMath::Matrix& transform);

	/**
		@brief	 Physics Shape들의 Flag에 대한 Converter
		@details -
		@param   flag - Wrapping된 ShapeFlag
		@retval  PxFlags에 묶어서 반환
	**/
	PxShapeFlags ConvertShapeFlags(ShapeFlag flag);
}