#include "DUOLGraphicsEngine/ResourceManager/Resource/RenderObject.h"

#include "DUOLGraphicsEngine/ResourceManager/Resource/Mesh.h"
#include "DUOLGraphicsEngine/Util/ByteBuffer.h"

namespace DUOLGraphicsEngine
{
	unsigned int Vector4ToUnSignedInt(const DUOLMath::Vector4& value)
	{
		const unsigned int r = std::clamp<unsigned int>(value.x * 255, 0, 255);
		const unsigned int g = std::clamp<unsigned int>(value.y * 255, 0, 255);
		const unsigned int b = std::clamp<unsigned int>(value.z * 255, 0, 255);
		const unsigned int a = std::clamp<unsigned int>(value.w * 255, 0, 255);

		unsigned int result = 0;

		result = r << 24;
		result += g << 16;
		result += b << 8;
		result += a;

		return result;
	}

	DUOLMath::Vector4 UnsignedIntToVector4(const unsigned int& value)
	{
		DUOLMath::Vector4 ret;

		ret.x = value >> 24;
		ret.y = (float)((value >> 16) & 0xff);
		ret.z = (float)((value >> 8) & 0xff);
		ret.w = (float)(value & 0xff);

		ret /= 255;

		return ret;
	}


	Transform::Transform() :
		_world(DUOLMath::Matrix::Identity)
		, _worldInvTranspose(DUOLMath::Matrix::Identity)
	{
	}

	MeshInfo::~MeshInfo()
	{
	}

	bool MeshInfo::BindPipeline(ByteBuffer* bufferStartPoint, int bufferOffset)
	{
		bufferStartPoint->WriteData(&_commonRenderData, sizeof(CommonRenderBindData), bufferOffset);
		bufferOffset += sizeof(CommonRenderBindData);
		bufferStartPoint->WriteData(_transform, sizeof(Transform), bufferOffset);

		return true;
	}

	void MeshInfo::SetPaperBurnColor(const DUOLMath::Vector4& firstColor, const DUOLMath::Vector4& secondColor)
	{
		_commonRenderData._paperBurnFirstColor = Vector4ToUnSignedInt(firstColor);
		_commonRenderData._paperBurnSecondColor = Vector4ToUnSignedInt(secondColor);
	}

	void MeshInfo::SetScreenSpaceReflection(bool value)
	{
		if (value)
		{
			_commonRenderData._renderFlag |= 1;
		}
		else
		{
			_commonRenderData._renderFlag &= ~1;
		}
	}

	bool MeshInfo::IsEnableScreenSpaceReflection()
	{
		if (_commonRenderData._renderFlag &= 1)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void MeshInfo::SetRenderFlag(RenderFlags flag, bool value)
	{
		if (value)
		{
			_commonRenderData._renderFlag |= static_cast<unsigned int>(flag);
		}
		else
		{
			_commonRenderData._renderFlag &= ~static_cast<unsigned int>(flag);
		}
	}

	bool MeshInfo::GetRenderFlag(RenderFlags flag)
	{
		if (_commonRenderData._renderFlag &= static_cast<unsigned int>(flag))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void DUOLGraphicsEngine::MeshInfo::SetRimColor(DUOLMath::Vector4 rimColor)
	{
		_commonRenderData._rimColor = Vector4ToUnSignedInt(rimColor);
	}

	void MeshInfo::SetOffset(float offset)
	{
		_commonRenderData._offset = offset;
	}

	void DUOLGraphicsEngine::MeshInfo::SetRimPower(float value)
	{
		_commonRenderData._rimPower = value;
	}

	float DUOLGraphicsEngine::MeshInfo::GetRimPower()
	{
		return _commonRenderData._rimPower;
	}

	DUOLMath::Vector4 DUOLGraphicsEngine::MeshInfo::GetRimColor()
	{
		return UnsignedIntToVector4(_commonRenderData._rimColor);
	}

	float MeshInfo::GetOffset()
	{
		return _commonRenderData._offset;
	}

	SkinnedMeshInfo::~SkinnedMeshInfo()
	{
	}

	bool SkinnedMeshInfo::BindPipeline(ByteBuffer* bufferStartPoint, int bufferOffset)
	{
		bufferStartPoint->WriteData(&_commonRenderData, sizeof(CommonRenderBindData), bufferOffset);
		bufferOffset += sizeof(CommonRenderBindData);
		bufferStartPoint->WriteData(_transform, sizeof(Transform), bufferOffset);
		bufferOffset += sizeof(Transform);
		bufferStartPoint->WriteData(_boneTransforms->data(), sizeof(DUOLMath::Matrix) * _boneTransforms->size(), bufferOffset);

		return true;
	}

	void SkinnedMeshInfo::SetRenderFlag(RenderFlags flag, bool value)
	{
		if (value)
		{
			_commonRenderData._renderFlag |= static_cast<unsigned int>(flag);
		}
		else
		{
			_commonRenderData._renderFlag &= ~static_cast<unsigned int>(flag);
		}
	}

	bool SkinnedMeshInfo::GetRenderFlag(RenderFlags flag)
	{
		if (_commonRenderData._renderFlag &= static_cast<unsigned int>(flag))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void DUOLGraphicsEngine::SkinnedMeshInfo::SetRimColor(DUOLMath::Vector4 rimColor)
	{
		_commonRenderData._rimColor = Vector4ToUnSignedInt(rimColor);
	}

	void DUOLGraphicsEngine::SkinnedMeshInfo::SetRimPower(float value)
	{
		_commonRenderData._rimPower = value;
	}

	const float& DUOLGraphicsEngine::SkinnedMeshInfo::GetRimPower()
	{
		return _commonRenderData._rimPower;
	}

	DUOLMath::Vector4 DUOLGraphicsEngine::SkinnedMeshInfo::GetRimColor()
	{
		return UnsignedIntToVector4(_commonRenderData._rimColor);
	}

	void SkinnedMeshInfo::SetPaperBurnColor(const DUOLMath::Vector4& firstColor, const DUOLMath::Vector4& secondColor)
	{
		_commonRenderData._paperBurnFirstColor = Vector4ToUnSignedInt(firstColor);
		_commonRenderData._paperBurnSecondColor = Vector4ToUnSignedInt(secondColor);
	}

	const DUOLMath::Vector4& SkinnedMeshInfo::GetPaperBurnFirstColor()
	{
		return UnsignedIntToVector4(_commonRenderData._paperBurnFirstColor);
	}

	const DUOLMath::Vector4& SkinnedMeshInfo::GetPaperBurnSecondColor()
	{
		return UnsignedIntToVector4(_commonRenderData._paperBurnSecondColor);
	}

	bool DebugInfo::BindPipeline(ByteBuffer* bufferStartPoint, int bufferOffset)
	{
		return true;
	}

	bool ParticleInfo::BindPipeline(ByteBuffer* bufferStartPoint, int bufferOffset)
	{
		bufferStartPoint->WriteData(&_particleData, sizeof(ConstantBuffDesc::CB_PerObject_Particle), bufferOffset);

		return true;
	}
}
