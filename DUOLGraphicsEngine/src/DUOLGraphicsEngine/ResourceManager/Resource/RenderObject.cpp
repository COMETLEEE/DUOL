#include "DUOLGraphicsEngine/ResourceManager/Resource/RenderObject.h"

#include "DUOLGraphicsEngine/ResourceManager/Resource/Mesh.h"
#include "DUOLGraphicsEngine/Util/ByteBuffer.h"

namespace DUOLGraphicsEngine
{
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

	void MeshInfo::SetScreenSpaceReflection(bool value)
	{
		if(value)
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
		if(_commonRenderData._renderFlag &= 1)
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
		//컬러값을 packing합니다.
		// 255 | R, 255 | G, 255 | B, 255 | 255 none 

		unsigned int r = std::clamp<unsigned int>(rimColor.x * 255, 0, 255);
		unsigned int g = std::clamp<unsigned int>(rimColor.y * 255, 0, 255);
		unsigned int b = std::clamp<unsigned int>(rimColor.z * 255, 0, 255);

		_commonRenderData._rimColor = 0;

		_commonRenderData._rimColor = r << 24;
		_commonRenderData._rimColor += g << 16;
		_commonRenderData._rimColor += b << 8;
	}

	void DUOLGraphicsEngine::MeshInfo::SetRimPower(float value)
	{
		_commonRenderData._rimPower = value;
	}

	const float& DUOLGraphicsEngine::MeshInfo::GetRimPower()
	{
		return _commonRenderData._rimPower;
	}

	const DUOLMath::Vector4& DUOLGraphicsEngine::MeshInfo::GetRimColor()
	{
		//컬러값을 unPacking합니다.
		// 255 | R, 255 | G, 255 | B, 255 | 255 none 
		DUOLMath::Vector4 ret;

		ret.x = _commonRenderData._rimColor >> 24;
		ret.y = (float)((_commonRenderData._rimColor >> 16) & 0xff);
		ret.z = (float)((_commonRenderData._rimColor >> 8) & 0xff);
		ret.w = 255.f;

		ret /= 255;

		return ret;
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
		//컬러값을 packing합니다.
		// 255 | R, 255 | G, 255 | B, 255 | 255 none 

		unsigned int r = std::clamp<unsigned int>(rimColor.x * 255, 0, 255);
		unsigned int g = std::clamp<unsigned int>(rimColor.y * 255, 0, 255);
		unsigned int b = std::clamp<unsigned int>(rimColor.z * 255, 0, 255);

		_commonRenderData._rimColor = 0;

		_commonRenderData._rimColor = r << 24;
		_commonRenderData._rimColor += g << 16;
		_commonRenderData._rimColor += b << 8;
	}

	void DUOLGraphicsEngine::SkinnedMeshInfo::SetRimPower(float value)
	{
		_commonRenderData._rimPower = value;
	}

	const float& DUOLGraphicsEngine::SkinnedMeshInfo::GetRimPower()
	{
		return _commonRenderData._rimPower;
	}

	const DUOLMath::Vector4& DUOLGraphicsEngine::SkinnedMeshInfo::GetRimColor()
	{
		//컬러값을 unPacking합니다.
		// 255 | R, 255 | G, 255 | B, 255 | 255 none 
		DUOLMath::Vector4 ret;

		ret.x = _commonRenderData._rimColor >> 24;
		ret.y = (float)((_commonRenderData._rimColor >> 16) & 0xff);
		ret.z = (float)((_commonRenderData._rimColor >> 8) & 0xff);
		ret.w = 255.f;

		ret /= 255;

		return ret;
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
