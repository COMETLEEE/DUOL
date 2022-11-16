#include "RenderPipieline.h"

#include "DUOLGraphicsEngine/ResourceManager/ResourceManager.h"
#include "DUOLGraphicsEngine/Util/Hash/Hash.h"
#include "DUOLGraphicsLibrary/Renderer/Renderer.h"

namespace DUOLGraphicsEngine
{

	RenderPipieline::RenderPipieline(const DUOLCommon::tstring& name,
		DUOLGraphicsEngine::ResourceManager* resourceManager, DUOLGraphicsLibrary::Buffer* perFrameBuffer,
		DUOLGraphicsLibrary::Buffer* perObjectBuffer, const DUOLGraphicsLibrary::RenderPass& renderPassDesc,
		const DUOLGraphicsLibrary::PipelineStateDesc& pipelineDesc) :
		_pipelineDesc(pipelineDesc)
		,_renderPass(renderPassDesc)
	{
		auto pipelineID = Hash::Hash64(name);

		_pipielineState = resourceManager->CreatePipelineState(pipelineID, pipelineDesc);

		//todo shader Reflection을 통하여.. 바인딩 point를 만들어줄 수... 있나?
		_resourceViewLayout._resourceViews.reserve(12);

		//buffer
		_resourceViewLayout._resourceViews.emplace_back(perFrameBuffer, 0, static_cast<long>(DUOLGraphicsLibrary::BindFlags::CONSTANTBUFFER), static_cast<long>(DUOLGraphicsLibrary::StageFlags::VSPS));
		_resourceViewLayout._resourceViews.emplace_back(perObjectBuffer, 1, static_cast<long>(DUOLGraphicsLibrary::BindFlags::CONSTANTBUFFER), static_cast<long>(DUOLGraphicsLibrary::StageFlags::VSPS));

		//sampler
		DUOLGraphicsLibrary::SamplerDesc samplerDesc;

		_resourceViewLayout._resourceViews.emplace_back(resourceManager->CreateSampler(1, samplerDesc) , 0, static_cast<long>(DUOLGraphicsLibrary::BindFlags::SAMPLER), static_cast<long>(DUOLGraphicsLibrary::StageFlags::VSPS));
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
	}

	RenderPipieline::RenderPipieline(
		const DUOLCommon::tstring& name
		, DUOLGraphicsEngine::ResourceManager* resourceManager
		, DUOLGraphicsLibrary::Buffer* perFrameBuffer
		, DUOLGraphicsLibrary::Buffer* perObjectBuffer
		, RenderTargetDesc* renderTargetDesc
		, int renderTargetCount
		, RenderTargetDesc& depthSteniclDesc
		, const DUOLGraphicsLibrary::PipelineStateDesc& pipelineDesc) :
		_pipelineDesc(pipelineDesc)
	{
		auto pipelineID = Hash::Hash64(name);

		//texture naming 
		for (int textureIndex = 0; textureIndex < renderTargetCount; textureIndex++)
		{
			DUOLCommon::tstring strTextureID = name + std::to_wstring(textureIndex);
			auto textureID = Hash::Hash64(strTextureID);

			_renderTargetTextures.push_back(resourceManager->CreateTexture(textureID, renderTargetDesc[textureIndex]._textureDesc));\

			renderTargetDesc[textureIndex]._renderTargetDesc._texture = _renderTargetTextures[textureIndex];
			_renderTargets.push_back(resourceManager->CreateRenderTarget(renderTargetDesc[textureIndex]._renderTargetDesc));
		}

		{
			//texture naming 
			DUOLCommon::tstring strTextureID = name + (_T("Depth"));
			auto textureID = Hash::Hash64(strTextureID);

			_depthStencilTexture = resourceManager->CreateTexture(textureID, depthSteniclDesc._textureDesc);
			depthSteniclDesc._renderTargetDesc._texture = _depthStencilTexture;
			_depthStencilView = resourceManager->CreateRenderTarget(depthSteniclDesc._renderTargetDesc);
		}

		_renderPass._renderTargetViewRefs.reserve(_renderTargetTextures.size());

		for (auto& target : _renderTargets)
		{
			_renderPass._renderTargetViewRefs.emplace_back(target);
		}
		_renderPass._depthStencilViewRef = _depthStencilView;

		_pipielineState = resourceManager->CreatePipelineState(pipelineID, pipelineDesc);

		//todo shader Reflection을 통하여.. 바인딩 point를 만들어줄 수... 있나?
		_resourceViewLayout._resourceViews.reserve(12);

		//buffer
		_resourceViewLayout._resourceViews.emplace_back(perFrameBuffer, 0, static_cast<long>(DUOLGraphicsLibrary::BindFlags::CONSTANTBUFFER), static_cast<long>(DUOLGraphicsLibrary::StageFlags::VSPS));
		_resourceViewLayout._resourceViews.emplace_back(perObjectBuffer, 1, static_cast<long>(DUOLGraphicsLibrary::BindFlags::CONSTANTBUFFER), static_cast<long>(DUOLGraphicsLibrary::StageFlags::VSPS));

		//sampler
		_resourceViewLayout._resourceViews.emplace_back(nullptr, 0, static_cast<long>(DUOLGraphicsLibrary::BindFlags::SAMPLER), static_cast<long>(DUOLGraphicsLibrary::StageFlags::VSPS));
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
	}


	void RenderPipieline::ReserveVector()
	{
		_renderTargetTextures.reserve(8);
		_renderTargets.reserve(8);
	}

	void RenderPipieline::ChangeTexture(DUOLGraphicsLibrary::Texture* texture, int slot)
	{
		if (slot > 7 || slot < 0)
		{
			return;
		}

		_resourceViewLayout._resourceViews[slot + 4]._resource = texture;
	}

	void RenderPipieline::ChangeSampler(DUOLGraphicsLibrary::Sampler* sampler)
	{
		_resourceViewLayout._resourceViews[2]._resource = sampler;
	}

	DUOLGraphicsLibrary::Buffer* RenderPipieline::GetPerFrameBuffer()
	{
		return static_cast<DUOLGraphicsLibrary::Buffer*>(_resourceViewLayout._resourceViews[0]._resource);
	}

	DUOLGraphicsLibrary::Buffer* RenderPipieline::GetPerObjectBuffer()
	{
		return static_cast<DUOLGraphicsLibrary::Buffer*>(_resourceViewLayout._resourceViews[1]._resource);
	}
}
