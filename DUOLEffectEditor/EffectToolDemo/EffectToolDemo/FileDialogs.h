#pragma once
class FileDialogs
{
public:
	// These return empty strings if cancelled
	static tstring OpenFile(const char* filter);
	static tstring SaveFile(const char* filter);
};

