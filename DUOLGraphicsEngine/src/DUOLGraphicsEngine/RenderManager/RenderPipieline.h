#pragma once
#include <vector>

#include "DUOLGraphicsLibrary/PipelineStateFlags.h"
#include "DUOLGraphicsLibrary/RenderPassFlags.h"
#include "DUOLGraphicsLibrary/Renderer/ResourceViewLayout.h"
#include "DUOLCommon/StringHelper.h"
#include "DUOLGraphicsLibrary/RenderTargetFlags.h"
#include "DUOLGraphicsLibrary/TextureFlags.h"

namespace DUOLGraphicsLibrary
{
	class RenderTarget;
	class Shader;
	class PipelineState;
	class Renderer;
	class Texture;
	class Buffer;
	class RenderPass;
}

namespace DUOLGraphicsEngine
{
	struct RenderTargetDesc
	{
		DUOLGraphicsLibrary::TextureDesc _textureDesc;

		DUOLGraphicsLibrary::RenderTargetDesc _renderTargetDesc;
	};

	class ResourceManager;

	class RenderPipieline
	{
	public:
		RenderPipieline(
			const DUOLCommon::tstring& name
			, DUOLGraphicsEngine::ResourceManager* resourceManager
			, DUOLGraphicsLibrary::Buffer* perFrameBuffer
			, DUOLGraphicsLibrary::Buffer* perObjectBuffer
			, const DUOLGraphicsLibrary::RenderPassDesc& renderPassDesc
			, const DUOLGraphicsLibrary::PipelineStateDesc& pipelineDesc);

		RenderPipieline(
			const DUOLCommon::tstring& name
			, DUOLGraphicsEngine::ResourceManager* resourceManager
			, DUOLGraphicsLibrary::Buffer* perFrameBuffer
			, DUOLGraphicsLibrary::Buffer* perObjectBuffer
			, RenderTargetDesc* renderTargetDesc
			, int renderTargetCount
			, RenderTargetDesc& depthSteniclDesc
			, const DUOLGraphicsLibrary::PipelineStateDesc& pipelineDesc);


	private:

		std::vector<DUOLGraphicsLibrary::Texture*> _renderTargetTextures;

		std::vector<DUOLGraphicsLibrary::RenderTarget*> _renderTargets;

		DUOLGraphicsLibrary::Texture* _depthStencilTexture;

		DUOLGraphicsLibrary::RenderTarget* _depthStencilView;

		//렌더링 파이프라인 렌더타겟
		DUOLGraphicsLibrary::RenderPass* _renderPass;

		//렌더링 파이프라인 PSO
		DUOLGraphicsLibrary::PipelineState* _pipielineState;

		DUOLGraphicsLibrary::PipelineStateDesc _pipelineDesc;

		//DUOLGraphicsLibrary::Shader* _vertexShader;

		//DUOLGraphicsLibrary::Shader* _pixelShader;

		//DUOLGraphicsLibrary::Shader* _hullShader;

		//DUOLGraphicsLibrary::Shader* _domainShader;

		//DUOLGraphicsLibrary::Shader* _geometryShader;

		//렌더링 파이프라인 Resources slot
		DUOLGraphicsLibrary::ResourceViewLayout _resourceViewLayout;

	private:
		void ReserveVector();

	public:
		void ChangeTexture(DUOLGraphicsLibrary::Texture* texture, int slot);

		void ChangeSampler(DUOLGraphicsLibrary::Sampler* sampler);

		DUOLGraphicsLibrary::Buffer* GetPerFrameBuffer();
		DUOLGraphicsLibrary::Buffer* GetPerObjectBuffer();


		DUOLGraphicsLibrary::RenderPass* GetRenderPass() const
		{
			return _renderPass;
		}

		DUOLGraphicsLibrary::PipelineState* GetPipielineState() const
		{
			return _pipielineState;
		}

		DUOLGraphicsLibrary::ResourceViewLayout GetResourceViewLayout() const
		{
			return _resourceViewLayout;
		}
	};
}
