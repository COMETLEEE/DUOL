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

	PxVec3 ConvertVector3(const DUOLMath::Vector3& vec)
	{
		return PxVec3{ vec.x, vec.y, vec.z };
	}

	DUOLMath::Vector3 ConvertVector3(const PxVec3& vec)
	{
		return DUOLMath::Vector3{ vec.x, vec.y, vec.z };
	}

	PxQuat ConvertQuaternion(const DUOLMath::Quaternion& quat)
	{
		return PxQuat{ quat.x, quat.y, quat.z, quat.w };
	}

	DUOLMath::Quaternion ConvertQuaternion(const PxQuat& quat)
	{
		return DUOLMath::Quaternion{ quat.x, quat.y, quat.z, quat.w };
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

	GlobalPose ConvertTransform(const PxTransform& transform)
	{
		GlobalPose ret;

		ret._position = ConvertVector3(transform.p);
		ret._quaternion = ConvertQuaternion(transform.q);

		return ret;
	}

	PxShapeFlags ConvertShapeFlags(ShapeType flag)
	{
		PxShapeFlags retFlag;

		switch (flag)
		{

		case ShapeType::NONE:
		{
			return retFlag;
		}

		case ShapeType::TRIGGER:
		{
			return retFlag |= PxShapeFlag::Enum::eTRIGGER_SHAPE;
		}

		case ShapeType::TRIGGER_AND_SCENE_QUERY:
		{
			return retFlag |= PxShapeFlag::Enum::eTRIGGER_SHAPE | PxShapeFlag::Enum::eSCENE_QUERY_SHAPE;
		}

		case ShapeType::COLLIDER: 
		{
			return retFlag |= PxShapeFlag::Enum::eSIMULATION_SHAPE;
		}

		case ShapeType::COLLIDER_AND_SCENE_QUERY: 
		{
			return retFlag |= PxShapeFlag::Enum::eSIMULATION_SHAPE | PxShapeFlag::Enum::eSCENE_QUERY_SHAPE;
		}

		}

		return retFlag;
	}
}