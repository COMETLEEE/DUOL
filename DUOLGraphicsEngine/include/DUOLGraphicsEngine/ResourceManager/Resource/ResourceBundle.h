#pragma once
#include <unordered_map>
#include "DUOLGraphicsEngine/ResourceManager//Resource/ResourceBundle.h"
#include "DUOLGraphicsLibrary/Renderer/Texture.h"
#include "DUOLGraphicsLibrary/Renderer/Buffer.h"
#include "DUOLCommon/StringHelper.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/Material.h"

namespace DUOLGraphicsEngine
{
	struct ResourceBundle
	{
		std::unordered_map<DUOLCommon::tstring, DUOLGraphicsLibrary::Texture*> _textures;

		std::unordered_map<DUOLCommon::tstring, DUOLGraphicsEngine::Material> _materials;

		std::unordered_map<DUOLCommon::tstring, DUOLGraphicsLibrary::Buffer*> _buffers;
	};
}
