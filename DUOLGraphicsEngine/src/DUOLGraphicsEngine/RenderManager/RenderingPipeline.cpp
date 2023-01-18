#include "RenderingPipeline.h"

#include "DUOLGraphicsEngine/ResourceManager/ResourceManager.h"
#include "DUOLGraphicsEngine/Util/Hash/Hash.h"
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

		_samplerResourceViewLayout._resourceViews.reserve(4);

		_samplerResourceViewLayout = samplerResourceViewLayout;
	}

	void RenderingPipeline::ChangeTexture(DUOLGraphicsLibrary::Texture* texture, int slot)
	{
		_textureResourceViewLayout._resourceViews[slot]._resource = texture;
	}

	void RenderingPipeline::SetName(const DUOLCommon::tstring& name)
	{
		_pipelineName = name;
	}

	const DUOLCommon::tstring& RenderingPipeline::GetName()
	{
		return _pipelineName;
	}
}
