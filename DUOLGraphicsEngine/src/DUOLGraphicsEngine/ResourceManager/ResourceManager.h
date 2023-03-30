#pragma once
#include <memory>
#include <unordered_map>
#include "DUOLCommon/StringHelper.h"

#include "DUOLGraphicsEngine/RenderManager/RenderingPipeline/RenderingPipeline.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/Material.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/Mesh.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/AnimationClip.h"

#include "DUOLGraphicsLibrary/Renderer/PipelineState.h"
#include "DUOLGraphicsLibrary/Renderer/RenderPass.h"

#include "DUOLGraphicsLibrary/BufferFlags.h"
#include "DUOLGraphicsLibrary/PipelineStateFlags.h"
#include "DUOLGraphicsLibrary/RenderTargetFlags.h"
#include "DUOLGraphicsLibrary/ShaderFlags.h"
#include "DUOLGraphicsLibrary/TextureFlags.h"
#include "DUOLGraphicsLibrary/SamplerFlags.h"
#include "DUOLGraphicsLibrary/Renderer/RenderContext.h"

namespace DUOLGraphicsLibrary
{
	class RenderTarget;
	class Shader;
	class Buffer;
	class Renderer;
	class Sampler;
}

namespace DUOLGraphicsEngine
{

	class ResourceManager
	{
	public:
		ResourceManager(DUOLGraphicsLibrary::Renderer* renderer);

		~ResourceManager();

	private:
		DUOLGraphicsLibrary::Renderer* _renderer;

		//sceneID / ResourceBundle of Scene
		//std::unordered_map<DUOLCommon::tstring, std::unique_ptr<ResourceBundle>> _resourceBundles;
		//hash function https://en.wikipedia.org/wiki/Jenkins_hash_function#SpookyHash

		std::unordered_map<UINT64, DUOLGraphicsLibrary::Buffer*> _buffers;

		std::unordered_map<UINT64, DUOLGraphicsLibrary::Shader*> _shaders;

		std::unordered_map<UINT64, DUOLGraphicsLibrary::Sampler*> _samplers;

		std::unordered_map<UINT64, DUOLGraphicsLibrary::Texture*> _textures;

		std::unordered_map<UINT64, std::pair<DUOLGraphicsLibrary::RenderTarget*, bool>> _renderTargets;

		std::unordered_map<UINT64, DUOLGraphicsLibrary::RenderPass> _renderPasses;

		std::unordered_map<UINT64, DUOLGraphicsLibrary::PipelineState*> _pipelineStates;

		struct ProportionalTexture
		{
			float _percent = 1.f; //0보다 커야함

			DUOLGraphicsLibrary::Texture* _texture;
		};
		//screensize에 비례하는 텍스쳐들입니다.
		std::unordered_map<UINT64, ProportionalTexture> _proportionalTexture;

		std::unordered_map<UINT64, std::unique_ptr<Model>> _models; //특정 포맷으로부터 파싱한 메쉬의 묶음

		std::unordered_map<UINT64, std::unique_ptr<MeshBase>> _meshes;

		std::unordered_map<UINT64, std::unique_ptr<Material>> _materials;

		std::unordered_map<UINT64, std::unique_ptr<AnimationClip>> _animationClips;

		std::unordered_map<UINT64, std::unique_ptr<DUOLGraphicsEngine::RenderingPipeline>> _renderingPipelines;

		DUOLGraphicsLibrary::Buffer* _perFrameBuffer;

		DUOLGraphicsLibrary::Buffer* _perCameraBuffer;

		DUOLGraphicsLibrary::Buffer* _perObjectBuffer;

		std::vector<std::pair<uint64, DUOLCommon::tstring>> _materialNameList;

		std::vector<std::pair<uint64, DUOLCommon::tstring>> _animationNameList;
	
	private:
		DUOLGraphicsLibrary::Texture* LoadMaterialTexture(const DUOLCommon::tstring& path, DUOLCommon::tstring& fileID);

		void DeSerializeMaterial(MaterialDesc& material,std::string& name);

		void DeSerializeMesh(Model& model, std::string& name);

		void DeSerializeAnimationClip(AnimationClip& animation, std::string& name);

		void FindMaterialName(std::vector<uint64> useid, std::vector<DUOLCommon::tstring>& id);
		
		void FindAnimaitonName(std::vector<uint64> useid,std::vector<DUOLCommon::tstring>& id);

	public:
		void OnResize(const DUOLMath::Vector2& resolution);

		void ResizeTexture(DUOLGraphicsLibrary::Texture* texture, const DUOLMath::Vector2& resolution);

		void ClearRenderTargets();

		//TODO: inl or json 형식으로 빼야할 것들 
		void AddBackbufferRenderTarget(DUOLGraphicsLibrary::RenderTarget* backbuffer);

		void CreateDebugMaterial();

		DUOLGraphicsLibrary::Buffer* GetPerFrameBuffer() { return _perFrameBuffer; };

		DUOLGraphicsLibrary::Buffer* GetPerCameraBuffer() { return _perCameraBuffer; };

		DUOLGraphicsLibrary::Buffer* GetPerObjectBuffer() { return _perObjectBuffer; };

		DUOLGraphicsLibrary::Texture* CreateTexture(const DUOLCommon::tstring& objectID, const DUOLGraphicsLibrary::TextureDesc& textureDesc, bool isProportional = false, float percent = 1.f);

		DUOLGraphicsLibrary::Texture* CreateTexture(const UINT64& objectID, const DUOLGraphicsLibrary::TextureDesc& textureDesc, bool isProportional = false, float percent = 1.f);

		Model* CreateModelFromFBX(const DUOLCommon::tstring& objectID, std::pair<std::vector<uint64>, std::vector<uint64>>& modeldatas);

		MeshBase* CreateMesh(const DUOLCommon::tstring& objectID, SerializeMesh& meshInfo);

		MeshBase* CreateMesh(const DUOLCommon::tstring& objectID, void* vertices, UINT vertexSize, UINT vertexStructureSize, void* indices, UINT indexSize);

		MeshBase* CreateParticleBuffer(const DUOLCommon::tstring& objectID, int maxParticle);

		void UpdateMesh(MeshBase* mesh, void* vertices, UINT vertexSize, void* indices, UINT indexSize);

		void SetDataName(std::vector<std::pair<uint64, DUOLCommon::tstring>>& materialname, std::vector<std::pair<uint64, DUOLCommon::tstring>>& animationname);

		DUOLGraphicsLibrary::Buffer* CreateEmptyBuffer(const DUOLCommon::tstring& objectID, const DUOLGraphicsLibrary::BufferDesc& bufferDesc);

		DUOLGraphicsLibrary::Buffer* CreateBuffer(const UINT64& objectID, const DUOLGraphicsLibrary::BufferDesc& bufferDesc, void* initialData = nullptr);

		DUOLGraphicsLibrary::RenderTarget* CreateRenderTarget(const DUOLCommon::tstring& objectID, const DUOLGraphicsLibrary::RenderTargetDesc& renderTargetDesc, bool autoClearable = true);

		DUOLGraphicsLibrary::RenderTarget* GetRenderTarget(const UINT64& objectID);

		DUOLGraphicsLibrary::Shader* CreateShader(const UINT64& objectID, const DUOLGraphicsLibrary::ShaderDesc& shaderDesc);

		DUOLGraphicsLibrary::RenderPass* CreateRenderPass(const UINT64& objectID, const DUOLGraphicsLibrary::RenderPass& renderPassDesc);

		DUOLGraphicsLibrary::PipelineState* CreatePipelineState(const UINT64& objectID, const DUOLGraphicsLibrary::PipelineStateDesc& pipelineStateDesc);

		DUOLGraphicsLibrary::PipelineState* CreatePipelineState(const UINT64& objectID, const DUOLGraphicsLibrary::ComputePipelineStateDesc& pipelineStateDesc);

		DUOLGraphicsLibrary::Sampler* CreateSampler(const UINT64& objectID, const DUOLGraphicsLibrary::SamplerDesc& samplerDesc);

		Material* CreateMaterial(const DUOLCommon::tstring& objectID, const MaterialDesc& materialDesc);

		DUOLGraphicsEngine::RenderingPipeline* CreateRenderingPipeline(
			const DUOLCommon::tstring& objectID
			, const PipelineType& pipelineType
			, const DUOLGraphicsLibrary::RenderPass& renderPass
			, const DUOLGraphicsLibrary::ResourceViewLayout& textureResourceViewLayout
			, const DUOLGraphicsLibrary::ResourceViewLayout& samplerResourceViewLayout);

		//Get & Set
		DUOLGraphicsLibrary::Texture* GetTexture(const DUOLCommon::tstring& objectID);

		DUOLGraphicsLibrary::Texture* GetTexture(const UINT64& objectID);

		MeshBase* GetMesh(const DUOLCommon::tstring& objectID);

		DUOLGraphicsLibrary::Buffer* GetBuffer(const DUOLCommon::tstring& objectID);

		DUOLGraphicsLibrary::Buffer* GetBuffer(const UINT64& objectID);

		DUOLGraphicsLibrary::Shader* GetShader(const UINT64& objectID);

		DUOLGraphicsLibrary::Sampler* GetSampler(const UINT64& objectID);

		DUOLGraphicsLibrary::PipelineState* GetPipelineState(const UINT64& objectID);

		Material* GetMaterial(const DUOLCommon::tstring& objectID);

		Model* GetModel(const DUOLCommon::tstring& objectID);

		AnimationClip* GetAnimationClip(const DUOLCommon::tstring& objectID);

		AnimationClip* GetAnimationClip(int index);

		int GetAnimationClipSize();

		DUOLGraphicsEngine::RenderingPipeline* GetRenderingPipeline(const DUOLCommon::tstring& objectID);

		DUOLGraphicsEngine::RenderingPipeline* GetRenderingPipeline(const UINT64& objectID);

		//---------------Delete-------------
		void DeleteTexture(const DUOLCommon::tstring& objectID);

		void DeleteRenderTarget(const DUOLCommon::tstring& objectID);

		void DeleteRenderTarget(const UINT64& objectID);

		void DeleteBuffer(const DUOLCommon::tstring& objectID);


	};
}
