#pragma once
#include "DUOLGraphicsEngine/Export.h"
#include "DUOLGraphicsEngine/GraphicsEngineFlags.h"
#include "DUOLCommon/StringHelper.h"

#include <memory>

#include "DUOLGraphicsEngine/ResourceManager/Resource/RenderContantBuffer.h"

namespace DUOLGraphicsLibrary
{
	class RenderContext;
	class Buffer;
	class Renderer;
}

namespace DUOLGraphicsEngine
{
	class RenderPipieline;
	struct Mesh;
	struct RenderObject;
	class ResourceManager;

	class RenderManager;

 /**
     @class   GraphicsEngine
     @brief   그래픽스 엔진 
     @details ~
     @author  KyungMin Oh

 **/
	class DUOLGRAPHICSENGINE_EXPORT GraphicsEngine
	{
	public:
		GraphicsEngine(const GraphicsEngineDesc& engineDesc);

		~GraphicsEngine();

	public:
		

	private:
		DUOLGraphicsLibrary::Renderer* _renderer;

		DUOLGraphicsLibrary::RenderContext* _context;

		DUOLGraphicsLibrary::Buffer* _perFrameBuffer;

		DUOLGraphicsLibrary::Buffer* _perObjectBuffer;

		std::unique_ptr<ResourceManager> _resourceManager;

		std::unique_ptr<RenderManager> _renderManager;

		RenderPipieline* pipeline;

	private:
		void CreateDefaultRenderPass(const DUOLMath::Vector2& screenSize);

		void Initialize();

	public:
		void RenderObject(const RenderObject* object);

		void Execute(const ConstantBufferPerFrame& perFrameInfo);

		void Present();

		Mesh* LoadMesh(const DUOLCommon::tstring& objectID);

		Material* LoadMaterial(const DUOLCommon::tstring& objectID);

		void LoadMeshTable(const DUOLCommon::tstring& path);
	};

	
}
