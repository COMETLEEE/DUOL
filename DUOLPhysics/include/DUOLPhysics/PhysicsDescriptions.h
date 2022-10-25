#pragma once
#include "PxPhysicsAPI.h"
#include "DUOLMath/DUOLMath.h"

namespace DUOLPhysics
{
	enum class RendererType
	{
		NONE,
		OPENGL,
		DIRECTX10,
		DIRECTX11,
	};

	/* Scene 적용 전달 값*/
	struct PhysicsSceneDesc
	{
		DUOLMath::Vector3 _gravity;
	};

	/* Material 적용 전달 값 */
	struct PhysicsMaterialDesc
	{
		// 정적 마찰 계수
		float _staticFriction;

		// 동적 마찰 계수
		float _dynamicFriction;

		// 반발 계수
		float _restitution;
	};

	/* Plane 적용 전달 값 */
	struct PhysicsPlaneDesc
	{
		DUOLMath::Vector3 _normal;

		float _distance;
	};

	/* 기반 적용 전달 값 */
	struct PhysicsSystemDesc
	{
		// Physics Visual Debugger 사용 여부
		bool _usePvd;
	};

	/* CUDA 적용 전달 값 */
	struct PhysicsCudaDesc
	{
		// Renderer Type
		RendererType _rendererType;

		// Renderer가 D3D인 경우 GraphicsDevice Pointer가 필요
		void* _graphicsDevice;
	};
}