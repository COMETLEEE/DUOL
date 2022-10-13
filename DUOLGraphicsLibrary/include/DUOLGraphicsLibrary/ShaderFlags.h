#pragma once

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