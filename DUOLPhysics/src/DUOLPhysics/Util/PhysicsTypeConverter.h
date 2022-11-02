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
		@brief	 Physics Shape들의 Flag에 대한 Converter
		@details -
		@param   flag - Wrapping된 ShapeFlag
		@retval  PxFlags에 묶어서 반환
	**/
	PxShapeFlags ConvertShapeFlags(ShapeFlag flag);
}