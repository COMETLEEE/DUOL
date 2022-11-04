#pragma once
#include "DUOLGraphicsLibrary/ResourceFormat.h"
#include "DUOLGraphicsLibrary/Export.h"

namespace DUOLGraphicsLibrary
{
	struct BufferDesc
	{
		BufferDesc() :
			_size(0)
			, _stride(0)
			, _format(ResourceFormat::FORMAT_UNKNOWN)
			, _usage(ResourceUsage::USAGE_DEFAULT)
			, _bindFlags(0)
			, _cpuAccessFlags(0)
			, _miscFlags(0)
		{

		}

		~BufferDesc() = default;

		BufferDesc(const BufferDesc& bufferDesc) = default;

		int _size;

		int _stride;

		ResourceFormat _format;

		ResourceUsage _usage;

		long _bindFlags;

		long _cpuAccessFlags;

		long _miscFlags;
	};

}