#pragma once
#include "DUOLGraphicsLibrary/Renderer/Resource.h"
#include "DUOLGraphicsLibrary/TextureFlags.h"

namespace DUOLGraphicsLibrary
{

	class DUOLGRAPHICSLIBRARY_EXPORT Texture : public Resource
	{
		DUOLGRAPHICSLIBRARY_DECLARE_ENTITY(EntityID::Texture);

	protected:
		Texture(const TextureType textureType, long bindFlags);

	protected:
		TextureType _textureType;

		long _bindFlags;

	public:
		long GetBindFlags() const { return _bindFlags; }

		TextureType GetTextureDesc() const  { return _textureType; }
	};

}
