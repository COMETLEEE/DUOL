#pragma once
#include "DUOLGraphicsLibrary/Renderer/Resource.h"
#include "DUOLGraphicsLibrary/TextureFlags.h"

namespace DUOLGraphicsLibrary
{
 /**

     @class   Texture
     @brief   ���������ο� ���ε��� ���ҽ� �� �ؽ���
     @details ~
     @author  KyungMin Oh

 **/
	class DUOLGRAPHICSLIBRARY_EXPORT Texture : public Resource
	{
		DUOLGRAPHICSLIBRARY_DECLARE_ENTITY(EntityID::Texture);

	protected:
		Texture(const UINT64& guid, const TextureDesc& _textureDesc):
			Resource(guid)
			, _textureDesc(_textureDesc)
		{

		}

		virtual ~Texture() = default;

	protected:
		TextureDesc _textureDesc;

	public:
		virtual ResourceType GetResourceType() override { return ResourceType::TEXTURE; };

		const TextureDesc& GetTextureDesc() { return _textureDesc; }

		virtual void* GetShaderResourceAddress() abstract;
	};

}
