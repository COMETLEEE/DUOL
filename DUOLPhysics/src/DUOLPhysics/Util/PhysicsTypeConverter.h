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
		@brief
		@details -
		@param   vec -
		@retval      -
	**/
	PxVec3 ConvertVector3(const DUOLMath::Vector3& vec);

	/**
		@brief
		@details -
		@param   transform -
		@retval            -
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