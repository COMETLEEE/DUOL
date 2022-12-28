#include "RenderingPipeline.h"

#include "DUOLGraphicsEngine/ResourceManager/ResourceManager.h"
#include "DUOLGraphicsEngine/Util/Hash/Hash.h"
#include "DUOLGraphicsLibrary/Renderer/Renderer.h"

namespace DUOLGraphicsEngine
{

	RenderingPipeline::RenderingPipeline(
		DUOLGraphicsEngine::ResourceManager* resourceManager, DUOLGraphicsLibrary::Buffer* perFrameBuffer,
		DUOLGraphicsLibrary::Buffer* perObjectBuffer, const DUOLGraphicsLibrary::RenderPass& renderPass) :
		_renderPass(renderPass)
	{
	}

	RenderingPipeline::RenderingPipeline(
		DUOLGraphicsEngine::ResourceManager* resourceManager
		, DUOLGraphicsLibrary::Buffer* perFrameBuffer
		, DUOLGraphicsLibrary::Buffer* perObjectBuffer
		, const PipelineType& pipelineType
		, const DUOLGraphicsLibrary::RenderPass& renderPass
		, const DUOLGraphicsLibrary::ResourceViewLayout& resourceViewLayout) :
		_pipelineType(pipelineType), _renderPass(renderPass)
	{

		//todo shader Reflection을 통하여.. 바인딩 point를 만들어줄 수... 있나?
		_resourceViewLayout._resourceViews.reserve(12);

		//buffer
		_resourceViewLayout._resourceViews.emplace_back(perFrameBuffer, 0, static_cast<long>(DUOLGraphicsLibrary::BindFlags::CONSTANTBUFFER), static_cast<long>(DUOLGraphicsLibrary::StageFlags::VSPS) | static_cast<long>(DUOLGraphicsLibrary::StageFlags::GEOMETRYSTAGE));
		_resourceViewLayout._resourceViews.emplace_back(perObjectBuffer, 1, static_cast<long>(DUOLGraphicsLibrary::BindFlags::CONSTANTBUFFER), static_cast<long>(DUOLGraphicsLibrary::StageFlags::VSPS) | static_cast<long>(DUOLGraphicsLibrary::StageFlags::GEOMETRYSTAGE));

		//sampler
		DUOLGraphicsLibrary::SamplerDesc samplerDesc;

		_resourceViewLayout._resourceViews.emplace_back(resourceManager->CreateSampler(1, samplerDesc), 0, static_cast<long>(DUOLGraphicsLibrary::BindFlags::SAMPLER), static_cast<long>(DUOLGraphicsLibrary::StageFlags::VSPS) | static_cast<long>(DUOLGraphicsLibrary::StageFlags::GEOMETRYSTAGE));
		_resourceViewLayout._resourceViews.emplace_back(nullptr, 1, static_cast<long>(DUOLGraphicsLibrary::BindFlags::SAMPLER), static_cast<long>(DUOLGraphicsLibrary::StageFlags::VSPS));

		//sampler
		_resourceViewLayout._resourceViews.emplace_back(nullptr, 0, static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE), static_cast<long>(DUOLGraphicsLibrary::StageFlags::VSPS));
		_resourceViewLayout._resourceViews.emplace_back(nullptr, 1, static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE), static_cast<long>(DUOLGraphicsLibrary::StageFlags::VSPS));
		_resourceViewLayout._resourceViews.emplace_back(nullptr, 2, static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE), static_cast<long>(DUOLGraphicsLibrary::StageFlags::VSPS));
		_resourceViewLayout._resourceViews.emplace_back(nullptr, 3, static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE), static_cast<long>(DUOLGraphicsLibrary::StageFlags::VSPS));
		_resourceViewLayout._resourceViews.emplace_back(nullptr, 4, static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE), static_cast<long>(DUOLGraphicsLibrary::StageFlags::VSPS));
		_resourceViewLayout._resourceViews.emplace_back(nullptr, 5, static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE), static_cast<long>(DUOLGraphicsLibrary::StageFlags::VSPS));
		_resourceViewLayout._resourceViews.emplace_back(nullptr, 6, static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE), static_cast<long>(DUOLGraphicsLibrary::StageFlags::VSPS));
		_resourceViewLayout._resourceViews.emplace_back(nullptr, 7, static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE), static_cast<long>(DUOLGraphicsLibrary::StageFlags::VSPS));

		for (int inputResourceViewIndex = 0; inputResourceViewIndex < resourceViewLayout._resourceViews.size(); inputResourceViewIndex++)
		{
			_resourceViewLayout._resourceViews[inputResourceViewIndex + 4] = resourceViewLayout._resourceViews[inputResourceViewIndex];
		}
	}

	void RenderingPipeline::ChangeTexture(DUOLGraphicsLibrary::Texture* texture, int slot)
	{
		_resourceViewLayout._resourceViews[4 + slot]._resource = texture;

	}

	void RenderingPipeline::ChangeSampler(DUOLGraphicsLibrary::Sampler* sampler)
	{
	}

	DUOLGraphicsLibrary::Buffer* RenderingPipeline::GetPerFrameBuffer()
	{
		return static_cast<DUOLGraphicsLibrary::Buffer*>(_resourceViewLayout._resourceViews[0]._resource);
	}

	DUOLGraphicsLibrary::Buffer* RenderingPipeline::GetPerObjectBuffer()
	{
		return static_cast<DUOLGraphicsLibrary::Buffer*>(_resourceViewLayout._resourceViews[1]._resource);
	}
}
