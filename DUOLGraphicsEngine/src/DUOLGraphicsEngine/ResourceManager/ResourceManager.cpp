#include "ResourceManager.h"
#include "DUOLGraphicsLibrary/Renderer/Renderer.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/ResourceBundle.h"
#include "Resource/Mesh.h"

namespace DUOLGraphicsEngine
{

	DUOLGraphicsLibrary::Texture* ResourceManager::LoadTexture(const DUOLCommon::tstring& objectID,
		const DUOLGraphicsLibrary::TextureDesc& textureDesc)
	{
		/*wchar든 char든 캐스팅을 한번 거친다..뭐가 들어올지 모르니까
		필요한건 char string*/
		auto foundTexture = _textures.find(objectID);

		if (foundTexture != _textures.end())
		{
			return foundTexture->second;
		}

		auto texture = _renderer->CreateTexture(textureDesc);

		_textures.emplace(objectID, texture);

		return texture;

	}

	Mesh* ResourceManager::LoadMesh(const DUOLCommon::tstring& objectID, const DUOLCommon::tstring& path)
	{
		auto foundMesh = _meshes.find(objectID);

		if (foundMesh != _meshes.end())
		{
			return foundMesh->second;
		}

		Mesh* mesh = new Mesh;

		//mesh->_vertexBuffer = CreateEmptyBuffer();

		_meshes.emplace(objectID, mesh);

		return mesh;
	}


	DUOLGraphicsLibrary::Buffer* ResourceManager::CreateEmptyBuffer(const DUOLCommon::tstring& objectID,
		const DUOLGraphicsLibrary::BufferDesc& bufferDesc)
	{
		auto buffer = _renderer->CreateBuffer(bufferDesc);

		_buffers.emplace(objectID, buffer);

		return buffer;
	}

	Material* ResourceManager::RegistMaterial(const DUOLCommon::tstring& objectID, const MaterialDesc& materialDesc)
	{
		//Material material;
		//
		//auto foundTexture = _textures.find(materialDesc._albedoMap);
		//
		//if(foundTexture == _textures.end())
		//{
		//	//map의 id 만으로 오브젝트의 path를 찾을 수 있는 수단이 필요하다.
		//	//만약에 없다면 error / assert 발생시키자.
		//}
		//
		//material._albedoMap = foundTexture->second;
		//
		return nullptr;
	}
}
