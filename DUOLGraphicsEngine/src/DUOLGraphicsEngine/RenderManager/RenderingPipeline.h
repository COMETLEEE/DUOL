#pragma once
#include <vector>

#include "DUOLGraphicsLibrary/PipelineStateFlags.h"
#include "DUOLGraphicsLibrary/RenderPassFlags.h"
#include "DUOLGraphicsLibrary/Renderer/ResourceViewLayout.h"
#include "DUOLCommon/StringHelper.h"
#include "DUOLGraphicsLibrary/RenderTargetFlags.h"
#include "DUOLGraphicsLibrary/TextureFlags.h"
#include "DUOLGraphicsLibrary/Renderer/RenderPass.h"

namespace DUOLGraphicsLibrary
{
	class RenderTarget;
	class Shader;
	class PipelineState;
	class Renderer;
	class Texture;
	class Buffer;
}



namespace DUOLGraphicsEngine
{
	class ResourceManager;

	enum class PipelineType
	{
		Render,
		PostProcessing
	};

	class RenderingPipeline
	{
	public:
		RenderingPipeline(
			DUOLGraphicsEngine::ResourceManager* resourceManager
			, DUOLGraphicsLibrary::Buffer* perFrameBuffer
			, DUOLGraphicsLibrary::Buffer* perObjectBuffer
			, const DUOLGraphicsLibrary::RenderPass& renderPass);

		RenderingPipeline(
			DUOLGraphicsEngine::ResourceManager* resourceManager
			, DUOLGraphicsLibrary::Buffer* perFrameBuffer
			, DUOLGraphicsLibrary::Buffer* perObjectBuffer
			, const PipelineType& pipelineType
			, const DUOLGraphicsLibrary::RenderPass& renderPass
			, const DUOLGraphicsLibrary::ResourceViewLayout& resourceViewLayout);

	private:
		DUOLGraphicsEngine::PipelineType _pipelineType;

		//∑ª¥ı∏µ ∆ƒ¿Ã«¡∂Û¿Œ ∑ª¥ı≈∏∞Ÿ
		DUOLGraphicsLibrary::RenderPass _renderPass;

		//∑ª¥ı∏µ ∆ƒ¿Ã«¡∂Û¿Œ Resources slot
		DUOLGraphicsLibrary::ResourceViewLayout _resourceViewLayout;

		DUOLGraphicsLibrary::PipelineState* _pipelineState;
	public:
		void SetPipelineState(DUOLGraphicsLibrary::PipelineState* pipeline_state)
		{
			_pipelineState = pipeline_state;
		}

		DUOLGraphicsLibrary::PipelineState* GetPipelineState() const
		{
			return _pipelineState;
		}

		void ChangeTexture(DUOLGraphicsLibrary::Texture* texture, int slot);

		void ChangeSampler(DUOLGraphicsLibrary::Sampler* sampler);

		DUOLGraphicsLibrary::Buffer* GetPerFrameBuffer();
		DUOLGraphicsLibrary::Buffer* GetPerObjectBuffer();

		DUOLGraphicsLibrary::RenderPass* GetRenderPass()
		{
			return &_renderPass;
		}

		DUOLGraphicsLibrary::ResourceViewLayout GetResourceViewLayout() const
		{
			return _resourceViewLayout;
		}

		DUOLGraphicsEngine::PipelineType GetPipelineType() const
		{
			return _pipelineType;
		}
	};
}
