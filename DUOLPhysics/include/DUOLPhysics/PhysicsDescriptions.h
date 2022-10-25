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

	/* Scene ���� ���� ��*/
	struct PhysicsSceneDesc
	{
		DUOLMath::Vector3 _gravity;
	};

	/* Material ���� ���� �� */
	struct PhysicsMaterialDesc
	{
		// ���� ���� ���
		float _staticFriction;

		// ���� ���� ���
		float _dynamicFriction;

		// �ݹ� ���
		float _restitution;
	};

	/* Plane ���� ���� �� */
	struct PhysicsPlaneDesc
	{
		DUOLMath::Vector3 _normal;

		float _distance;
	};

	/* ��� ���� ���� �� */
	struct PhysicsSystemDesc
	{
		// Physics Visual Debugger ��� ����
		bool _usePvd;
	};

	/* CUDA ���� ���� �� */
	struct PhysicsCudaDesc
	{
		// Renderer Type
		RendererType _rendererType;

		// Renderer�� D3D�� ��� GraphicsDevice Pointer�� �ʿ�
		void* _graphicsDevice;
	};
}