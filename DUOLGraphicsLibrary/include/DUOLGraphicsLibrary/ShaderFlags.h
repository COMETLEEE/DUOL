#pragma once
#include "Export.h"
#include <vector>

namespace DUOLGraphicsLibrary
{
	enum class DUOLGRAPHICSLIBRARY_EXPORT ShaderType
	{
		UNKNOWN = 0,
		VERTEX = (1 << 0),
		HULL = (1 << 1),
		DOMAINS = (1 << 2),
		GEOMETRY = (1 << 3),
		PIXEL = (1 << 4),
		COMPUTE = (1 << 5)
	};

	struct DUOLGRAPHICSLIBRARY_EXPORT ShaderMacroDesc
	{
		ShaderMacroDesc():
			Name(nullptr),
			Definition(nullptr)
		{
			
		}

		ShaderMacroDesc(const char* name, const char* definition) :
			Name(name),
			Definition(definition)
		{

		}

		const char* Name;
		const char* Definition;
	};



	struct DUOLGRAPHICSLIBRARY_EXPORT ShaderDesc
	{
		ShaderDesc() :
			_type(ShaderType::UNKNOWN)
			, _source(nullptr)
			, _entryPoint(nullptr)
			, _profile(nullptr)
			, _shaderMacro()
			, _useStreamOut(false)
			, _flags(0)
		{

		}

		~ShaderDesc() = default;

		ShaderDesc(const ShaderDesc& shaderDesc) = default;

		ShaderType _type;

		const char* _source = nullptr;

		const char* _entryPoint = nullptr;

		const char* _profile = nullptr;

		std::vector<ShaderMacroDesc> _shaderMacro;

		bool _useStreamOut;

		unsigned int _flags;
	};
}