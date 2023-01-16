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
		PostProcessing,
		OrderIndependentTransparency
	};

	class RenderingPipeline
	{
	public:
		RenderingPipeline(
			DUOLGraphicsEngine::ResourceManager* resourceManager
			, const PipelineType& pipelineType
			, const DUOLGraphicsLibrary::RenderPass& renderPass
			, const DUOLGraphicsLibrary::ResourceViewLayout& textureResourceViewLayout
			, const DUOLGraphicsLibrary::ResourceViewLayout& samplerResourceViewLayout);

	private:
		DUOLGraphicsEngine::PipelineType _pipelineType;

		//������ ���������� ����Ÿ��
		DUOLGraphicsLibrary::RenderPass _renderPass;

		//������ ���������� Resources slot for ����Ʈ���μ��� �ؽ��� ���ε�
		DUOLGraphicsLibrary::ResourceViewLayout _textureResourceViewLayout;

		DUOLGraphicsLibrary::ResourceViewLayout _samplerResourceViewLayout;

		//������ ����������  ����Ʈ���μ����� ���Ѱ�
		DUOLGraphicsLibrary::PipelineState* _pipelineState;

		DUOLCommon::tstring _pipelineName;

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

		void SetName(const DUOLCommon::tstring& name);

		const DUOLCommon::tstring& GetName();

		DUOLGraphicsLibrary::RenderPass* GetRenderPass()
		{
			return &_renderPass;
		}

		DUOLGraphicsLibrary::ResourceViewLayout GetTextureResourceViewLayout() const
		{
			return _textureResourceViewLayout;
		}

		DUOLGraphicsLibrary::ResourceViewLayout GetSamplerResourceViewLayout() const
		{
			return _samplerResourceViewLayout;
		}

		DUOLGraphicsEngine::PipelineType GetPipelineType() const
		{
			return _pipelineType;
		}
	};
}
