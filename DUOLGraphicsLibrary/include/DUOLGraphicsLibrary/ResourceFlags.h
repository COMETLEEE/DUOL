#pragma once
#include "Export.h"

namespace DUOLGraphicsLibrary
{
	class Resource;

	enum class DUOLGRAPHICSLIBRARY_EXPORT ResourceType
	{
		UNDEFINED
		, BUFFER
		, TEXTURE
		, SAMPLER
	};

	/**
	 * \brief �� ���ҽ��� � �������� ���ε� �Ǵ� ���� ���� flag�Դϴ�.
	 */
	enum class DUOLGRAPHICSLIBRARY_EXPORT BindFlags
	{
		UNDEFINED = 0
		, VERTEXBUFFER = (1 << 0)
		, INDEXBUFFER = (1 << 1)
		, CONSTANTBUFFER = (1 << 2)
		, SHADERRESOURCE = (1 << 3)
		, UNORDEREDACCESS = (1 << 4)
		, STREAMOUTPUTBUFFER = (1 << 5)
		, DEPTHSTENCIL = (1 << 6)
		, RENDERTARGET = (1 << 7)
		, SAMPLER = (1 << 8)
	};

	enum class DUOLGRAPHICSLIBRARY_EXPORT CPUAccessFlags
	{
		READ = (1 << 0)
		, WRITE = (1 << 1)
		, WRITEDISCARD = (1 << 2)
		, READWRITE = (READ | WRITE)
	};

	/**
	 * \brief � ���̴����������� ���ε� �� ���ΰ�.
	 */
	enum class DUOLGRAPHICSLIBRARY_EXPORT StageFlags
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

	enum class DUOLGRAPHICSLIBRARY_EXPORT ResourceMiscFlags
	{
		RESOURCE_MISC_GENERATE_MIPS = 0x1L,
		RESOURCE_MISC_SHARED = 0x2L,
		RESOURCE_MISC_TEXTURECUBE = 0x4L,
		RESOURCE_MISC_DRAWINDIRECT_ARGS = 0x10L,
		RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS = 0x20L,
		RESOURCE_MISC_BUFFER_STRUCTURED = 0x40L,
		RESOURCE_MISC_RESOURCE_CLAMP = 0x80L,
		RESOURCE_MISC_SHARED_KEYEDMUTEX = 0x100L,
		RESOURCE_MISC_GDI_COMPATIBLE = 0x200L,
		RESOURCE_MISC_SHARED_NTHANDLE = 0x800L,
		RESOURCE_MISC_RESTRICTED_CONTENT = 0x1000L,
		RESOURCE_MISC_RESTRICT_SHARED_RESOURCE = 0x2000L,
		RESOURCE_MISC_RESTRICT_SHARED_RESOURCE_DRIVER = 0x4000L,
		RESOURCE_MISC_GUARDED = 0x8000L,
		RESOURCE_MISC_TILE_POOL = 0x20000L,
		RESOURCE_MISC_TILED = 0x40000L,
		RESOURCE_MISC_HW_PROTECTED = 0x80000L,
		RESOURCE_MISC_SHARED_DISPLAYABLE,
		RESOURCE_MISC_SHARED_EXCLUSIVE_WRITER
	};

	struct DUOLGRAPHICSLIBRARY_EXPORT ResourceViewDesc
	{
		ResourceViewDesc(Resource* resource, unsigned int slot, long bindFlags, long stageFlags) :
			_resource(resource)
			, _slot(slot)
			, _bindFlags(bindFlags)
			, _stageFlags(stageFlags)
		{

		}

		ResourceViewDesc() :
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
