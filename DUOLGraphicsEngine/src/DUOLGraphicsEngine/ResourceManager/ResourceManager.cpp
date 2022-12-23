#include "ResourceManager.h"
#include "DUOLGraphicsLibrary/Renderer/Renderer.h"
#include "DUOLGraphicsEngine/Util/Hash/Hash.h"

#include "ParserBase/DUOLParserBase.h"
#include "DUOLFBXParser/DUOLFBXData.h"

#include "DUOLGraphicsEngine/ResourceManager/Resource/RenderConstantBuffer.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/RenderObject.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/Vertex.h"

namespace DUOLGraphicsEngine
{
	ResourceManager::ResourceManager(DUOLGraphicsLibrary::Renderer* renderer) :
		_renderer(renderer)
	{
		_parser = DUOLParser::DUOLParserBase::Create();

		DUOLGraphicsLibrary::BufferDesc perFrameBufferDesc;
		perFrameBufferDesc._size = sizeof(ConstantBufferPerFrame);
		perFrameBufferDesc._usage = DUOLGraphicsLibrary::ResourceUsage::USAGE_DYNAMIC;
		perFrameBufferDesc._format = DUOLGraphicsLibrary::ResourceFormat::FORMAT_UNKNOWN;
		perFrameBufferDesc._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::CONSTANTBUFFER);
		perFrameBufferDesc._cpuAccessFlags = static_cast<long>(DUOLGraphicsLibrary::CPUAccessFlags::WRITE);

		_perFrameBuffer = CreateEmptyBuffer(_T("perFrameBuffer"), perFrameBufferDesc);

		DUOLGraphicsLibrary::BufferDesc perObjectBufferDesc;
		perObjectBufferDesc._size = sizeof(DUOLMath::Matrix) * 512;
		perObjectBufferDesc._usage = DUOLGraphicsLibrary::ResourceUsage::USAGE_DYNAMIC;
		perObjectBufferDesc._format = DUOLGraphicsLibrary::ResourceFormat::FORMAT_UNKNOWN;
		perObjectBufferDesc._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::CONSTANTBUFFER);
		perObjectBufferDesc._cpuAccessFlags = static_cast<long>(DUOLGraphicsLibrary::CPUAccessFlags::WRITE);

		_perObjectBuffer = CreateEmptyBuffer(_T("perObjectBuffer"), perObjectBufferDesc);

		//����
		_buffers.reserve(32);
		_textures.reserve(32);
		_materials.reserve(32);
		_meshes.reserve(32);
		_samplers.reserve(32);
	}

	ResourceManager::~ResourceManager()
	{

	}

	DUOLGraphicsLibrary::Texture* ResourceManager::LoadMaterialTexture(const DUOLCommon::tstring& path, DUOLCommon::tstring& fileID)
	{
		fileID = path.substr(path.find_last_of(_T("/")) + 1);
		UINT64 HashfileID = Hash::Hash64(fileID);

		const std::string filePath = DUOLCommon::StringHelper::ToString(path);

		DUOLGraphicsLibrary::TextureDesc albedo;
		albedo._texturePath = filePath.c_str();

		auto texture = _renderer->CreateTexture(HashfileID, albedo);
		_textures.emplace(HashfileID, texture);

		return texture;
	}

	void ResourceManager::OnResize(const DUOLMath::Vector2& resolution)
	{
		for (auto& rendertarget : _proportionalRenderTarget)
		{
			_renderer->SetResolution(*rendertarget.second._renderTarget, resolution);
		}
	}

	void ResourceManager::ClearRenderTargets()
	{
		for (auto& renderTarget : _renderTargets)
		{
			_renderer->ClearRenderTarget(*renderTarget.second);;
		}
	}

	void ResourceManager::AddBackbufferRenderTarget(DUOLGraphicsLibrary::RenderTarget* backbuffer)
	{
		UINT64 backbufferID = Hash::Hash64(_T("BackBuffer"));

		_renderTargets.emplace(backbufferID, backbuffer);
	}

	void ResourceManager::CreateDebugMaterial()
	{
		MaterialDesc debugMat;
		debugMat._pipelineState = _T("Debug");

		auto ret = RegistMaterial(_T("Debug"), debugMat);
	}

	void ResourceManager::CreateParticleMaterial()
	{
		DUOLMath::Vector4 randomValues[1024];

		for (int i = 0; i < 1024; ++i)
		{
			randomValues[i].x = DUOLMath::MathHelper::RandF(-1.0f, 1.0f);
			randomValues[i].y = DUOLMath::MathHelper::RandF(-1.0f, 1.0f);
			randomValues[i].z = DUOLMath::MathHelper::RandF(-1.0f, 1.0f);
			randomValues[i].w = DUOLMath::MathHelper::RandF(-1.0f, 1.0f);
		}

		DUOLGraphicsLibrary::TextureDesc textureDesc;

		textureDesc._initData = randomValues;
		textureDesc._type = DUOLGraphicsLibrary::TextureType::TEXTURE1D;
		textureDesc._size = 1024 * sizeof(DUOLMath::Vector4);
		textureDesc._mipLevels = 1;
		textureDesc._textureExtent = DUOLMath::Vector3{1024.f, 0.f, 0.f};
		textureDesc._usage = DUOLGraphicsLibrary::ResourceUsage::USAGE_IMMUTABLE;
		textureDesc._format = DUOLGraphicsLibrary::ResourceFormat::FORMAT_R32G32B32A32_FLOAT;
		textureDesc._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE);
		textureDesc._cpuAccessFlags = 0;
		textureDesc._arraySize = 1;

		CreateTexture(_T("EffectRandomMap"), textureDesc);

		MaterialDesc particleMat;
		particleMat._pipelineState = _T("StreamOut");

		particleMat._albedoMap = _T("EffectRandomMap");

		auto ret = RegistMaterial(_T("StreamOut"), particleMat);
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

	Model* ResourceManager::CreateModelFromFBX(const DUOLCommon::tstring& objectID, const DUOLCommon::tstring& path)
	{
		auto keyValue = Hash::Hash64(objectID);
		auto foundModel = _models.find(keyValue);

		if (foundModel != _models.end())
		{
			return foundModel->second.get();
		}

		//tstring to string cast
		std::string strPath = DUOLCommon::StringHelper::ToString(path);

		auto modelInfo = _parser->LoadFBX(strPath);
		int meshSize = modelInfo->fbxMeshList.size();

		Model* model = new Model;

		model->SetMeshCount(meshSize);
		for (int meshIndex = 0; meshIndex < meshSize; meshIndex++)
		{
			auto& meshInfo = modelInfo->fbxMeshList[meshIndex];
			auto meshName = DUOLCommon::StringHelper::ToTString(meshInfo->nodeName);

			auto mesh = GetMesh(meshName);

			if (mesh == nullptr)
			{
				mesh = CreateMesh(meshName, meshInfo);
			}

			model->AddMesh(mesh);
		}

		_models.emplace(keyValue, model);

		//bone
		{
			int boneSize = modelInfo->fbxBoneList.size();

			if (boneSize > 0)
			{
				auto& bones = model->GetBones();
				bones.resize(boneSize);

				for (int boneIndex = 0; boneIndex < boneSize; boneIndex++)
				{
					bones[boneIndex]._boneName = DUOLCommon::StringHelper::ToWString(modelInfo->fbxBoneList[boneIndex]->boneName);
					bones[boneIndex]._parentIndex = modelInfo->fbxBoneList[boneIndex]->parentIndex;
					bones[boneIndex]._nodeMatrix = modelInfo->fbxBoneList[boneIndex]->nodeMatrix;
					bones[boneIndex]._offsetMatrix = modelInfo->fbxBoneList[boneIndex]->offsetMatrix;
				}

				model->SetIsSkinningModel(true);
			}
		}

		//material
		for (int materialIndex = 0; materialIndex < modelInfo->fbxmaterialList.size(); materialIndex++)
		{
			//material
			auto& materialInfo = modelInfo->fbxmaterialList[materialIndex];

			MaterialDesc materialDesc;

			DUOLCommon::tstring materialName(materialInfo->materialName.begin(), materialInfo->materialName.end());

			const DUOLCommon::tstring defaultPath = _T("Asset/Texture/");
			if (materialInfo->isAlbedo)
			{
				LoadMaterialTexture(defaultPath + materialInfo->albedoMap, materialDesc._albedoMap);
			}
			if (materialInfo->isNormal)
			{
				LoadMaterialTexture(defaultPath + materialInfo->normalMap, materialDesc._normalMap);
			}
			if (materialInfo->isMetallic || materialInfo->isRoughness)
			{
				LoadMaterialTexture(defaultPath + materialInfo->roughnessMap, materialDesc._metallicSmoothnessMap);
			}

			if (model->IsSkinningModel())
			{
				if (materialInfo->isAlbedo && materialInfo->isNormal && (materialInfo->isMetallic || materialInfo->isRoughness))
				{
					materialDesc._pipelineState = _T("SkinnedAlbedoNormalMRA");
				}
				else if (materialInfo->isAlbedo && materialInfo->isNormal)
				{
					materialDesc._pipelineState = _T("SkinnedAlbedoNormal");
				}
				else if (materialInfo->isAlbedo)
				{
					materialDesc._pipelineState = _T("SkinnedAlbedo");
				}
				else
				{
					materialDesc._pipelineState = _T("SkinnedDefault");
				}
			}
			else
			{
				if (materialInfo->isAlbedo && materialInfo->isNormal && (materialInfo->isMetallic || materialInfo->isRoughness))
				{
					materialDesc._pipelineState = _T("AlbedoNormalMRA");
				}
				else if (materialInfo->isAlbedo && materialInfo->isNormal)
				{
					materialDesc._pipelineState = _T("AlbedoNormal");
				}
				else if (materialInfo->isAlbedo)
				{
					materialDesc._pipelineState = _T("Albedo");
				}
				else
				{
					materialDesc._pipelineState = _T("Default");
				}
			}

			RegistMaterial(materialName, materialDesc);
		}

		//anim
		if (modelInfo->isSkinnedAnimation)
		{
			int animationClipSize = modelInfo->animationClipList.size();

			for (int animationClipIndex = 0; animationClipIndex < animationClipSize; animationClipIndex++)
			{
				auto& animaitonClipInfo = modelInfo->animationClipList[animationClipIndex];
				AnimationClip* animationClip = new AnimationClip;

				animationClip->_totalKeyFrame = animaitonClipInfo->totalKeyFrame;
				animationClip->_frameRate = animaitonClipInfo->frameRate;
				animationClip->_startKeyFrame = animaitonClipInfo->startKeyFrame;
				animationClip->_endKeyFrame = animaitonClipInfo->endKeyFrame;
				animationClip->_tickPerFrame = animaitonClipInfo->tickPerFrame;

				int animationFrameSize = animaitonClipInfo->keyframeList.size();
				animationClip->_keyFrameList.reserve(animationFrameSize);
				animationClip->_keyFrameList.resize(animationFrameSize);


				for (int animationKeyFrameIndex = 0; animationKeyFrameIndex < animationFrameSize; animationKeyFrameIndex++)
				{
					auto& animationFrameBonesInfo = modelInfo->animationClipList[animationClipIndex]->keyframeList[animationKeyFrameIndex];
					int animationFrameBoneSize = animationFrameBonesInfo.size();

					animationClip->_keyFrameList[animationKeyFrameIndex].reserve(animationFrameBoneSize);
					animationClip->_keyFrameList[animationKeyFrameIndex].resize(animationFrameBoneSize);

					for (int animationFrameBoneIndex = 0; animationFrameBoneIndex < animationFrameBoneSize; animationFrameBoneIndex++)
					{
						auto& animationFrameBoneInfoOrigin = animationFrameBonesInfo[animationFrameBoneIndex];
						auto& animationFrameBoneInfo = animationClip->_keyFrameList[animationKeyFrameIndex][animationFrameBoneIndex];
						animationFrameBoneInfo._time = animationFrameBoneInfoOrigin->time;
						animationFrameBoneInfo._localScale = animationFrameBoneInfoOrigin->localScale;
						animationFrameBoneInfo._localRotation = animationFrameBoneInfoOrigin->localRotation;
						animationFrameBoneInfo._localTransform = animationFrameBoneInfoOrigin->localTransform;
					}
				}

				DUOLCommon::tstring animName = DUOLCommon::tstring(animaitonClipInfo->animationName.begin(), animaitonClipInfo->animationName.end());

				_animationClips.emplace(Hash::Hash64(animName), animationClip);
			}
		}

		return model;
	}

	MeshBase* ResourceManager::CreateMesh(const DUOLCommon::tstring& objectID, std::shared_ptr<DuolData::Mesh>& meshInfo)
	{
		MeshBase* retMesh = nullptr;

		if (meshInfo->isSkinned)
		{
			SkinnedMesh* mesh = new SkinnedMesh;
			retMesh = mesh;

			mesh->_subMeshCount = 1;
			mesh->_subMeshs.reserve(1);

			DUOLCommon::tstring strVertexID = objectID + (_T("Vertex"));

			auto verticeSize = meshInfo->vertexList.size();

			DUOLGraphicsLibrary::BufferDesc vetexBufferDesc;

			vetexBufferDesc._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::VERTEXBUFFER);
			vetexBufferDesc._usage = DUOLGraphicsLibrary::ResourceUsage::USAGE_DEFAULT;
			vetexBufferDesc._stride = sizeof(DuolData::Vertex);
			vetexBufferDesc._size = vetexBufferDesc._stride * verticeSize;

			auto vertexId = Hash::Hash64(strVertexID);
			mesh->_vertexBuffer = _renderer->CreateBuffer(vertexId, vetexBufferDesc, meshInfo->vertexList.data());

			//����޽��� �ϴ� �ϳ��θ� �Ľ��Ѵ�.
			for (int submeshIndex = 0; submeshIndex < 1; submeshIndex++)
			{
				{
					DUOLCommon::tstring subMeshID = objectID + std::to_wstring(submeshIndex);
					DUOLCommon::tstring strIndexID = subMeshID + (_T("Index"));
					SubMesh subMesh;

					subMesh._submeshIndex = submeshIndex;

					int IndexSize = meshInfo->indices.size();

					DUOLGraphicsLibrary::BufferDesc indexBufferDesc;

					indexBufferDesc._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::INDEXBUFFER);
					indexBufferDesc._usage = DUOLGraphicsLibrary::ResourceUsage::USAGE_DEFAULT;
					indexBufferDesc._stride = sizeof(unsigned int);
					indexBufferDesc._size = indexBufferDesc._stride * IndexSize;
					indexBufferDesc._format = DUOLGraphicsLibrary::ResourceFormat::FORMAT_R32_UINT;

					auto indexID = Hash::Hash64(strIndexID);
					subMesh._indexBuffer = _renderer->CreateBuffer(indexID, indexBufferDesc, meshInfo->indices.data());

					subMesh._drawIndex = IndexSize;

					mesh->_subMeshs.emplace_back(std::move(subMesh));
				}
			}
		}

		{
			Mesh* mesh = new Mesh;
			retMesh = mesh;

			DUOLCommon::tstring strVertexID = objectID + (_T("Vertex"));

			auto verticeSize = meshInfo->vertexList.size();

			DUOLGraphicsLibrary::BufferDesc vetexBufferDesc;

			vetexBufferDesc._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::VERTEXBUFFER);
			vetexBufferDesc._usage = DUOLGraphicsLibrary::ResourceUsage::USAGE_DEFAULT;
			vetexBufferDesc._stride = sizeof(DUOLGraphicsEngine::StaticMeshVertex); //position 3 uv 2 normal 3 tangent 3 binormal 3
			vetexBufferDesc._size = vetexBufferDesc._stride * verticeSize;

			std::vector<StaticMeshVertex> vertices;
			vertices.resize(verticeSize);

			for (int vertexIndex = 0; vertexIndex < verticeSize; vertexIndex++)
			{
				memcpy(&vertices[vertexIndex], &meshInfo->vertexList[vertexIndex], sizeof(DUOLGraphicsEngine::StaticMeshVertex));
			}

			auto vertexId = Hash::Hash64(strVertexID);
			mesh->_vertexBuffer = _renderer->CreateBuffer(vertexId, vetexBufferDesc, vertices.data());

			mesh->_subMeshCount = meshInfo->indices.size();
			mesh->_subMeshs.reserve(mesh->_subMeshCount);

			for (int subMeshIndex = 0; subMeshIndex < mesh->_subMeshCount; subMeshIndex++)
			{
				{
					DUOLCommon::tstring strIndexID = objectID + (_T("Index")) + std::to_wstring(subMeshIndex);

					SubMesh subMesh;

					subMesh._submeshIndex = subMeshIndex;

					int IndexSize = meshInfo->indices[subMeshIndex].size();

					DUOLGraphicsLibrary::BufferDesc indexBufferDesc;

					indexBufferDesc._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::INDEXBUFFER);
					indexBufferDesc._usage = DUOLGraphicsLibrary::ResourceUsage::USAGE_DEFAULT;
					indexBufferDesc._stride = sizeof(unsigned int);
					indexBufferDesc._size = indexBufferDesc._stride * IndexSize;
					indexBufferDesc._format = DUOLGraphicsLibrary::ResourceFormat::FORMAT_R32_UINT;

					auto indexID = Hash::Hash64(strIndexID);
					subMesh._indexBuffer = _renderer->CreateBuffer(indexID, indexBufferDesc, meshInfo->indices[subMeshIndex].data());

					subMesh._drawIndex = IndexSize;

					mesh->_subMeshs.emplace_back(std::move(subMesh));
				}
			}
		}

		retMesh->_meshName = DUOLCommon::StringHelper::ToTString(meshInfo->nodeName);

		_meshes.emplace(Hash::Hash64(objectID), retMesh);

		return retMesh;
	}

	MeshBase* ResourceManager::CreateMesh(const DUOLCommon::tstring& objectID, void* vertices, UINT vertexSize, UINT vertexStructureSize, void* indices,
		UINT indexSize)
	{
		auto keyValue = Hash::Hash64(objectID);

		auto foundMesh = _meshes.find(keyValue);

		if (foundMesh != _meshes.end())
		{
			return foundMesh->second.get();
		}

		MeshBase* mesh = new Mesh;

		DUOLCommon::tstring strVertexID = objectID + (_T("Vertex"));
		auto vertexId = Hash::Hash64(strVertexID);

		DUOLGraphicsLibrary::BufferDesc vetexBufferDesc;

		vetexBufferDesc._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::VERTEXBUFFER);
		vetexBufferDesc._usage = DUOLGraphicsLibrary::ResourceUsage::USAGE_DYNAMIC;
		vetexBufferDesc._stride = vertexStructureSize;
		vetexBufferDesc._size = vertexStructureSize * vertexSize;
		vetexBufferDesc._cpuAccessFlags = static_cast<long>(DUOLGraphicsLibrary::CPUAccessFlags::WRITE);

		mesh->_vertexBuffer = _renderer->CreateBuffer(vertexId, vetexBufferDesc, vertices);

		{
			SubMesh subMesh;

			subMesh._submeshIndex = 0;

			DUOLCommon::tstring strIndexID = objectID + (_T("Index"));

			DUOLGraphicsLibrary::BufferDesc indexBufferDesc;

			indexBufferDesc._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::INDEXBUFFER);
			indexBufferDesc._usage = DUOLGraphicsLibrary::ResourceUsage::USAGE_DYNAMIC;
			indexBufferDesc._stride = sizeof(unsigned int);
			indexBufferDesc._size = indexBufferDesc._stride * indexSize;
			indexBufferDesc._format = DUOLGraphicsLibrary::ResourceFormat::FORMAT_R32_UINT;
			indexBufferDesc._cpuAccessFlags = static_cast<long>(DUOLGraphicsLibrary::CPUAccessFlags::WRITE);

			subMesh._drawIndex = indexSize;

			auto indexID = Hash::Hash64(strIndexID);
			subMesh._indexBuffer = _renderer->CreateBuffer(indexID, indexBufferDesc, indices);

			mesh->_subMeshs.emplace_back(std::move(subMesh));
		}

		_meshes.emplace(keyValue, mesh);

		return mesh;
	}

	MeshBase* ResourceManager::CreateParticleBuffer(const DUOLCommon::tstring& objectID, int maxParticle)
	{
		auto keyValue = Hash::Hash64(objectID);

		auto foundMesh = _meshes.find(keyValue);

		if (foundMesh != _meshes.end())
		{
			return foundMesh->second.get();
		}

		ParticleBuffer* mesh = new ParticleBuffer;

		DUOLCommon::tstring strVertexID = objectID + (_T("ParticleVertex"));
		auto vertexId = Hash::Hash64(strVertexID);

		DUOLGraphicsLibrary::BufferDesc vetexBufferDesc;

		vetexBufferDesc._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::VERTEXBUFFER) | static_cast<long>(DUOLGraphicsLibrary::BindFlags::STREAMOUTPUTBUFFER);
		vetexBufferDesc._usage = DUOLGraphicsLibrary::ResourceUsage::USAGE_DEFAULT;
		vetexBufferDesc._stride = sizeof(DUOLGraphicsEngine::Particle);
		vetexBufferDesc._size = vetexBufferDesc._stride * maxParticle;
		vetexBufferDesc._cpuAccessFlags = 0;

		mesh->_vertexBuffer = _renderer->CreateBuffer(vertexId, vetexBufferDesc, nullptr);

		strVertexID  = objectID + (_T("ParticleStream"));
		vertexId = Hash::Hash64(strVertexID);

		vetexBufferDesc._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::VERTEXBUFFER) | static_cast<long>(DUOLGraphicsLibrary::BindFlags::STREAMOUTPUTBUFFER);
		vetexBufferDesc._usage = DUOLGraphicsLibrary::ResourceUsage::USAGE_DEFAULT;
		vetexBufferDesc._stride = sizeof(DUOLGraphicsEngine::Particle);
		vetexBufferDesc._size = vetexBufferDesc._stride * maxParticle;
		vetexBufferDesc._cpuAccessFlags = 0;

		mesh->_streamOutBuffer = _renderer->CreateBuffer(vertexId, vetexBufferDesc, nullptr);

		strVertexID = (_T("ParticleInit"));
		vertexId = Hash::Hash64(strVertexID);

		vetexBufferDesc._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::VERTEXBUFFER);
		vetexBufferDesc._usage = DUOLGraphicsLibrary::ResourceUsage::USAGE_DEFAULT;
		vetexBufferDesc._stride = sizeof(DUOLGraphicsEngine::Particle);
		vetexBufferDesc._size = vetexBufferDesc._stride;
		vetexBufferDesc._cpuAccessFlags = 0;

		mesh->_initBuffer = _renderer->CreateBuffer(vertexId, vetexBufferDesc, nullptr);

		{
			SubMesh subMesh;

			subMesh._submeshIndex = 0;

			DUOLCommon::tstring strIndexID = objectID + (_T("Index"));

			DUOLGraphicsLibrary::BufferDesc indexBufferDesc;

			indexBufferDesc._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::INDEXBUFFER);
			indexBufferDesc._usage = DUOLGraphicsLibrary::ResourceUsage::USAGE_DYNAMIC;
			indexBufferDesc._stride = sizeof(unsigned int);
			indexBufferDesc._size = indexBufferDesc._stride * 1;
			indexBufferDesc._format = DUOLGraphicsLibrary::ResourceFormat::FORMAT_R32_UINT;
			indexBufferDesc._cpuAccessFlags = static_cast<long>(DUOLGraphicsLibrary::CPUAccessFlags::WRITE);

			subMesh._drawIndex = 1;

			auto indexID = Hash::Hash64(strIndexID);
			subMesh._indexBuffer = _renderer->CreateBuffer(indexID, indexBufferDesc, nullptr);

			mesh->_subMeshs.emplace_back(std::move(subMesh));
		}

		_meshes.emplace(keyValue, mesh);

		return mesh;
	}

	void ResourceManager::UpdateMesh(MeshBase* mesh, void* vertices, UINT vertexSize, void* indices, UINT indexSize)
	{
		_renderer->WriteBuffer(*mesh->_vertexBuffer, vertices, vertexSize, 0);
		_renderer->WriteBuffer(*mesh->_subMeshs[0]._indexBuffer, indices, indexSize, 0);

		mesh->_subMeshs[0]._drawIndex = indexSize;
	}

	MeshBase* ResourceManager::GetMesh(const DUOLCommon::tstring& objectID)
	{
		auto keyValue = Hash::Hash64(objectID);

		auto foundMesh = _meshes.find(keyValue);

		if (foundMesh != _meshes.end())
		{
			return foundMesh->second.get();
		}

		return nullptr;
	}

	DUOLGraphicsLibrary::Buffer* ResourceManager::CreateEmptyBuffer(const DUOLCommon::tstring& objectID,
		const DUOLGraphicsLibrary::BufferDesc& bufferDesc)
	{
		auto keyValue = Hash::Hash64(objectID);

		//�ֳ� ���� üũ
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
		//�ֳ� ���� üũ
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

		//�ֳ� ���� üũ
		auto foundBuffer = _buffers.find(keyValue);

		if (foundBuffer != _buffers.end())
		{
			return foundBuffer->second;
		}

		return nullptr;
	}

	DUOLGraphicsLibrary::Buffer* ResourceManager::GetBuffer(const UINT64& objectID)
	{
		//�ֳ� ���� üũ
		auto foundBuffer = _buffers.find(objectID);

		if (foundBuffer != _buffers.end())
		{
			return foundBuffer->second;
		}

		return nullptr;
	}

	DUOLGraphicsLibrary::RenderTarget* ResourceManager::CreateRenderTarget(const DUOLGraphicsLibrary::RenderTargetDesc& renderTargetDesc, bool isProportional, float percent)
	{

		auto guid = renderTargetDesc._texture->GetGUID();

		auto foundObject = _renderTargets.find(guid);

		if (foundObject != _renderTargets.end())
		{
			return foundObject->second;
		}

		auto renderTarget = _renderer->CreateRenderTarget(guid, renderTargetDesc);


		_renderTargets.emplace(guid, renderTarget);

		if (isProportional)
		{
			ProportionalRenderTarget info;
			info._renderTarget = renderTarget;
			info._percent = percent;

			_proportionalRenderTarget.emplace(guid, info);
		}

		return renderTarget;
	}

	DUOLGraphicsLibrary::RenderTarget* ResourceManager::GetRenderTarget(const UINT64& objectID)
	{
		auto foundObject = _renderTargets.find(objectID);

		if (foundObject != _renderTargets.end())
		{
			return foundObject->second;
		}

		return nullptr;
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

	DUOLGraphicsLibrary::Shader* ResourceManager::GetShader(const UINT64& objectID)
	{
		auto foundObject = _shaders.find(objectID);

		if (foundObject != _shaders.end())
		{
			return foundObject->second;
		}

		return nullptr;
	}

	DUOLGraphicsLibrary::RenderPass* ResourceManager::CreateRenderPass(const UINT64& objectID, const DUOLGraphicsLibrary::RenderPass& renderPassDesc)
	{
		auto foundObject = _renderPasses.find(objectID);

		if (foundObject != _renderPasses.end())
		{
			return &foundObject->second;
		}

		_renderPasses.emplace(objectID, renderPassDesc);

		return &_renderPasses.find(objectID)->second;
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

	DUOLGraphicsLibrary::PipelineState* ResourceManager::GetPipelineState(const UINT64& objectID)
	{
		auto foundObject = _pipelineStates.find(objectID);

		if (foundObject != _pipelineStates.end())
		{
			return foundObject->second;
		}

		return nullptr;
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
			return foundMaterial->second.get();
		}

		Material* material = new Material;

		material->SetAlbedoMap(GetTexture(materialDesc._albedoMap));
		material->SetNormalMap(GetTexture(materialDesc._normalMap));
		material->SetMetallicSmoothnessAOMap(GetTexture(materialDesc._metallicSmoothnessMap));

		auto foundObj = _pipelineStates.find(Hash::Hash64(materialDesc._pipelineState));
		if (foundObj != _pipelineStates.end())
		{
			material->SetPipelineState(foundObj->second);
		}

		material->SetRenderingPipeline(GetRenderingPipeline(materialDesc._renderPipeline));

		_materials.emplace(Hash::Hash64(objectID), material);

		return material;
	}

	Material* ResourceManager::GetMaterial(const DUOLCommon::tstring& objectID)
	{
		auto foundMaterial = _materials.find(Hash::Hash64(objectID));

		if (foundMaterial != _materials.end())
		{
			return foundMaterial->second.get();
		}

		return nullptr;
	}

	Model* ResourceManager::GetModel(const DUOLCommon::tstring& objectID)
	{
		auto foundModel = _models.find(Hash::Hash64(objectID));

		if (foundModel != _models.end())
		{
			return foundModel->second.get();
		}

		return nullptr;
	}

	AnimationClip* ResourceManager::GetAnimationClip(const DUOLCommon::tstring& objectID)
	{
		auto foundAnimation = _animationClips.find(Hash::Hash64(objectID));

		if (foundAnimation != _animationClips.end())
		{
			return foundAnimation->second.get();
		}

		return nullptr;
	}

	DUOLGraphicsEngine::RenderingPipeline* ResourceManager::CreateRenderingPipeline(const DUOLCommon::tstring& objectID,
		const PipelineType& pipelineType, const DUOLGraphicsLibrary::RenderPass& renderPass,
		const DUOLGraphicsLibrary::ResourceViewLayout& resourceViewLayout)
	{
		auto foundPipeline = _renderingPipelines.find(Hash::Hash64(objectID));

		if (foundPipeline != _renderingPipelines.end())
		{
			return foundPipeline->second.get();
		}

		_renderingPipelines.emplace(Hash::Hash64(objectID), std::move(std::make_unique<RenderingPipeline>(
			this
			, _perFrameBuffer
			, _perObjectBuffer
			, pipelineType
			, renderPass
			, resourceViewLayout)));

		return _renderingPipelines.find(Hash::Hash64(objectID))->second.get();
	}


	DUOLGraphicsEngine::RenderingPipeline* ResourceManager::GetRenderingPipeline(const DUOLCommon::tstring& objectID)
	{
		auto foundPipeline = _renderingPipelines.find(Hash::Hash64(objectID));

		if (foundPipeline != _renderingPipelines.end())
		{
			return foundPipeline->second.get();
		}

		return nullptr;
	}

	DUOLGraphicsEngine::RenderingPipeline* ResourceManager::GetRenderingPipeline(const UINT64& objectID)
	{
		auto foundPipeline = _renderingPipelines.find(objectID);

		if (foundPipeline != _renderingPipelines.end())
		{
			return foundPipeline->second.get();
		}

		return nullptr;
	}
}
