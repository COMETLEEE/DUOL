#include "GraphicsEngine.h"
#include "DUOLGraphicsLibrary/Renderer/Renderer.h"
#include "DUOLGraphicsEngine/ResourceManager/ResourceManager.h"
#include "DUOLGraphicsEngine/RenderManager/RenderManager.h"

namespace DUOLGraphicsEngine
{
	GraphicsEngine::GraphicsEngine(const GraphicsEngineDesc& engineDesc)
	{
		_renderer = DUOLGraphicsLibrary::Renderer::CreateRenderer(engineDesc._rendererDesc);

		_resourceManager = std::make_unique<ResourceManager>(_renderer);
		_renderManager = std::make_unique<RenderManager>(_renderer);
	}

	GraphicsEngine::~GraphicsEngine()
	{
		DUOLGraphicsLibrary::Renderer::DeleteRenderer(_renderer);
	}
}
