#pragma once
#include "DUOLGraphicsLibrary/EntityBase.h"
#include "DUOLGraphicsLibrary/ShaderFlags.h"

namespace DUOLGraphicsLibrary
{

	class DUOLGRAPHICSLIBRARY_EXPORT Shader : public EntityBase
	{
		DUOLGRAPHICSLIBRARY_DECLARE_ENTITY(EntityID::Shader);
	public:
		Shader(const ShaderType type):
			_shaderType(type)
		{
			
		}

	protected:
		ShaderType _shaderType;

	public:
		ShaderType GetShaderType() const
		{
			return _shaderType;
		}
	};

}