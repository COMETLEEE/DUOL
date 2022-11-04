#include "ResourceManager.h"
#include "DUOLGraphicsLibrary/Renderer/Renderer.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/ResourceBundle.h"
#include "Resource/Mesh.h"

namespace DUOLGraphicsEngine
{

	DUOLGraphicsLibrary::Texture* ResourceManager::LoadTexture(const DUOLCommon::tstring& objectID,
		const DUOLGraphicsLibrary::TextureDesc& textureDesc)
	{
		/*wchar�� char�� ĳ������ �ѹ� ��ģ��..���� ������ �𸣴ϱ�
		�ʿ��Ѱ� char string*/
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
		//	//map�� id ������ ������Ʈ�� path�� ã�� �� �ִ� ������ �ʿ��ϴ�.
		//	//���࿡ ���ٸ� error / assert �߻���Ű��.
		//}
		//
		//material._albedoMap = foundTexture->second;
		//
		return nullptr;
	}
}
