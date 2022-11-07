#pragma once
#include "DUOLGraphicsLibrary/Export.h"
#include "DUOLGraphicsLibrary/ResourceFormat.h"
#include "DUOLMath/DUOLMath.h"

namespace DUOLGraphicsLibrary
{
	struct DUOLGRAPHICSLIBRARY_EXPORT DUOLMath::Vector2;
	struct DUOLGRAPHICSLIBRARY_EXPORT DUOLMath::Vector3;

	enum class TextureType
	{
		TEXTURE1D,
		TEXTURE2D,
		TEXTURE3D,
		TEXTURECUBE,
		TEXTURE1DARRAY,
		TEXTURE2DARRAY,
		TEXTURECUBEARRAY,
		TEXTURE2DMS,
		TEXTURE2DMSARRAY,
	};

//todo :: tetureDesc tstring으로.. 내부 캐스팅하자

	struct DUOLGRAPHICSLIBRARY_EXPORT TextureDesc
	{
		TextureDesc() :
			_type(TextureType::TEXTURE2D)
			, _textureExtend(1, 1, 1)
			, _sample(1)
			, _format(ResourceFormat::FORMAT_R8G8B8A8_UNORM)
			, _usage(ResourceUsage::USAGE_DEFAULT)
			, _mipLevels(1)
			, _arraySize(1)
			, _bindFlags(0)
			, _cpuAccessFlags(0)
			, _miscFlags(0)
			, _texturePath(nullptr)
		{

		}

		TextureDesc(
			TextureType			textureType
			, DUOLMath::Vector3 textureExtend
			, unsigned int		sample
			, ResourceFormat	format
			, ResourceUsage		usage
			, unsigned int		mipLevels
			, unsigned int		arraySize
			, long				bindFlags
			, long				cpuAccessFlags
			, long				miscFlags
			, const char* texturePath = nullptr) :
			_type(textureType)
			, _textureExtend(textureExtend)
			, _sample(sample)
			, _format(format)
			, _usage(usage)
			, _mipLevels(mipLevels)
			, _arraySize(arraySize)
			, _bindFlags(bindFlags)
			, _cpuAccessFlags(cpuAccessFlags)
			, _miscFlags(miscFlags)
			, _texturePath(texturePath)
		{

		}

		~TextureDesc() = default;

		TextureDesc(const TextureDesc& textureDesc) = default;

		TextureType _type;

		DUOLMath::Vector3 _textureExtend;

		unsigned int _sample;

		ResourceFormat _format;

		ResourceUsage _usage;

		unsigned int _mipLevels;

		unsigned int _arraySize;

		long _bindFlags;

		long _cpuAccessFlags;

		long _miscFlags;

		const char* _texturePath;
	};

}
