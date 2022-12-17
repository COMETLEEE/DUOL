#pragma once
#include <memory>
#include <unordered_map>
#include "DUOLCommon/StringHelper.h"

#include "DUOLGraphicsEngine/RenderManager/RenderingPipeline.h"
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

namespace DuolData
{
	class Mesh;	
}

namespace DUOLParser
{
	class DUOLParserBase;
}

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

		std::shared_ptr<DUOLParser::DUOLParserBase> _parser;

		//sceneID / ResourceBundle of Scene
		//std::unordered_map<DUOLCommon::tstring, std::unique_ptr<ResourceBundle>> _resourceBundles;
		//hash function https://en.wikipedia.org/wiki/Jenkins_hash_function#SpookyHash

		std::unordered_map<UINT64, DUOLGraphicsLibrary::Buffer*> _buffers;

		std::unordered_map<UINT64, DUOLGraphicsLibrary::Shader*> _shaders;

		std::unordered_map<UINT64, DUOLGraphicsLibrary::Sampler*> _samplers;

		std::unordered_map<UINT64, DUOLGraphicsLibrary::Texture*> _textures;

		std::unordered_map<UINT64, DUOLGraphicsLibrary::RenderTarget*> _renderTargets;

		std::unordered_map<UINT64, DUOLGraphicsLibrary::RenderPass> _renderPasses;

		std::unordered_map<UINT64, DUOLGraphicsLibrary::PipelineState*> _pipelineStates;

		struct ProportionalRenderTarget
		{
			float _percent; //0보다 커야함

			DUOLGraphicsLibrary::RenderTarget* _renderTarget;
		};

		std::unordered_map<UINT64, ProportionalRenderTarget> _proportionalRenderTarget;

		std::unordered_map<UINT64, std::unique_ptr<Model>> _models; //특정 포맷으로부터 파싱한 메쉬의 묶음

		std::unordered_map<UINT64, std::unique_ptr<MeshBase>> _meshes;

		std::unordered_map<UINT64, std::unique_ptr<Material>> _materials;

		std::unordered_map<UINT64, std::unique_ptr<AnimationClip>> _animationClips;

		std::unordered_map<UINT64, std::unique_ptr<DUOLGraphicsEngine::RenderingPipeline>> _renderingPipelines;

		DUOLGraphicsLibrary::Buffer* _perFrameBuffer;

		DUOLGraphicsLibrary::Buffer* _perObjectBuffer;

	private:
		DUOLGraphicsLibrary::Texture* LoadMaterialTexture(const DUOLCommon::tstring& path, DUOLCommon::tstring& fileID);

	public:
		void OnResize(const DUOLMath::Vector2& resolution);

		void ClearRenderTargets();

		void AddBackbufferRenderTarget(DUOLGraphicsLibrary::RenderTarget* backbuffer);

		void CreateDebugMaterial(DUOLGraphicsLibrary::RenderTarget* backbuffer);

		DUOLGraphicsLibrary::Buffer* GetPerFrameBuffer() { return _perFrameBuffer; };

		DUOLGraphicsLibrary::Buffer* GetPerFrameObjectBuffer() { return _perObjectBuffer; };
	public:
		DUOLGraphicsLibrary::Texture* CreateTexture(const DUOLCommon::tstring& objectID, const DUOLGraphicsLibrary::TextureDesc& textureDesc);

		DUOLGraphicsLibrary::Texture* CreateTexture(const UINT64& objectID, const DUOLGraphicsLibrary::TextureDesc& textureDesc);

		Model* CreateModelFromFBX(const DUOLCommon::tstring& objectID, const DUOLCommon::tstring& path);

		MeshBase* CreateMesh(const DUOLCommon::tstring& objectID, std::shared_ptr<DuolData::Mesh>& meshInfo);\

		MeshBase* CreateMesh(const DUOLCommon::tstring& objectID, void* vertices, UINT vertexSize, UINT vertexStructureSize, void* indices, UINT indexSize);

		void UpdateMesh(MeshBase* mesh, void* vertices, UINT vertexSize, void* indices, UINT indexSize);

		DUOLGraphicsLibrary::Buffer* CreateEmptyBuffer(const DUOLCommon::tstring& objectID, const DUOLGraphicsLibrary::BufferDesc& bufferDesc);

		DUOLGraphicsLibrary::Buffer* CreateEmptyBuffer(const UINT64& objectID, const DUOLGraphicsLibrary::BufferDesc& bufferDesc);

		DUOLGraphicsLibrary::RenderTarget* CreateRenderTarget(const DUOLGraphicsLibrary::RenderTargetDesc& renderTargetDesc, bool isProportional = false, float percent = 1.f);

		DUOLGraphicsLibrary::RenderTarget* GetRenderTarget(const UINT64& objectID);

		DUOLGraphicsLibrary::Shader* CreateShader(const UINT64& objectID, const DUOLGraphicsLibrary::ShaderDesc& shaderDesc);

		DUOLGraphicsLibrary::RenderPass* CreateRenderPass(const UINT64& objectID, const DUOLGraphicsLibrary::RenderPass& renderPassDesc);

		DUOLGraphicsLibrary::PipelineState* CreatePipelineState(const UINT64& objectID, const DUOLGraphicsLibrary::PipelineStateDesc& pipelineStateDesc);

		DUOLGraphicsLibrary::Sampler* CreateSampler(const UINT64& objectID, const DUOLGraphicsLibrary::SamplerDesc& samplerDesc);

		Material* RegistMaterial(const DUOLCommon::tstring& objectID, const MaterialDesc& materialDesc);

		DUOLGraphicsEngine::RenderingPipeline* CreateRenderingPipeline(
			const DUOLCommon::tstring& objectID
			, const PipelineType& pipelineType
			, const DUOLGraphicsLibrary::RenderPass& renderPass
			, const DUOLGraphicsLibrary::ResourceViewLayout& resourceViewLayout);
		//Ge
		//tSet
		DUOLGraphicsLibrary::Texture* GetTexture(const DUOLCommon::tstring& objectID);

		DUOLGraphicsLibrary::Texture* GetTexture(const UINT64& objectID);

		MeshBase* GetMesh(const DUOLCommon::tstring& objectID);

		DUOLGraphicsLibrary::Buffer* GetBuffer(const DUOLCommon::tstring& objectID);

		DUOLGraphicsLibrary::Buffer* GetBuffer(const UINT64& objectID);

		DUOLGraphicsLibrary::Shader* GetShader(const UINT64& objectID);

		DUOLGraphicsLibrary::PipelineState* GetPipelineState(const UINT64& objectID);

		Material* GetMaterial(const DUOLCommon::tstring& objectID);

		DUOLGraphicsEngine::RenderingPipeline* GetRenderingPipeline(const DUOLCommon::tstring& objectID);

		DUOLGraphicsEngine::RenderingPipeline* GetRenderingPipeline(const UINT64& objectID);

	};
}
