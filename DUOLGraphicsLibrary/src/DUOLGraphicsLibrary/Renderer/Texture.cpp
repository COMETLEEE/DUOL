#include "DUOLGraphicsLibrary/Renderer/Texture.h"


namespace  DUOLGraphicsLibrary
{
	Texture::~Texture()
	{
	}

	void* Texture::GetShaderResourceAddress()
	{
		return nullptr;
	}

	void Texture::SetTextureDesc(std::string path)
	{
		_textureDesc._texturePath = path.c_str();
	}

}
