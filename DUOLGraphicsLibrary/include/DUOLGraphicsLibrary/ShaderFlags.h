#pragma once
#include "Export.h"
#include "DUOLGraphicsLibrary/ResourceFormat.h"
#include <vector>
#include <string>

namespace DUOLGraphicsLibrary
{
	enum class ShaderType
	{
		UNKNOWN = 0,
		VERTEX = (1 << 0),
		HULL = (1 << 1),
		DOMAINS = (1 << 2),
		GEOMETRY = (1 << 3),
		PIXEL = (1 << 4),
		COMPUTE = (1 << 5)
	};

	enum class ShaderFlags
	{
		UNKNOWN = 0,
		GEOMETRY_STREAM_OUT = (1 << 0),
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

	enum class INPUT_CLASSIFICATION
	{
		VertexData = 0
		,InstanceData = 1
	};

	struct DUOLGRAPHICSLIBRARY_EXPORT InputLayoutAttribute
	{
		InputLayoutAttribute():
			_semanticName()
			,_semanticIndex(0)
			, _format(ResourceFormat::FORMAT_UNKNOWN)
			, _inputSlot(0)
			//, _aligendByteOffset(0)
			, _inputSlotType(INPUT_CLASSIFICATION::VertexData)
			, _instanceStepRate(0)
		{
		}

		std::string _semanticName;

		int _semanticIndex;

		ResourceFormat _format;

		int _inputSlot;

		//int _aligendByteOffset;

		INPUT_CLASSIFICATION _inputSlotType;

		int _instanceStepRate;
	};

	struct DUOLGRAPHICSLIBRARY_EXPORT ShaderDesc
	{
		ShaderDesc() :
			_type(ShaderType::UNKNOWN)
			////, _source()
			, _entryPoint()
			, _profile()
			, _shaderMacro()
			, _useStreamOut(false)
			, _useShaderReflection(true)
			, _flags(0)
		{

		}

		~ShaderDesc() = default;

		ShaderDesc(const ShaderDesc& shaderDesc);

		ShaderType _type;

		char _source[256];

		char _entryPoint[256];

		char _profile[256];

		std::vector<ShaderMacroDesc> _shaderMacro;

		// if vertex Buffer, default input layout is created by shader Reflection;
		bool _useShaderReflection;

		std::vector<InputLayoutAttribute> _inputLayout;

		//if use streamOut in Geometry Shader
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
		_useShaderReflection = shaderDesc._useShaderReflection;
		_flags = shaderDesc._flags;
	}
}
