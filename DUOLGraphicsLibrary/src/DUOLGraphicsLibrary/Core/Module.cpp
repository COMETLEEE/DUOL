#include "Module.h"

DUOLGraphicsLibrary::Module::Module(const char* moduleName)
{
	_module = LoadLibraryA(moduleName);
}

DUOLGraphicsLibrary::Module::~Module()
{
	FreeLibrary(_module);
}

std::unique_ptr<DUOLGraphicsLibrary::Module> DUOLGraphicsLibrary::Module::Load(const char* moduleName)
{
	return std::make_unique<Module>(moduleName);
}

void* DUOLGraphicsLibrary::Module::LoadProcedure(const char* procedureName)
{
	auto procAddr = GetProcAddress(_module, procedureName);

	return reinterpret_cast<void*>(procAddr);
}
