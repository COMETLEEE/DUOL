#pragma once
#include "DUOLGraphicsEngine/Export.h"
#include "DUOLGraphicsEngine/GraphicsEngineFlags.h"
#include "DUOLCommon/StringHelper.h"

#include <memory>

#include "DUOLGraphicsEngine/ResourceManager/Resource/RenderConstantBuffer.h"

namespace DUOLGraphicsLibrary
{
	class RenderContext;
	class Buffer;
	class Renderer;
}

namespace DUOLGraphicsEngine
{
	class RenderingPipeline;
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

	private:
		DUOLGraphicsLibrary::Renderer* _renderer;

		DUOLGraphicsLibrary::RenderContext* _context;

		std::unique_ptr<ResourceManager> _resourceManager;

		std::unique_ptr<RenderManager> _renderManager;

	private:
		void LoadRenderingPipelineTables(const DUOLMath::Vector2& screenSize);

		void Initialize();

	public:
		void RenderObject(const RenderObject* object);

		void Execute(const ConstantBufferPerFrame& perFrameInfo);

		void Present();

		void OnReszie(const DUOLMath::Vector2& resolution);

		Mesh* LoadMesh(const DUOLCommon::tstring& objectID);

		Mesh* CreateMesh(const DUOLCommon::tstring& objectID, const DUOLCommon::tstring& path);

		Mesh* CreateMesh(const DUOLCommon::tstring& objectID, void* vertices, UINT vertexSize, UINT vertexStructureSize, void* indices, UINT indexSize);

		void UpdateMesh(const Mesh* mesh, void* vertices, UINT vertexSize, void* indices, UINT indexSize);

		Material* LoadMaterial(const DUOLCommon::tstring& objectID);

		void LoadMeshTable(const DUOLCommon::tstring& path);
	};

}
