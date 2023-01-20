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
		Shader(const UINT64& guid, const ShaderType type) :
			EntityBase(guid)
			, _shaderType(type)
		{

		}

		struct ConstantBufferInfoDesc
		{
			struct ConstantBufferVariable
			{
				const char* _name;           // Name of the constant buffer
				UINT32 _startOffset;
				UINT32 _size;
			};

			const char* _name;           // Name of the constant buffer
			std::vector<ConstantBufferVariable> _variables;      // Number of member variables
			UINT32                    _size;
		};


	protected:
		ShaderType _shaderType;

		std::vector<ConstantBufferInfoDesc> _constantBufferInfos;

	public:
		[[nodiscard]] const std::vector<ConstantBufferInfoDesc>& GetConstantBufferInfos()
		{
			return _constantBufferInfos;
		}

		[[nodiscard]] ShaderType GetShaderType() const
		{
			return _shaderType;
		}
	};

}