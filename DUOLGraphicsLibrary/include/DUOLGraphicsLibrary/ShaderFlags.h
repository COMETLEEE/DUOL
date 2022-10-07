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

		ShaderType _type = ShaderType::UNKNOWN;

		const char* _source = nullptr;



	};



}