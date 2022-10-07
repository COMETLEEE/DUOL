#pragma once
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

    struct TextureDesc
    {
		TextureType _type = TextureType::TEXTURE2D;

    	Resource_Format _format = Resource_Format::FORMAT_R8G8B8A8_UNORM;

    	Resource_Usage _Usage = Resource_Usage::USAGE_DEFAULT;

    	int _MipLevels = 1;

    	int _ArraySize = 1;

    	int _BindFlags;

    	int _CPUAccessFlags = 0;

    	int _MiscFlags = 0;
    };

}
