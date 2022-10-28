#pragma once
#include "DUOLGraphicsEngine/GraphicsEngineFlags.h"
#include <memory>

namespace DUOLGraphicsLibrary
{
	class Renderer;
}

namespace DUOLGraphicsEngine
{
	class ResourceManager;

	class RenderManager;

 /**
     @class   GraphicsEngine
     @brief   �׷��Ƚ� ���� 
     @details ~
     @author  KyungMin Oh

 **/
	class GraphicsEngine
	{
	public:
		GraphicsEngine(const GraphicsEngineDesc& engineDesc);

		~GraphicsEngine();

	private:
		DUOLGraphicsLibrary::Renderer* _renderer;

		std::unique_ptr<ResourceManager> _resourceManager;

		std::unique_ptr<RenderManager> _renderManager;
	};
}