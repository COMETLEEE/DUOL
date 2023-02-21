#include "DUOLGraphicsEngine/RenderManager/RenderingPipeline/RenderingPipeline.h"

#include "DUOLGraphicsEngine/ResourceManager/ResourceManager.h"
#include "DUOLGraphicsLibrary/Renderer/Renderer.h"

namespace DUOLGraphicsEngine
{

	RenderingPipeline::RenderingPipeline(
		DUOLGraphicsEngine::ResourceManager* resourceManager
		, const PipelineType& pipelineType
		, const DUOLGraphicsLibrary::RenderPass& renderPass
		, const DUOLGraphicsLibrary::ResourceViewLayout& textureResourceViewLayout
		, const DUOLGraphicsLibrary::ResourceViewLayout& samplerResourceViewLayout) :
		_pipelineType(pipelineType), _renderPass(renderPass)
	{
		//todo shader Reflection을 통하여.. 바인딩 point를 만들어줄 수... 있나?
		_textureResourceViewLayout._resourceViews.reserve(8);

		//texture
		_textureResourceViewLayout._resourceViews.emplace_back(nullptr, 0, static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE), static_cast<long>(DUOLGraphicsLibrary::StageFlags::VSPS)	| static_cast<long>(DUOLGraphicsLibrary::StageFlags::GEOMETRYSTAGE));
		_textureResourceViewLayout._resourceViews.emplace_back(nullptr, 1, static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE), static_cast<long>(DUOLGraphicsLibrary::StageFlags::VSPS)	| static_cast<long>(DUOLGraphicsLibrary::StageFlags::GEOMETRYSTAGE));
		_textureResourceViewLayout._resourceViews.emplace_back(nullptr, 2, static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE), static_cast<long>(DUOLGraphicsLibrary::StageFlags::VSPS)	| static_cast<long>(DUOLGraphicsLibrary::StageFlags::GEOMETRYSTAGE));
		_textureResourceViewLayout._resourceViews.emplace_back(nullptr, 3, static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE), static_cast<long>(DUOLGraphicsLibrary::StageFlags::VSPS)	| static_cast<long>(DUOLGraphicsLibrary::StageFlags::GEOMETRYSTAGE));
		_textureResourceViewLayout._resourceViews.emplace_back(nullptr, 4, static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE), static_cast<long>(DUOLGraphicsLibrary::StageFlags::VSPS)	| static_cast<long>(DUOLGraphicsLibrary::StageFlags::GEOMETRYSTAGE));
		_textureResourceViewLayout._resourceViews.emplace_back(nullptr, 5, static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE), static_cast<long>(DUOLGraphicsLibrary::StageFlags::VSPS)	| static_cast<long>(DUOLGraphicsLibrary::StageFlags::GEOMETRYSTAGE));
		_textureResourceViewLayout._resourceViews.emplace_back(nullptr, 6, static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE), static_cast<long>(DUOLGraphicsLibrary::StageFlags::VSPS)	| static_cast<long>(DUOLGraphicsLibrary::StageFlags::GEOMETRYSTAGE));
		_textureResourceViewLayout._resourceViews.emplace_back(nullptr, 7, static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE), static_cast<long>(DUOLGraphicsLibrary::StageFlags::VSPS)	| static_cast<long>(DUOLGraphicsLibrary::StageFlags::GEOMETRYSTAGE));

		for (int inputResourceViewIndex = 0; inputResourceViewIndex < textureResourceViewLayout._resourceViews.size(); inputResourceViewIndex++)
		{
			_textureResourceViewLayout._resourceViews[inputResourceViewIndex] = textureResourceViewLayout._resourceViews[inputResourceViewIndex];
		}

		_samplerResourceViewLayout = samplerResourceViewLayout;
	}

	void RenderingPipeline::SetName(const DUOLCommon::tstring& name)
	{
		_pipelineName = name;
	}

	const DUOLCommon::tstring& RenderingPipeline::GetName()
	{
		return _pipelineName;
	}

	DUOLGraphicsEngine::PipelineType RenderingPipeline::GetPipelineType() const
	{
		return _pipelineType;
	}

	void RenderingPipeline::SetPipelineType(DUOLGraphicsEngine::PipelineType pipeline_type)
	{
		_pipelineType = pipeline_type;
	}

	DUOLGraphicsLibrary::RenderPass* RenderingPipeline::GetRenderPass()
	{
		return &_renderPass;
	}

	void RenderingPipeline::SetRenderPass(DUOLGraphicsLibrary::RenderPass renderPass)
	{
		_renderPass = renderPass;
	}

	DUOLGraphicsLibrary::ResourceViewLayout& RenderingPipeline::GetTextureResourceViewLayout()
	{
		return _textureResourceViewLayout;
	}

	void RenderingPipeline::SetTextureResourceViewLayout(
		const DUOLGraphicsLibrary::ResourceViewLayout& textureResourceViewLayout)
	{
		_textureResourceViewLayout = textureResourceViewLayout;
	}

	DUOLGraphicsLibrary::PipelineState* RenderingPipeline::GetPipelineState() const
	{
		return _pipelineState;
	}

	void RenderingPipeline::SetPipelineState(DUOLGraphicsLibrary::PipelineState* pipelineState)
	{
		_pipelineState = pipelineState;
	}

	void RenderingPipeline::SetTexture(DUOLGraphicsLibrary::Texture* texture, int slot)
	{
		if(_textureResourceViewLayout._resourceViews.size() < slot)
			_textureResourceViewLayout._resourceViews.resize(slot);

		_textureResourceViewLayout._resourceViews[slot] = DUOLGraphicsLibrary::ResourceViewDesc{texture, static_cast<unsigned>(slot),static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE), static_cast<long>(DUOLGraphicsLibrary::StageFlags::VSPS) | static_cast<long>(DUOLGraphicsLibrary::StageFlags::GEOMETRYSTAGE)};
	}

	DUOLGraphicsLibrary::ResourceViewLayout& RenderingPipeline::GetSamplerResourceViewLayout() 
	{
		return _samplerResourceViewLayout;
	}

	void RenderingPipeline::SetSamplerResourceViewLayout(
		const DUOLGraphicsLibrary::ResourceViewLayout& samplerResourceViewLayout)
	{
		_samplerResourceViewLayout = samplerResourceViewLayout;
	}
}
