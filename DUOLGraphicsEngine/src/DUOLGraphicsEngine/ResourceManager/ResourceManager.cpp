
#include <fstream>

#include "ResourceManager.h"

#include <boost/archive/binary_iarchive.hpp>

#include "DUOLGraphicsLibrary/Renderer/Renderer.h"
#include "DUOLGraphicsEngine/Util/Hash/Hash.h"

#include "DUOLGraphicsEngine/ResourceManager/Resource/RenderConstantBuffer.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/RenderObject.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/Vertex.h"
#include "DUOLJson\JsonReader.h"

namespace DUOLGraphicsEngine
{
	ResourceManager::ResourceManager(DUOLGraphicsLibrary::Renderer* renderer) :
		_renderer(renderer)
	{

		DUOLGraphicsLibrary::BufferDesc perFrameBufferDesc;
		perFrameBufferDesc._size = sizeof(ConstantBufferPerFrame);
		perFrameBufferDesc._usage = DUOLGraphicsLibrary::ResourceUsage::USAGE_DYNAMIC;
		perFrameBufferDesc._format = DUOLGraphicsLibrary::ResourceFormat::FORMAT_UNKNOWN;
		perFrameBufferDesc._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::CONSTANTBUFFER);
		perFrameBufferDesc._cpuAccessFlags = static_cast<long>(DUOLGraphicsLibrary::CPUAccessFlags::WRITE);

		_perFrameBuffer = CreateEmptyBuffer(_T("perFrameBuffer"), perFrameBufferDesc);

		DUOLGraphicsLibrary::BufferDesc perCameraBufferDesc;
		perCameraBufferDesc._size = sizeof(ConstantBufferPerCamera);
		perCameraBufferDesc._usage = DUOLGraphicsLibrary::ResourceUsage::USAGE_DYNAMIC;
		perCameraBufferDesc._format = DUOLGraphicsLibrary::ResourceFormat::FORMAT_UNKNOWN;
		perCameraBufferDesc._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::CONSTANTBUFFER);
		perCameraBufferDesc._cpuAccessFlags = static_cast<long>(DUOLGraphicsLibrary::CPUAccessFlags::WRITE);

		_perCameraBuffer = CreateEmptyBuffer(_T("perCameraBuffer"), perCameraBufferDesc);


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

	void ResourceManager::DeSerializeMaterial(MaterialDesc& material, std::string& name)
	{
		std::string path = "Asset/BinaryData/Materials/" + name;

		std::ifstream fr(path + ".DUOL", std::ios_base::binary);

		boost::archive::binary_iarchive inArchive(fr);

		inArchive >> material;

		fr.close();
	}

	void ResourceManager::DeSerializeMesh(Model& model, std::string& name)
	{
		std::string path = "Asset/BinaryData/Mesh/" + name;

		std::ifstream fr(path + ".DUOL", std::ios_base::binary);

		boost::archive::binary_iarchive inArchive(fr);

		inArchive >> model;

		fr.close();
	}

	void ResourceManager::DeSerializeAnimationClip(AnimationClip& animation, std::string& name)
	{
		std::string path = "Asset/BinaryData/Animation/" + name;

		std::ifstream fr(path + ".DUOL", std::ios_base::binary);

		boost::archive::binary_iarchive inArchive(fr);

		inArchive >> animation;

		fr.close();
	}

	void ResourceManager::FindMaterialName(std::vector<uint64> useid, std::vector<DUOLCommon::tstring>& id)
	{
		// 사용하는 id를 돌면서 name을 받아온다.
		for (auto& materialID : useid)
		{
			for (auto& nameList : _materialNameList)
			{
				if (nameList.first == materialID)
				{
					id.emplace_back(nameList.second);
					break;;
				}

			}
		}
	}

	void ResourceManager::FindAnimaitonName(std::vector<uint64> useid, std::vector<DUOLCommon::tstring>& id)
	{
		// 사용하는 id를 돌면서 name을 받아온다.
		for (auto& animationID : useid)
		{
			for (auto& nameList : _animationNameList)
			{
				if (nameList.first == animationID)
					id.emplace_back(nameList.second);
			}
		}
	}

	void ResourceManager::OnResize(const DUOLMath::Vector2& resolution)
	{
		for (auto& texture : _proportionalTexture)
		{
			auto percentagerResolution = resolution;

			percentagerResolution.x *= texture.second._percent;
			percentagerResolution.y *= texture.second._percent;

			_renderer->SetResolution(texture.second._texture, percentagerResolution);
		}
	}

	void ResourceManager::ResizeTexture(DUOLGraphicsLibrary::Texture* texture, const DUOLMath::Vector2& resolution)
	{
		_renderer->SetResolution(texture, resolution);
	}

	void ResourceManager::ClearRenderTargets()
	{
		for (auto& renderTarget : _renderTargets)
		{
			if(renderTarget.second.second)
				_renderer->ClearRenderTarget(renderTarget.second.first);
		}
	}

	void ResourceManager::AddBackbufferRenderTarget(DUOLGraphicsLibrary::RenderTarget* backbuffer)
	{
		UINT64 backbufferID = Hash::Hash64(_T("BackBuffer"));

		_renderTargets.emplace(backbufferID, std::make_pair(backbuffer, true));
	}

	void ResourceManager::CreateDebugMaterial()
	{
		MaterialDesc debugMat;

		debugMat._pipelineState = TEXT("DebugPointDepthOn");
		CreateMaterial(_T("DebugPointDepthOn"), debugMat);

		debugMat._pipelineState = TEXT("DebugPointDepthOff");
		CreateMaterial(_T("DebugPointDepthOff"), debugMat);

		debugMat._pipelineState = TEXT("DebugLineDepthOn");
		CreateMaterial(_T("DebugLineDepthOn"), debugMat);

		debugMat._pipelineState = TEXT("DebugLineDepthOff");
		CreateMaterial(_T("DebugLineDepthOff"), debugMat);

		debugMat._pipelineState = TEXT("DebugTriangleDepthOn");
		CreateMaterial(_T("DebugTriangleDepthOn"), debugMat);

		debugMat._pipelineState = TEXT("DebugTriangleDepthOff");
		CreateMaterial(_T("DebugTriangleDepthOff"), debugMat);
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

	void ResourceManager::SetDataName(std::vector<std::pair<uint64, DUOLCommon::tstring>>& materialname, std::vector<std::pair<uint64, DUOLCommon::tstring>>& animationname)
	{
		_materialNameList = materialname;
		_animationNameList = animationname;
	}

	DUOLGraphicsLibrary::Texture* ResourceManager::CreateTexture(const DUOLCommon::tstring& objectID,
		const DUOLGraphicsLibrary::TextureDesc& textureDesc, bool isProportional, float percent)
	{
		auto keyValue = Hash::Hash64(objectID);

		auto foundTexture = _textures.find(keyValue);

		if (foundTexture != _textures.end())
		{
			return foundTexture->second;
		}

		auto texture = _renderer->CreateTexture(keyValue, textureDesc);

		_textures.emplace(keyValue, texture);

		if (isProportional)
		{
			ProportionalTexture info;
			info._texture = texture;
			info._percent = percent;

			_proportionalTexture.emplace(keyValue, info);
		}
		return texture;
	}

	DUOLGraphicsLibrary::Texture* ResourceManager::CreateTexture(const UINT64& objectID,
		const DUOLGraphicsLibrary::TextureDesc& textureDesc, bool isProportional, float percent)
	{
		auto foundTexture = _textures.find(objectID);

		if (foundTexture != _textures.end())
		{
			return foundTexture->second;
		}

		auto texture = _renderer->CreateTexture(objectID, textureDesc);

		_textures.emplace(objectID, texture);

		if (isProportional)
		{
			ProportionalTexture info;
			info._texture = texture;
			info._percent = percent;

			_proportionalTexture.emplace(objectID, info);
		}

		return texture;
	}


	Model* ResourceManager::CreateModelFromFBX(const DUOLCommon::tstring& objectID, std::pair<std::vector<uint64>, std::vector<uint64>>& modeldatas)
	{
		auto keyValue = Hash::Hash64(objectID);
		auto foundModel = _models.find(keyValue);

		if (foundModel != _models.end())
		{
			return foundModel->second.get();
		}

#pragma region Serialize_Mesh
		//tstring to string cast
		//std::string strPath = DUOLCommon::StringHelper::ToString(path);
		std::string modelName = DUOLCommon::StringHelper::ToString(objectID);

		Model* model = new Model;

		DeSerializeMesh((*model), modelName);

		int meshSize = static_cast<int>(model->GetSerializeMesh().size());

		model->SetMeshCount(meshSize);

		std::vector<SerializeMesh> meshdatas = model->GetSerializeMesh();

		for (int meshIndex = 0; meshIndex < meshSize; meshIndex++)
		{
			auto& meshInfo = meshdatas[meshIndex];
			auto meshName = DUOLCommon::StringHelper::ToTString(meshInfo.nodeName);

			auto mesh = GetMesh(meshName);

			if (mesh == nullptr)
			{
				mesh = CreateMesh(meshName, meshInfo);
			}

			// 모델에 참조할 수 있는 메쉬 포인터를 넣습니다.
			model->AddMesh(mesh);

			// 추가로 거대한 .fbx 모델에서 각각의 스태틱 메쉬들을 뽑아올 수 있도록 이름과 매핑해둡니다.
			model->AddMeshWithName(meshName, mesh);
		}

		_models.emplace(keyValue, model);

		//bone
		{
			int boneSize = static_cast<int>(model->GetBones().size());

			std::vector<Bone> serializebone = model->GetBones();

			if (boneSize > 0)
			{
				auto& bones = model->GetBones();
				bones.resize(boneSize);

				for (int boneIndex = 0; boneIndex < boneSize; boneIndex++)
				{
					bones[boneIndex]._boneName = serializebone[boneIndex]._boneName;
					bones[boneIndex]._parentIndex = serializebone[boneIndex]._parentIndex;
					bones[boneIndex]._nodeMatrix = serializebone[boneIndex]._nodeMatrix;
					bones[boneIndex]._offsetMatrix = serializebone[boneIndex]._offsetMatrix;
				}

				model->SetIsSkinningModel(true);
			}
		}
#pragma endregion 

#pragma region Serialize_Material

		std::vector<DUOLCommon::tstring> materialId;

		FindMaterialName(modeldatas.first, materialId);

		for (int materialIndex = 0; materialIndex < modeldatas.first.size(); materialIndex++)
		{
			MaterialDesc materialDesc;

			std::string path = DUOLCommon::StringHelper::ToString(materialId[materialIndex]);

			// 여기서 받아온다.
			DeSerializeMaterial(materialDesc, path);

			DUOLCommon::tstring materialName(materialDesc._materialName.begin(), materialDesc._materialName.end());

			const DUOLCommon::tstring defaultPath = _T("Asset/Texture/");

			if (materialDesc._isAlbedo)
			{
				LoadMaterialTexture(defaultPath + materialDesc._albedoMap, materialDesc._albedoMap);
			}
			if (materialDesc._isNormal)
			{
				LoadMaterialTexture(defaultPath + materialDesc._normalMap, materialDesc._normalMap);
			}
			if (materialDesc._isMetallic)
			{
				LoadMaterialTexture(defaultPath + materialDesc._metallicRoughnessMap, materialDesc._metallicRoughnessMap);
			}

			if (model->IsSkinningModel())
			{
				if (materialDesc._isAlbedo && materialDesc._isNormal && (materialDesc._isMetallic || materialDesc._isRoughness))
				{
					materialDesc._pipelineState = _T("SkinnedAlbedoNormalMRA");
				}
				else if (materialDesc._isAlbedo && materialDesc._isNormal)
				{
					materialDesc._pipelineState = _T("SkinnedAlbedoNormal");
				}
				else if (materialDesc._isAlbedo)
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
				if (materialDesc._isAlbedo && materialDesc._isNormal && (materialDesc._isMetallic || materialDesc._isRoughness))
				{
					materialDesc._pipelineState = _T("AlbedoNormalMRA");
				}
				else if (materialDesc._isAlbedo && materialDesc._isNormal)
				{
					materialDesc._pipelineState = _T("AlbedoNormal");
				}
				else if (materialDesc._isAlbedo)
				{
					materialDesc._pipelineState = _T("Albedo");
				}
				else
				{
					materialDesc._pipelineState = _T("Default");
				}
			}

			CreateMaterial(materialName, materialDesc);
		}

#pragma endregion 

		if (model->IsSkinningModel())
		{
			std::vector<DUOLCommon::tstring> animationId;

			FindAnimaitonName(modeldatas.second, animationId);

			//int animationClipSize = model->animationClipList.size();

			for (int animationClipIndex = 0; animationClipIndex < modeldatas.second.size(); animationClipIndex++)
			{
				AnimationClip* animationClip = new AnimationClip;

				std::string path = DUOLCommon::StringHelper::ToString(animationId[animationClipIndex]);

				DeSerializeAnimationClip((*animationClip), path);

				animationClip->_totalKeyFrame = animationClip->_totalKeyFrame;
				animationClip->_frameRate = animationClip->_frameRate;
				animationClip->_startKeyFrame = animationClip->_startKeyFrame;
				animationClip->_endKeyFrame = animationClip->_endKeyFrame;
				animationClip->_tickPerFrame = animationClip->_tickPerFrame;

				int animationFrameSize = static_cast<int>(animationClip->_keyFrameList.size());
				animationClip->_keyFrameList.reserve(animationFrameSize);
				animationClip->_keyFrameList.resize(animationFrameSize);


				for (int animationKeyFrameIndex = 0; animationKeyFrameIndex < animationFrameSize; animationKeyFrameIndex++)
				{
					auto& animationFrameBonesInfo = animationClip->_keyFrameList[animationKeyFrameIndex];
					int animationFrameBoneSize = static_cast<int>(animationFrameBonesInfo.size());

					animationClip->_keyFrameList[animationKeyFrameIndex].reserve(animationFrameBoneSize);
					animationClip->_keyFrameList[animationKeyFrameIndex].resize(animationFrameBoneSize);

					for (int animationFrameBoneIndex = 0; animationFrameBoneIndex < animationFrameBoneSize; animationFrameBoneIndex++)
					{
						auto& animationFrameBoneInfoOrigin = animationFrameBonesInfo[animationFrameBoneIndex];
						auto& animationFrameBoneInfo = animationClip->_keyFrameList[animationKeyFrameIndex][animationFrameBoneIndex];
						animationFrameBoneInfo._time = animationFrameBoneInfoOrigin._time;
						animationFrameBoneInfo._localScale = animationFrameBoneInfoOrigin._localScale;
						animationFrameBoneInfo._localRotation = animationFrameBoneInfoOrigin._localRotation;
						animationFrameBoneInfo._localTransform = animationFrameBoneInfoOrigin._localTransform;
					}
				}

				DUOLCommon::tstring animName = DUOLCommon::tstring(animationClip->_animationName.begin(), animationClip->_animationName.end());

				_animationClips.emplace(Hash::Hash64(animName), animationClip);
			}
		}
		return model;
	}

	MeshBase* ResourceManager::CreateMesh(const DUOLCommon::tstring& objectID, SerializeMesh& meshInfo)
	{
		MeshBase* retMesh = nullptr;

		if (meshInfo.isSkinned)
		{
			SkinnedMesh* mesh = new SkinnedMesh;
			retMesh = mesh;

			/*mesh->_subMeshCount = 1;
			mesh->_subMeshs.reserve(1);*/

			mesh->_subMeshCount = static_cast<unsigned int>(meshInfo.indices.size());
			mesh->_subMeshs.reserve(mesh->_subMeshCount);
			mesh->_halfExtents = meshInfo.halfExtent;

			DUOLCommon::tstring strVertexID = objectID + (_T("Vertex"));

			int verticeSize = static_cast<int>(meshInfo.vertexList.size());

			DUOLGraphicsLibrary::BufferDesc vetexBufferDesc;

			vetexBufferDesc._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::VERTEXBUFFER);
			vetexBufferDesc._usage = DUOLGraphicsLibrary::ResourceUsage::USAGE_DEFAULT;
			vetexBufferDesc._stride = sizeof(DUOLGraphicsEngine::Vertex);
			vetexBufferDesc._size = vetexBufferDesc._stride * verticeSize;

			mesh->_vertices.resize(verticeSize);

			for (int vertexIndex = 0; vertexIndex < verticeSize; vertexIndex++)
			{
				memcpy(&mesh->_vertices[vertexIndex], &meshInfo.vertexList[vertexIndex], sizeof(DUOLGraphicsEngine::SKinnedMeshVertex));
			}

			mesh->_center = meshInfo.center;

			auto vertexId = Hash::Hash64(strVertexID);
			mesh->_vertexBuffer = _renderer->CreateBuffer(vertexId, vetexBufferDesc, mesh->_vertices.data());

			for (int subMeshIndex = 0; subMeshIndex < mesh->_subMeshCount ; subMeshIndex++)
			{
				DUOLCommon::tstring strIndexID = objectID + (_T("Index")) + std::to_wstring(subMeshIndex);

				SubMesh subMesh;

				subMesh._submeshIndex = subMeshIndex;

				int indexSize = meshInfo.indices[subMeshIndex].size();

				subMesh._materialName = DUOLCommon::StringHelper::ToTString(meshInfo.materialName[subMeshIndex]);

				DUOLGraphicsLibrary::BufferDesc indexBufferDesc;

				indexBufferDesc._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::INDEXBUFFER);
				indexBufferDesc._usage = DUOLGraphicsLibrary::ResourceUsage::USAGE_DEFAULT;
				indexBufferDesc._stride = sizeof(unsigned int);
				indexBufferDesc._size = indexBufferDesc._stride * indexSize;
				indexBufferDesc._format = DUOLGraphicsLibrary::ResourceFormat::FORMAT_R32_UINT;

				auto indexID = Hash::Hash64(strIndexID);
				subMesh._indexBuffer = _renderer->CreateBuffer(indexID, indexBufferDesc, meshInfo.indices[subMeshIndex].data());

				subMesh._indices.reserve(indexSize);
				for (auto& index : meshInfo.indices[subMeshIndex])
				{
					subMesh._indices.emplace_back(index);
				}

				subMesh._drawIndex = indexSize;

				mesh->_subMeshs.emplace_back(std::move(subMesh));
			}
		}
		else
		{
			Mesh* mesh = new Mesh;
			retMesh = mesh;

			DUOLCommon::tstring strVertexID = objectID + (_T("Vertex"));

			int verticeSize = static_cast<int>(meshInfo.vertexList.size());

			DUOLGraphicsLibrary::BufferDesc vetexBufferDesc;

			vetexBufferDesc._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::VERTEXBUFFER);
			vetexBufferDesc._usage = DUOLGraphicsLibrary::ResourceUsage::USAGE_DEFAULT;
			vetexBufferDesc._stride = sizeof(DUOLGraphicsEngine::StaticMeshVertex); //position 3 uv 2 normal 3 tangent 3 binormal 3
			vetexBufferDesc._size = vetexBufferDesc._stride * verticeSize;
			mesh->_halfExtents = meshInfo.halfExtent;

			mesh->_vertices.resize(verticeSize);

			for (int vertexIndex = 0; vertexIndex < verticeSize; vertexIndex++)
			{
				memcpy(&mesh->_vertices[vertexIndex], &meshInfo.vertexList[vertexIndex], sizeof(DUOLGraphicsEngine::StaticMeshVertex));
			}

			mesh->_center = meshInfo.center;

			auto vertexId = Hash::Hash64(strVertexID);
			mesh->_vertexBuffer = _renderer->CreateBuffer(vertexId, vetexBufferDesc, mesh->_vertices.data());

			mesh->_subMeshCount = static_cast<unsigned int>(meshInfo.indices.size());
			mesh->_subMeshs.reserve(mesh->_subMeshCount);

			for (int subMeshIndex = 0; subMeshIndex < mesh->_subMeshCount; subMeshIndex++)
			{
				{
					DUOLCommon::tstring strIndexID = objectID + (_T("Index")) + std::to_wstring(subMeshIndex);

					SubMesh subMesh;

					subMesh._materialName = DUOLCommon::StringHelper::ToTString(meshInfo.materialName[subMeshIndex]);

					subMesh._submeshIndex = subMeshIndex;

					int indexSize = static_cast<int>(meshInfo.indices[subMeshIndex].size());

					DUOLGraphicsLibrary::BufferDesc indexBufferDesc;

					indexBufferDesc._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::INDEXBUFFER);
					indexBufferDesc._usage = DUOLGraphicsLibrary::ResourceUsage::USAGE_DEFAULT;
					indexBufferDesc._stride = sizeof(unsigned int);
					indexBufferDesc._size = indexBufferDesc._stride * indexSize;
					indexBufferDesc._format = DUOLGraphicsLibrary::ResourceFormat::FORMAT_R32_UINT;

					auto indexID = Hash::Hash64(strIndexID);
					subMesh._indexBuffer = _renderer->CreateBuffer(indexID, indexBufferDesc, meshInfo.indices[subMeshIndex].data());

					subMesh._indices.reserve(indexSize);
					for (auto& index : meshInfo.indices[subMeshIndex])
					{
						subMesh._indices.emplace_back(index);
					}

					subMesh._drawIndex = indexSize;

					mesh->_subMeshs.emplace_back(std::move(subMesh));
				}
			}
		}

		retMesh->_meshName = DUOLCommon::StringHelper::ToTString(meshInfo.nodeName);

		_meshes.emplace(Hash::Hash64(objectID), retMesh);

		return retMesh;
	}

	MeshBase* ResourceManager::CreateMesh(const DUOLCommon::tstring& objectID, void* vertices, UINT vertexSize, UINT vertexStructureSize, void* indices, UINT indexSize)
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

		DUOLCommon::tstring strVertexID = objectID + (_T("Vertex"));
		auto vertexId = Hash::Hash64(strVertexID);

		DUOLGraphicsLibrary::BufferDesc vetexBufferDesc;

		vetexBufferDesc._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::VERTEXBUFFER);
		vetexBufferDesc._usage = DUOLGraphicsLibrary::ResourceUsage::USAGE_DEFAULT;
		vetexBufferDesc._stride = sizeof(int);
		vetexBufferDesc._size = sizeof(int) * maxParticle;
		vetexBufferDesc._cpuAccessFlags = 0;

		mesh->_vertexBuffer = _renderer->CreateBuffer(vertexId, vetexBufferDesc, nullptr);

		DUOLGraphicsLibrary::BufferDesc particleBufferDesc;

		particleBufferDesc._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::UNORDEREDACCESS) | static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE);
		particleBufferDesc._usage = DUOLGraphicsLibrary::ResourceUsage::USAGE_DEFAULT;
		particleBufferDesc._stride = sizeof(Particle);
		particleBufferDesc._size = sizeof(Particle) * maxParticle;
		particleBufferDesc._format = DUOLGraphicsLibrary::ResourceFormat::FORMAT_UNKNOWN;
		particleBufferDesc._cpuAccessFlags = 0;
		particleBufferDesc._miscFlags = static_cast<long>(DUOLGraphicsLibrary::MiscFlags::RESOURCE_MISC_BUFFER_STRUCTURED);

		DUOLCommon::tstring strParticleID = objectID + (_T("Particle"));
		mesh->_particleBuffer = _renderer->CreateBuffer(Hash::Hash64(strParticleID), particleBufferDesc, nullptr);

		DUOLGraphicsLibrary::BufferDesc counterBuffer;

		DUOLCommon::tstring strCounterID = objectID + (_T("Counter"));
		counterBuffer._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::UNORDEREDACCESS);
		counterBuffer._usage = DUOLGraphicsLibrary::ResourceUsage::USAGE_DEFAULT;
		counterBuffer._stride = 16;
		counterBuffer._size = 16;
		counterBuffer._format = DUOLGraphicsLibrary::ResourceFormat::FORMAT_UNKNOWN;
		counterBuffer._cpuAccessFlags = 0;
		counterBuffer._miscFlags = static_cast<long>(DUOLGraphicsLibrary::MiscFlags::RESOURCE_MISC_BUFFER_STRUCTURED);

		mesh->_counterBuffer = _renderer->CreateBuffer(Hash::Hash64(strCounterID), counterBuffer, nullptr);


		_meshes.emplace(keyValue, mesh);

		return mesh;
	}

	void ResourceManager::UpdateMesh(MeshBase* mesh, void* vertices, UINT vertexSize, void* indices, UINT indexSize)
	{
		_renderer->WriteBuffer(mesh->_vertexBuffer, vertices, vertexSize, 0);
		_renderer->WriteBuffer(mesh->_subMeshs[0]._indexBuffer, indices, indexSize, 0);

		mesh->_subMeshs[0]._drawIndex = indexSize / sizeof(UINT);
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

	DUOLGraphicsLibrary::Buffer* ResourceManager::CreateBuffer(const UINT64& objectID,
	                                                                const DUOLGraphicsLibrary::BufferDesc& bufferDesc, void* initialData)
	{
		//�ֳ� ���� üũ
		auto foundBuffer = _buffers.find(objectID);

		if (foundBuffer != _buffers.end())
		{
			return foundBuffer->second;
		}

		auto buffer = _renderer->CreateBuffer(objectID, bufferDesc, initialData);

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


	DUOLGraphicsLibrary::RenderTarget* ResourceManager::CreateRenderTarget(const DUOLCommon::tstring& objectID, const DUOLGraphicsLibrary::RenderTargetDesc& renderTargetDesc, bool autoClearable)
	{
		auto guid = Hash::Hash64(objectID);

		auto foundObject = _renderTargets.find(guid);

		if (foundObject != _renderTargets.end())
		{
			return foundObject->second.first;
		}

		auto renderTarget = _renderer->CreateRenderTarget(guid, renderTargetDesc);

		_renderTargets.emplace(guid, std::make_pair(renderTarget, autoClearable));

		return renderTarget;
	}

	void ResourceManager::DeleteRenderTarget(const DUOLCommon::tstring& objectID)
	{
		auto guid = Hash::Hash64(objectID);

		auto foundObject = _renderTargets.find(guid);

		if (foundObject != _renderTargets.end())
		{
			_renderer->Release(foundObject->second.first);
			auto ret = _renderTargets.erase(guid);
		}

		return;
	}

	void ResourceManager::DeleteRenderTarget(const UINT64& objectID)
	{
		auto foundObject = _renderTargets.find(objectID);

		if (foundObject != _renderTargets.end())
		{
			_renderer->Release(foundObject->second.first);
			auto ret = _renderTargets.erase(objectID);
		}

		return;
	}

	void ResourceManager::DeleteBuffer(const DUOLCommon::tstring& objectID)
	{
		auto guid = Hash::Hash64(objectID);

		auto foundObject = _buffers.find(guid);

		if (foundObject != _buffers.end())
		{
			_renderer->Release(foundObject->second);
			auto ret = _buffers.erase(guid);
		}
	}

	DUOLGraphicsLibrary::RenderTarget* ResourceManager::GetRenderTarget(const UINT64& objectID)
	{
		auto foundObject = _renderTargets.find(objectID);

		if (foundObject != _renderTargets.end())
		{
			return foundObject->second.first;
		}

		return nullptr;
	}

	DUOLGraphicsLibrary::Shader* ResourceManager::CreateShader(const UINT64& objectID,
		const DUOLGraphicsLibrary::ShaderDesc& shaderDesc)
	{
		static std::mutex shaderMutex1;
		static std::mutex shaderMutex2;

		shaderMutex1.lock();
		auto foundObject = _shaders.find(objectID);
		shaderMutex1.unlock();

		if (foundObject != _shaders.end())
		{
			return foundObject->second;
		}

		auto shader = _renderer->CreateShader(objectID, shaderDesc);
		shaderMutex2.lock();
		_shaders.emplace(objectID, shader);
		shaderMutex2.unlock();

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

	DUOLGraphicsLibrary::Sampler* ResourceManager::GetSampler(const UINT64& objectID)
	{
		auto foundObject = _samplers.find(objectID);

		if (foundObject != _samplers.end())
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

	DUOLGraphicsLibrary::PipelineState* ResourceManager::CreatePipelineState(const UINT64& objectID,
		const DUOLGraphicsLibrary::ComputePipelineStateDesc& pipelineStateDesc)
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


	Material* ResourceManager::CreateMaterial(const DUOLCommon::tstring& objectID, const MaterialDesc& materialDesc)
	{
		auto foundMaterial = _materials.find(Hash::Hash64(objectID));

		if (foundMaterial != _materials.end())
		{
			return foundMaterial->second.get();
		}

		std::string path = "Asset/Texture/";

		Material* material = new Material;

		//Albedo
		if (!materialDesc._albedoMap.empty())
		{
			auto albedo = GetTexture(materialDesc._albedoMap);
			if (albedo == nullptr)
			{
				DUOLGraphicsLibrary::TextureDesc albeldoDesc;
				path += DUOLCommon::StringHelper::ToString(materialDesc._albedoMap);
				albeldoDesc._texturePath = path.c_str();

				albedo = CreateTexture(materialDesc._albedoMap, albeldoDesc);
			}

			material->SetAlbedoMap(albedo);
		}

		if (!materialDesc._normalMap.empty())
		{
			auto normal = GetTexture(materialDesc._normalMap);
			if (normal == nullptr)
			{
				DUOLGraphicsLibrary::TextureDesc normalDesc;
				path += DUOLCommon::StringHelper::ToString(materialDesc._normalMap);
				normalDesc._texturePath = path.c_str();

				normal = CreateTexture(materialDesc._normalMap, normalDesc);
			}

			material->SetNormalMap(normal);
		}

		if (!materialDesc._metallicRoughnessMap.empty())
		{
			auto MRAmap = GetTexture(materialDesc._metallicRoughnessMap);
			if (MRAmap == nullptr)
			{
				DUOLGraphicsLibrary::TextureDesc MRAdesc;
				path += DUOLCommon::StringHelper::ToString(materialDesc._metallicRoughnessMap);
				MRAdesc._texturePath = path.c_str();

				MRAmap = CreateTexture(materialDesc._metallicRoughnessMap, MRAdesc);
			}

			material->SetMetallicSmoothnessAOMap(MRAmap);
		}

		material->SetAlbedo(materialDesc._albedo);
		material->SetMetallic(materialDesc._metallic);
		material->SetRoughness(materialDesc._roughness);
		material->SetSpecular(materialDesc._specular);
		material->SetEmissive(materialDesc._emissive);

		auto foundObj = _pipelineStates.find(Hash::Hash64(materialDesc._pipelineState));
		if (foundObj != _pipelineStates.end())
		{
			material->SetPipelineState(foundObj->second);
		}

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

	AnimationClip* ResourceManager::GetAnimationClip(int index)
	{
		auto iter = _animationClips.begin();

		int count = 0;

		for(iter; iter !=_animationClips.end(); ++iter)
		{
			if(count ==index)
			{
				return iter->second.get();
			}
			count++;
		}

		return nullptr;
	}

	int ResourceManager::GetAnimationClipSize()
	{
		return _animationClips.size();
	}

	DUOLGraphicsEngine::RenderingPipeline* ResourceManager::CreateRenderingPipeline(const DUOLCommon::tstring& objectID,
		const PipelineType& pipelineType, const DUOLGraphicsLibrary::RenderPass& renderPass
		, const DUOLGraphicsLibrary::ResourceViewLayout& textureResourceViewLayout
		, const DUOLGraphicsLibrary::ResourceViewLayout& samplerResourceViewLayout)
	{
		auto foundPipeline = _renderingPipelines.find(Hash::Hash64(objectID));

		if (foundPipeline != _renderingPipelines.end())
		{
			return foundPipeline->second.get();
		}

		auto pipeline = std::make_unique<RenderingPipeline>(
			this
			, pipelineType
			, renderPass
			, textureResourceViewLayout
			, samplerResourceViewLayout);

#if defined(_DEBUG) || defined(DEBUG)
		pipeline->SetName(objectID.c_str());
#endif

		_renderingPipelines.emplace(Hash::Hash64(objectID), std::move(pipeline));

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

	void ResourceManager::DeleteTexture(const DUOLCommon::tstring& objectID)
	{
		auto guid = Hash::Hash64(objectID);

		auto foundObject = _textures.find(guid);

		if (foundObject != _textures.end())
		{
			_renderer->Release(foundObject->second);
			auto ret = _textures.erase(guid);

			DeleteRenderTarget(objectID);
		}

		return;
	}
}
