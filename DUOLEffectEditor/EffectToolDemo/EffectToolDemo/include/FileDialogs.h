#pragma once

#include "StringHelper.h"

class FileDialogs
{
public:
	// These return empty strings if cancelled
	static DUOLCommon::tstring OpenFile(const char* filter);
	static DUOLCommon::tstring SaveFile(const char* filter);
};

