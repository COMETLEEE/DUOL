#pragma once
#include <memory>
#include <unordered_map>
#include "DUOLCommon/StringHelper.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/Material.h"
#include "DUOLGraphicsLibrary/BufferFlags.h"
#include "DUOLGraphicsLibrary/TextureFlags.h"

namespace DUOLGraphicsLibrary
{
	class Buffer;
	class Renderer;
	class Sampler;
}

namespace DUOLGraphicsEngine
{
	struct ResourceBundle;

	struct Mesh;

	class ResourceManager
	{
	public:
		ResourceManager(DUOLGraphicsLibrary::Renderer* renderer) :
			_renderer(renderer)
		{

		}

		~ResourceManager() = default;

	private:
		DUOLGraphicsLibrary::Renderer* _renderer;

		//sceneID / ResourceBundle of Scene
		//std::unordered_map<DUOLCommon::tstring, std::unique_ptr<ResourceBundle>> _resourceBundles;

		std::unordered_map<DUOLCommon::tstring, DUOLGraphicsLibrary::Buffer*> _buffers;

		std::unordered_map<DUOLCommon::tstring, DUOLGraphicsLibrary::Texture*> _textures;

		std::unordered_map<DUOLCommon::tstring, Material*> _materials;

		std::unordered_map<DUOLCommon::tstring, DUOLGraphicsLibrary::Sampler*> _sampler;

		std::unordered_map<DUOLCommon::tstring, Mesh*> _meshes;

	private:
		


	public:
		DUOLGraphicsLibrary::Texture* LoadTexture(const DUOLCommon::tstring& objectID, const DUOLGraphicsLibrary::TextureDesc& textureDesc);

		Mesh* LoadMesh(const DUOLCommon::tstring& objectID, const DUOLCommon::tstring& path);

		DUOLGraphicsLibrary::Buffer* CreateEmptyBuffer(const DUOLCommon::tstring& objectID, const DUOLGraphicsLibrary::BufferDesc& bufferDesc);

		Material* RegistMaterial(const DUOLCommon::tstring& objectID,const MaterialDesc& materialDesc);


	};
}
