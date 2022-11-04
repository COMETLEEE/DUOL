#pragma once
#include <memory>
#include <string>
#include <Windows.h>

namespace DUOLGraphicsLibrary
{
	// for dll runtime Linking On WindowOS

	class Module
	{
	public:
		Module(const char* moduleName);
		virtual ~Module();

	public:
		static std::unique_ptr<Module> Load(const char* moduleName);

		void* LoadProcedure(const char* procedureName);

	private:
		/**
		 * \brief 
		 */
		HMODULE _module;

	};

}