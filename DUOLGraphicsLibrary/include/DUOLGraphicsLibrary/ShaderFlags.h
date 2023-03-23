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
			, _source()
			, _entryPoint()
			, _profile()
			, _shaderMacro()
			, _useStreamOut(false)
			, _flags(0)
		{

		}

		~ShaderDesc() = default;

		ShaderDesc(const ShaderDesc& shaderDesc);

		ShaderType _type;

		char _source[256];

		char _entryPoint[256];

		char _profile[256];

		//const char* _source;

		//const char* _entryPoint;

		//const char* _profile;

		std::vector<ShaderMacroDesc> _shaderMacro;

		bool _useStreamOut;

		unsigned int _flags;
	};

	inline ShaderDesc::ShaderDesc(const ShaderDesc& shaderDesc)
	{
		_type = shaderDesc._type;
		memcpy(_entryPoint, shaderDesc._entryPoint, 256);
		memcpy(_source, shaderDesc._source, 256);
		memcpy(_profile, shaderDesc._profile, 256);

		//_source = shaderDesc._source;
		//_entryPoint = shaderDesc._entryPoint;
		//_profile = shaderDesc._profile;
		_shaderMacro = shaderDesc._shaderMacro;
		_useStreamOut = shaderDesc._useStreamOut;
		_flags = shaderDesc._flags;
	}
}
