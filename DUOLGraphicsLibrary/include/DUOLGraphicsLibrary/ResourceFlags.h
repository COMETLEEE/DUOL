#pragma once

namespace DUOLGraphicsLibrary
{
	enum class ResourceType
	{
		UNDEFINED
		, BUFFER
		, TEXTURE
		, SAMPLER
	};

	enum class BindFlags
	{
		VERTEXBUFFER = (1 << 0)
		, INDEXBUFFER = (1 << 1)
		, CONSTANTBUFFER = (1 << 2)
		, SHADERRESOURCE = (1 << 3)
		, UNRODERED = (1 << 4)
		, STREAMOUTPUTBUFFER = (1 << 5)
		, DEPTHSTENCIL = (1 << 6)
		, RENDERTARGET = (1 << 7)
	};
}
