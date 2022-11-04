#pragma once
#include <memory>
#include <unordered_map>
#include "DUOLCommon/StringHelper.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/Material.h"
#include "DUOLGraphicsLibrary/BufferFlags.h"
#include "DUOLGraphicsLibrary/PipelineStateFlags.h"
#include "DUOLGraphicsLibrary/RenderPassFlags.h"
#include "DUOLGraphicsLibrary/RenderTargetFlags.h"
#include "DUOLGraphicsLibrary/ShaderFlags.h"
#include "DUOLGraphicsLibrary/TextureFlags.h"
#include "DUOLGraphicsLibrary/Renderer/PipelineState.h"

namespace DUOLParser
{
	class DUOLParserBase;
}

namespace DUOLGraphicsLibrary
{
	struct SamplerDesc;
	class RenderPass;
	class RenderTarget;
	class Shader;
	class Buffer;
	class Renderer;
	class Sampler;
}

namespace DUOLGraphicsEngine
{
	struct Material;
	struct ResourceBundle;

	struct Mesh;

	class ResourceManager
	{
	public:
		ResourceManager(DUOLGraphicsLibrary::Renderer* renderer);

		~ResourceManager() = default;

	private:
		DUOLGraphicsLibrary::Renderer* _renderer;

		std::shared_ptr<DUOLParser::DUOLParserBase> _parser;

		//sceneID / ResourceBundle of Scene
		//std::unordered_map<DUOLCommon::tstring, std::unique_ptr<ResourceBundle>> _resourceBundles;
		//hash function https://en.wikipedia.org/wiki/Jenkins_hash_function#SpookyHash

		std::unordered_map<UINT64, DUOLGraphicsLibrary::Buffer*> _buffers;

		std::unordered_map<UINT64, DUOLGraphicsLibrary::Shader*> _shaders;

		std::unordered_map<UINT64, Material*> _materials;

		std::unordered_map<UINT64, DUOLGraphicsLibrary::Sampler*> _samplers;

		std::unordered_map<UINT64, Mesh*> _meshes;

		std::unordered_map<UINT64, DUOLGraphicsLibrary::Texture*> _textures;

		std::unordered_map<UINT64, DUOLGraphicsLibrary::RenderTarget*> _renderTargets;

		std::unordered_map<UINT64, DUOLGraphicsLibrary::RenderPass*> _renderPasses;

		std::unordered_map<UINT64, DUOLGraphicsLibrary::PipelineState*> _pipelineStates;

	private:
		DUOLGraphicsLibrary::Texture* LoadMaterialTexture(const DUOLCommon::tstring& path, DUOLCommon::tstring& fileID);

	public:
		DUOLGraphicsLibrary::Texture* CreateTexture(const DUOLCommon::tstring& objectID, const DUOLGraphicsLibrary::TextureDesc& textureDesc);

		DUOLGraphicsLibrary::Texture* CreateTexture(const UINT64& objectID, const DUOLGraphicsLibrary::TextureDesc& textureDesc);

		DUOLGraphicsLibrary::Texture* GetTexture(const DUOLCommon::tstring& objectID);

		DUOLGraphicsLibrary::Texture* GetTexture(const UINT64& objectID);

		Mesh* CreateMesh(const DUOLCommon::tstring& objectID, const DUOLCommon::tstring& path);

		Mesh* CreateMesh(const UINT64& objectID, const DUOLCommon::tstring& path);

		DUOLGraphicsLibrary::Buffer* CreateEmptyBuffer(const DUOLCommon::tstring& objectID, const DUOLGraphicsLibrary::BufferDesc& bufferDesc);

		DUOLGraphicsLibrary::Buffer* CreateEmptyBuffer(const UINT64& objectID, const DUOLGraphicsLibrary::BufferDesc& bufferDesc);

		DUOLGraphicsLibrary::Buffer* GetBuffer(const DUOLCommon::tstring& objectID);

		DUOLGraphicsLibrary::Buffer* GetBuffer(const UINT64& objectID);

		DUOLGraphicsLibrary::RenderTarget* CreateRenderTarget(const DUOLGraphicsLibrary::RenderTargetDesc& renderTargetDesc);

		DUOLGraphicsLibrary::Shader* CreateShader(const UINT64& objectID, const DUOLGraphicsLibrary::ShaderDesc& shaderDesc);

		DUOLGraphicsLibrary::RenderPass* CreateRenderPass(const UINT64& objectID, const DUOLGraphicsLibrary::RenderPassDesc& renderPassDesc);

		DUOLGraphicsLibrary::PipelineState* CreatePipelineState(const UINT64& objectID, const DUOLGraphicsLibrary::PipelineStateDesc& pipelineStateDesc);

		DUOLGraphicsLibrary::Sampler* CreateSampler(const UINT64& objectID, const DUOLGraphicsLibrary::SamplerDesc& samplerDesc);

		Material* RegistMaterial(const DUOLCommon::tstring& objectID,const MaterialDesc& materialDesc);

		Material* GetMaterial(const DUOLCommon::tstring& objectID);

	private:

	};
}