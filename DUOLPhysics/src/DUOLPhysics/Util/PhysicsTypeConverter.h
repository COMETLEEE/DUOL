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

#include "DUOLPhysics/PhysicsDescriptions.h"
#include "PxPhysicsAPI.h"

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
		@brief	 Physics Shape���� Flag�� ���� Converter
		@details -
		@param   flag - Wrapping�� ShapeFlag
		@retval  PxFlags�� ��� ��ȯ
	**/
	PxShapeFlags ConvertShapeFlags(ShapeFlag flag);
}