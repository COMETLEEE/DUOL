#pragma once
#include "DUOLGraphicsLibrary/Resource_Format.h"

namespace DUOLGraphicsLibrary
{
	struct BufferDesc
	{
		int _size = 0;

		int _stride = 0;

		Resource_Format _format = Resource_Format::FORMAT_UNKNOWN;

		Resource_Usage _usage = Resource_Usage::USAGE_DEFAULT;

		long _bindFlags = 0;

		long _cpuAccessFlags = 0;

		long _miscFlags = 0;
	};

}