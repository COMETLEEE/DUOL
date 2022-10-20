#pragma once
#include "Export.h"
#include <vector>

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
	 * \brief �� ���ҽ��� � �������� ���ε� �Ǵ� ���� ���� flag�Դϴ�.
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
	};

	enum class CPUAccessFlags
	{
		READ = (1 << 0)
		, WRITE = (1 << 1)
		, READWIRTE = (READ | WRITE)
	};

	/**
	 * \brief � ���̴����������� ���ε� �� ���ΰ�.
	 */
	enum class StageFlags
	{
		UNDEFINED = 0,
		VERTEXSTAGE = (1 << 0),
		HULLSTAGE = (1 << 1),
		DOMAINSSTAGE = (1 << 2),
		GEOMETRYSTAGE = (1 << 3),
		PIXELSTAGE = (1 << 4),
		COMPUTESTAGE = (1 << 5)
	};

	struct ResourceViewDesc
	{
		ResourceViewDesc(Resource* resource, unsigned int slot, long bindFlags, long stageFlags) :
			_resource(resource)
			, _slot(slot)
			, _bindFlags(bindFlags)
			, _stageFlags(stageFlags)
		{

		}

		Resource* _resource;

		unsigned int _slot;

		long _bindFlags;

		long _stageFlags;
	};

	struct ResourceViewLayout
	{
		std::vector<ResourceViewDesc> _resourceViews;
	};

}
