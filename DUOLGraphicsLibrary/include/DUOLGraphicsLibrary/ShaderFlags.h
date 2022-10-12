#pragma once

namespace DUOLGraphicsLibrary
{
	enum class ShaderType
	{
		UNKNOWN,
		VERTEX,
		HULL,
		DOMAINS,
		GEOMETRY,
		PIXEL,
		COMPUTE
	};

	struct ShaderDesc
	{
		ShaderDesc() :
			_type(ShaderType::UNKNOWN)
			, _source(nullptr)
		{

		}

		~ShaderDesc() = default;

		ShaderDesc(const ShaderDesc& shaderDesc) = default;

		ShaderType _type;

		const char* _source = nullptr;
	};
}