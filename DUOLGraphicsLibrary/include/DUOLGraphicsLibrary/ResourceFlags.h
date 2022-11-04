#pragma once
#include "Export.h"

namespace DUOLGraphicsLibrary
{
	class Resource;

	enum class ResourceType
	{
		UNDEFINED
		, BUFFER
		, TEXTURE
		, SAMPLER
	};

	/**
	 * \brief 이 리소스가 어떤 형식으로 바인딩 되는 가에 대한 flag입니다.
	 */
	enum class BindFlags
	{
		UNDEFINED = 0
		, VERTEXBUFFER = (1 << 0)
		, INDEXBUFFER = (1 << 1)
		, CONSTANTBUFFER = (1 << 2)
		, SHADERRESOURCE = (1 << 3)
		, UNRODERED = (1 << 4)
		, STREAMOUTPUTBUFFER = (1 << 5)
		, DEPTHSTENCIL = (1 << 6)
		, RENDERTARGET = (1 << 7)
		, SAMPLER = (1 << 8)
	};

	enum class CPUAccessFlags
	{
		READ = (1 << 0)
		, WRITE = (1 << 1)
		, READWIRTE = (READ | WRITE)
	};

	/**
	 * \brief 어떤 쉐이더스테이지에 바인딩 될 것인가.
	 */
	enum class StageFlags
	{
		UNDEFINED = 0,
		VERTEXSTAGE = (1 << 0),
		HULLSTAGE = (1 << 1),
		DOMAINSSTAGE = (1 << 2),
		GEOMETRYSTAGE = (1 << 3),
		PIXELSTAGE = (1 << 4),
		COMPUTESTAGE = (1 << 5),
		VSPS = (VERTEXSTAGE | PIXELSTAGE)
	};

	struct  ResourceViewDesc
	{
		ResourceViewDesc(Resource* resource, unsigned int slot, long bindFlags, long stageFlags) :
			_resource(resource)
			, _slot(slot)
			, _bindFlags(bindFlags)
			, _stageFlags(stageFlags)
		{

		}

		ResourceViewDesc():
			_resource(nullptr)
			, _slot(0)
			, _bindFlags(0)
			, _stageFlags(0)
		{
			
		};

		~ResourceViewDesc() = default;

		Resource* _resource;

		unsigned int _slot;

		long _bindFlags;

		long _stageFlags;
	};
}
