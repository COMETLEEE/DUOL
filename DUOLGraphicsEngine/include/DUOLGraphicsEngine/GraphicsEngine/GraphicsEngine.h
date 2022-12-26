#pragma once
#include "DUOLGraphicsEngine/Export.h"
#include "DUOLGraphicsEngine/GraphicsEngineFlags.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/RenderConstantBuffer.h"

#include "DUOLCommon/StringHelper.h"
#include <memory>

namespace DUOLGraphicsLibrary
{
	class RenderContext;
	class Buffer;
	class Renderer;
}

namespace DUOLGraphicsEngine
{
	class AnimationClip;
	class Model;
	class Material;
	class RenderingPipeline;
	class MeshBase;
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
		void RenderObject(const DUOLGraphicsEngine::RenderObject* object);

		void RenderDebugObject(const DUOLGraphicsEngine::RenderObject* object);

		void Execute(const ConstantBufferPerFrame& perFrameInfo);

		void Present();

		void OnReszie(const DUOLMath::Vector2& resolution);

		void UpdateMesh(MeshBase* mesh, void* vertices, UINT vertexSize, void* indices, UINT indexSize);

		Model* CreateModelFromFBX(const DUOLCommon::tstring& objectID, const DUOLCommon::tstring& path);

		MeshBase* CreateMesh(const DUOLCommon::tstring& objectID, void* vertices, UINT vertexSize, UINT vertexStructureSize, void* indices, UINT indexSize);
		
		MeshBase* CreateParticle(const DUOLCommon::tstring&, int maxParticle);

		Model* LoadModel(const DUOLCommon::tstring& objectID);

		bool ReadMeshInfo(const DUOLCommon::tstring& objectID, std::vector<DUOLMath::Vector3>& vertexInfo, std::vector<UINT32>& indexInfo);

		bool ReadMeshInfo(const MeshBase* mesh, std::vector<DUOLMath::Vector3>& vertexInfo, std::vector<UINT32>& indexInfo);

		MeshBase* LoadMesh(const DUOLCommon::tstring& objectID);

		Material* LoadMaterial(const DUOLCommon::tstring& objectID);

		AnimationClip* LoadAnimationClip(const DUOLCommon::tstring& objectID);

		void LoadMeshTable(const DUOLCommon::tstring& path);
	};


}
