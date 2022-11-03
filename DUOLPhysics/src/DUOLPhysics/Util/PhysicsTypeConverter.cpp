#include "PhysicsTypeConverter.h"

namespace DUOLPhysics
{
	PxCudaInteropMode::Enum ConvertInteropMode(RendererType rendererType)
	{
		switch (rendererType)
		{
		case RendererType::OPENGL: return PxCudaInteropMode::OGL_INTEROP;
		case RendererType::DIRECTX10: return PxCudaInteropMode::D3D10_INTEROP;
		case RendererType::DIRECTX11: return PxCudaInteropMode::D3D11_INTEROP;
		}

		return PxCudaInteropMode::NO_INTEROP;
	}

	PxU8 operator &(ShapeFlag flag, PxU8 mask)
	{
		return static_cast<PxU8>(flag) & mask;
	}

	PxVec3 ConvertVector3(const DUOLMath::Vector3& vec)
	{
		return PxVec3{ vec.x, vec.y, vec.z };
	}

	DUOLMath::Vector3 ConvertPxVec3(const PxVec3& vec)
	{
		return DUOLMath::Vector3{ vec.x, vec.y, vec.z };
	}

	PxTransform ConvertTransform(const DUOLMath::Matrix& transform)
	{
		float v[] =
		{
			transform._11, transform._12, transform._13, transform._14,
			transform._21, transform._22, transform._23, transform._24,
			transform._31, transform._32, transform._33, transform._34,
			transform._41, transform._42, transform._43, transform._44
		};

		return PxTransform(PxMat44(v));
	}

	PxShapeFlags ConvertShapeFlags(ShapeFlag flag)
	{
		PxShapeFlags retFlag;

		if ((flag & (1 << 0)) != 0)
			retFlag |= PxShapeFlag::Enum::eSIMULATION_SHAPE;

		if ((flag & (1 << 1)) != 0)
			retFlag |= PxShapeFlag::Enum::eSCENE_QUERY_SHAPE;

		if ((flag & (1 << 2)) != 0)
			retFlag |= PxShapeFlag::Enum::eTRIGGER_SHAPE;

		if ((flag & (1 << 3)) != 0)
			retFlag |= PxShapeFlag::Enum::eVISUALIZATION;

 		return retFlag;
	}
}