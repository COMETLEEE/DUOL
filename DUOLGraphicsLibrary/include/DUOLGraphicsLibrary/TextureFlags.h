#pragma once
#include "DUOLGraphicsLibrary/Export.h"
#include "DUOLGraphicsLibrary/Resource_Format.h"

namespace DUOLGraphicsLibrary
{
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

    struct DUOLGRAPHICSLIBRARY_EXPORT TextureDesc
    {
		TextureDesc():
			_type(TextureType::TEXTURE2D)
			, _format(Resource_Format::FORMAT_R8G8B8A8_UNORM)
			, _usage(Resource_Usage::USAGE_DEFAULT)
			, _mipLevels(1)
			, _arraySize(1)
			, _bindFlags(0)
			, _cpuAccessFlags(0)
			, _miscFlags(0)
        {

        }

    	~TextureDesc() = default;

        TextureDesc(const TextureDesc& textureDesc) = default;

		TextureType _type;

    	Resource_Format _format ;

    	Resource_Usage _usage;

    	int _mipLevels;

    	int _arraySize;

    	int _bindFlags;

    	int _cpuAccessFlags;

    	int _miscFlags;
    };

}
