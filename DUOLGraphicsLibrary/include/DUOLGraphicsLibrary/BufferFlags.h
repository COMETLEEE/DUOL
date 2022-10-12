#pragma once
#include "DUOLGraphicsLibrary/Resource_Format.h"
#include "DUOLGraphicsLibrary/Export.h"

namespace DUOLGraphicsLibrary
{
	struct DUOLGRAPHICSLIBRARY_EXPORT BufferDesc
	{
		BufferDesc() :
			_size(0)
			, _stride(0)
			, _format(Resource_Format::FORMAT_UNKNOWN)
			, _usage(Resource_Usage::USAGE_DEFAULT)
			, _bindFlags(0)
			, _cpuAccessFlags(0)
			, _miscFlags(0)
		{

		}

		~BufferDesc() = default;

		BufferDesc(const BufferDesc& bufferDesc) = default;

		int _size;

		int _stride;

		Resource_Format _format;

		Resource_Usage _usage;

		long _bindFlags;

		long _cpuAccessFlags;

		long _miscFlags;
	};

}