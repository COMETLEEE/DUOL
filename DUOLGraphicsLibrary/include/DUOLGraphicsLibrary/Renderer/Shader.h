#pragma once
#include <vector>
#include "DUOLGraphicsLibrary/EntityBase.h"
#include "DUOLGraphicsLibrary/ShaderFlags.h"

namespace DUOLGraphicsLibrary
{

 /**

     @class   Shader
     @brief	  파이프라인에 바인딩 될 쉐이더
     @details ~
     @author  KyungMin Oh

 **/
	class DUOLGRAPHICSLIBRARY_EXPORT Shader : public EntityBase
	{
		DUOLGRAPHICSLIBRARY_DECLARE_ENTITY(EntityID::Shader);
	public:
		Shader(const UINT64& guid, const ShaderType type):
			EntityBase(guid)
			, _shaderType(type)
		{
			
		}

		struct ConstantBufferInfoDesc
		{
			const char*               _name;           // Name of the constant buffer
			UINT32                    _variables;      // Number of member variables
			UINT32                    _size;

		};

	protected:
		ShaderType _shaderType;

	public:
		ShaderType GetShaderType() const
		{
			return _shaderType;
		}
	};

}