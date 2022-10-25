#pragma once
#include "DUOLGraphicsLibrary/Renderer/Resource.h"
#include "DUOLGraphicsLibrary/TextureFlags.h"

namespace DUOLGraphicsLibrary
{
 /**

     @class   Texture
     @brief   파이프라인에 바인딩될 리소스 중 텍스쳐
     @details ~
     @author  KyungMin Oh

 **/
	class DUOLGRAPHICSLIBRARY_EXPORT Texture : public Resource
	{
		DUOLGRAPHICSLIBRARY_DECLARE_ENTITY(EntityID::Texture);

	protected:
		Texture(const TextureType textureType, long bindFlags);

	protected:
		TextureType _textureType;

		long _bindFlags;

	public:
		virtual ResourceType GetResourceType() override;

		long GetBindFlags() const { return _bindFlags; }

		TextureType GetTextureDesc() const  { return _textureType; }
	};

}
