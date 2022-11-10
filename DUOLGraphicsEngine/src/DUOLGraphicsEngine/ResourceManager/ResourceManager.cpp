#include "ResourceManager.h"
#include "DUOLGraphicsLibrary/Renderer/Renderer.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/ResourceBundle.h"
#include "DUOLGraphicsEngine/Util/Hash/Hash.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/Mesh.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/Material.h"
#include "ParserBase/DUOLParserBase.h"
#include "DUOLFBXParser/DUOLFBXData.h"

namespace DUOLGraphicsEngine
{
	ResourceManager::ResourceManager(DUOLGraphicsLibrary::Renderer* renderer) :
		_renderer(renderer)
	{
		_parser = DUOLParser::DUOLParserBase::Create();

		//예약
		_buffers.reserve(32);
		_textures.reserve(32);
		_materials.reserve(32);
		_meshes.reserve(32);
		_samplers.reserve(32);
	}

	DUOLGraphicsLibrary::Texture* ResourceManager::LoadMaterialTexture(const DUOLCommon::tstring& path, DUOLCommon::tstring& fileID)
	{
		fileID = path.substr(path.find_last_of(_T("/")) + 1);
		UINT64 HashfileID = Hash::Hash64(fileID);
		std::string FilePath(path.begin(), path.end());

		DUOLGraphicsLibrary::TextureDesc albedo;
		albedo._texturePath = FilePath.c_str();

		auto texture = _renderer->CreateTexture(HashfileID, albedo);
		_textures.emplace(HashfileID, texture);

		return texture;
	}

	DUOLGraphicsLibrary::Texture* ResourceManager::CreateTexture(const DUOLCommon::tstring& objectID,
		const DUOLGraphicsLibrary::TextureDesc& textureDesc)
	{
		auto keyValue = Hash::Hash64(objectID);

		auto foundTexture = _textures.find(keyValue);

		if (foundTexture != _textures.end())
		{
			return foundTexture->second;
		}

		auto texture = _renderer->CreateTexture(keyValue, textureDesc);

		_textures.emplace(keyValue, texture);

		return texture;
	}

	DUOLGraphicsLibrary::Texture* ResourceManager::CreateTexture(const UINT64& objectID,
		const DUOLGraphicsLibrary::TextureDesc& textureDesc)
	{
		auto foundTexture = _textures.find(objectID);

		if (foundTexture != _textures.end())
		{
			return foundTexture->second;
		}

		auto texture = _renderer->CreateTexture(objectID, textureDesc);

		_textures.emplace(objectID, texture);

		return texture;
	}

	DUOLGraphicsLibrary::Texture* ResourceManager::GetTexture(const DUOLCommon::tstring& objectID)
	{
		auto keyValue = Hash::Hash64(objectID);

		const auto foundTexture = _textures.find(keyValue);

		if (foundTexture != _textures.end())
		{
			return foundTexture->second;
		}

		return nullptr;
	}

	DUOLGraphicsLibrary::Texture* ResourceManager::GetTexture(const UINT64& objectID)
	{
		auto foundTexture = _textures.find(objectID);

		if (foundTexture != _textures.end())
		{
			return foundTexture->second;
		}

		return nullptr;
	}

	Mesh* ResourceManager::CreateMesh(const DUOLCommon::tstring& objectID, const DUOLCommon::tstring& path)
	{
		auto keyValue = Hash::Hash64(objectID);

		auto foundMesh = _meshes.find(keyValue);

		if (foundMesh != _meshes.end())
		{
			return foundMesh->second;
		}

		Mesh* mesh = new Mesh;

		//tstring to string cast
		std::string strPath(path.begin(), path.end());

		auto meshInfo = _parser->LoadFBX(strPath);
		int meshSize = meshInfo->fbxMeshList.size();

		mesh->_submeshCount = meshSize;
		mesh->_subMesh.reserve(meshSize);

		for (int meshIndex = 0; meshIndex < meshSize; meshIndex++)
		{
			{
				DUOLCommon::tstring subMeshID = objectID + std::to_wstring(meshIndex);
				SubMesh subMesh;

				subMesh._submeshIndex = meshIndex;

				DUOLCommon::tstring strVertexID = subMeshID + (_T("Vertex"));
				DUOLCommon::tstring strIndexID = subMeshID + (_T("Index"));

				auto verticeSize = meshInfo->fbxMeshList[meshIndex]->vertexList.size();

				DUOLGraphicsLibrary::BufferDesc vetexBufferDesc;

				vetexBufferDesc._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::VERTEXBUFFER);
				vetexBufferDesc._usage = DUOLGraphicsLibrary::ResourceUsage::USAGE_DEFAULT;
				vetexBufferDesc._stride = sizeof(DuolData::Vertex);
				vetexBufferDesc._size = vetexBufferDesc._stride * verticeSize;

				auto vertexId = Hash::Hash64(strVertexID);
				subMesh._vertexBuffer = _renderer->CreateBuffer(vertexId, vetexBufferDesc, meshInfo->fbxMeshList[meshIndex]->vertexList.data());

				auto IndexSize = meshInfo->fbxMeshList[meshIndex]->indices.size();

				DUOLGraphicsLibrary::BufferDesc indexBufferDesc;

				indexBufferDesc._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::INDEXBUFFER);
				indexBufferDesc._usage = DUOLGraphicsLibrary::ResourceUsage::USAGE_DEFAULT;
				indexBufferDesc._stride = sizeof(unsigned int);
				indexBufferDesc._size = indexBufferDesc._stride * IndexSize;
				indexBufferDesc._format = DUOLGraphicsLibrary::ResourceFormat::FORMAT_R32_UINT;

				auto indexID = Hash::Hash64(strIndexID);
				subMesh._indexBuffer = _renderer->CreateBuffer(indexID, indexBufferDesc, meshInfo->fbxMeshList[meshIndex]->indices.data());

				mesh->_subMesh.emplace_back(std::move(subMesh));
			}
			{
				auto& materialInfo = meshInfo->fbxmaterialList[meshIndex];

				MaterialDesc materialDesc;

				DUOLCommon::tstring materialName(materialInfo->materialName.begin(), meshInfo->fbxmaterialList[meshIndex]->materialName.end());

				const DUOLCommon::tstring defaultPath = _T("Asset/Texture/");
				if (materialInfo->isAlbedo)
				{
					LoadMaterialTexture(defaultPath+materialInfo->albedoMap, materialDesc._albedoMap);
				}
				if (materialInfo->isNormal)
				{
					LoadMaterialTexture(defaultPath + materialInfo->normalMap, materialDesc._normalMap);
				}

				materialDesc._renderPass = _T("default");
				RegistMaterial(materialName, materialDesc);
			}
		}

		_meshes.emplace(keyValue, mesh);

		return mesh;
	}

	Mesh* ResourceManager::CreateMesh(const UINT64& objectID, const DUOLCommon::tstring& path)
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
		auto keyValue = Hash::Hash64(objectID);

		//있나 없나 체크
		auto foundBuffer = _buffers.find(keyValue);

		if (foundBuffer != _buffers.end())
		{
			return foundBuffer->second;
		}

		auto buffer = _renderer->CreateBuffer(keyValue, bufferDesc);

		_buffers.emplace(keyValue, buffer);

		return buffer;
	}

	DUOLGraphicsLibrary::Buffer* ResourceManager::CreateEmptyBuffer(const UINT64& objectID,
		const DUOLGraphicsLibrary::BufferDesc& bufferDesc)
	{
		//있나 없나 체크
		auto foundBuffer = _buffers.find(objectID);

		if (foundBuffer != _buffers.end())
		{
			return foundBuffer->second;
		}

		auto buffer = _renderer->CreateBuffer(objectID, bufferDesc);

		_buffers.emplace(objectID, buffer);

		return buffer;
	}

	DUOLGraphicsLibrary::Buffer* ResourceManager::GetBuffer(const DUOLCommon::tstring& objectID)
	{
		auto keyValue = Hash::Hash64(objectID);

		//있나 없나 체크
		auto foundBuffer = _buffers.find(keyValue);

		if (foundBuffer != _buffers.end())
		{
			return foundBuffer->second;
		}

		return nullptr;
	}

	DUOLGraphicsLibrary::Buffer* ResourceManager::GetBuffer(const UINT64& objectID)
	{
		//있나 없나 체크
		auto foundBuffer = _buffers.find(objectID);

		if (foundBuffer != _buffers.end())
		{
			return foundBuffer->second;
		}

		return nullptr;
	}

	DUOLGraphicsLibrary::RenderTarget* ResourceManager::CreateRenderTarget(const DUOLGraphicsLibrary::RenderTargetDesc& renderTargetDesc)
	{
		auto guid = renderTargetDesc._texture->GetGUID();

		auto foundObject = _renderTargets.find(guid);

		if (foundObject != _renderTargets.end())
		{
			return foundObject->second;
		}

		auto renderTarget = _renderer->CreateRenderTarget(guid, renderTargetDesc);

		_renderTargets.emplace(guid, renderTarget);

		return renderTarget;
	}

	DUOLGraphicsLibrary::Shader* ResourceManager::CreateShader(const UINT64& objectID,
		const DUOLGraphicsLibrary::ShaderDesc& shaderDesc)
	{
		auto foundObject = _shaders.find(objectID);

		if (foundObject != _shaders.end())
		{
			return foundObject->second;
		}

		auto shader = _renderer->CreateShader(objectID, shaderDesc);

		_shaders.emplace(objectID, shader);

		return shader;
	}

	DUOLGraphicsLibrary::RenderPass* ResourceManager::CreateRenderPass(const UINT64& objectID,
		const DUOLGraphicsLibrary::RenderPassDesc& renderPassDesc)
	{
		auto foundObject = _renderPasses.find(objectID);

		if (foundObject != _renderPasses.end())
		{
			return foundObject->second;
		}

		auto renderPass = _renderer->CreateRenderPass(objectID, renderPassDesc);

		_renderPasses.emplace(objectID, renderPass);

		return renderPass;
	}

	DUOLGraphicsLibrary::PipelineState* ResourceManager::CreatePipelineState(const UINT64& objectID,
		const DUOLGraphicsLibrary::PipelineStateDesc& pipelineStateDesc)
	{
		auto foundObject = _pipelineStates.find(objectID);

		if (foundObject != _pipelineStates.end())
		{
			return foundObject->second;
		}

		auto pipelineState = _renderer->CreatePipelineState(objectID, pipelineStateDesc);

		_pipelineStates.emplace(objectID, pipelineState);

		return pipelineState;
	}

	DUOLGraphicsLibrary::Sampler* ResourceManager::CreateSampler(const UINT64& objectID,
		const DUOLGraphicsLibrary::SamplerDesc& samplerDesc)
	{
		auto foundObject = _samplers.find(objectID);

		if (foundObject != _samplers.end())
		{
			return foundObject->second;
		}

		auto sampler = _renderer->CreateSampler(objectID, samplerDesc);

		_samplers.emplace(objectID, sampler);

		return sampler;
	}


	Material* ResourceManager::RegistMaterial(const DUOLCommon::tstring& objectID, const MaterialDesc& materialDesc)
	{
		auto foundMaterial = _materials.find(Hash::Hash64(objectID));

		if (foundMaterial != _materials.end())
		{
			return foundMaterial->second;
		}

		Material* material = new Material;

		material->_albedoMap = GetTexture(materialDesc._albedoMap);
		material->_normalMap = GetTexture(materialDesc._normalMap);
		material->_metalicSmoothnessMap = GetTexture(materialDesc._metalicSmoothnessMap);
		
		auto foundObj = _pipelineStates.find(Hash::Hash64(materialDesc._renderPass));
		if (foundObj != _pipelineStates.end())
		{
			material->_shaders = foundObj->second;
		}

		_materials.emplace(Hash::Hash64(objectID), material);

		return material;
	}

	Material* ResourceManager::GetMaterial(const DUOLCommon::tstring& objectID)
	{
		auto foundMaterial = _materials.find(Hash::Hash64(objectID));

		if (foundMaterial != _materials.end())
		{
			return foundMaterial->second;
		}

		return nullptr;
	}
}
