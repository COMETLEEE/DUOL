#pragma once
#include "DUOLGraphicsLibrary/Renderer/Resource.h"
#include "DUOLGraphicsLibrary/TextureFlags.h"

namespace DUOLGraphicsLibrary
{

	class Texture : public Resource
	{

		DUOLGRAPHICSLIBRARY_DECLARE_ENTITY(EntityID::Texture);

	protected:

		TextureDesc _textureDesc;

	public:

		TextureDesc GetTextureDesc() const  { return _textureDesc; }
	};

}
