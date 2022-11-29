#include "DUOLGraphicsLibrary/Renderer/Renderer.h"
#include "DUOLGraphicsEngine/GraphicsEngine/GraphicsEngine.h"
#include "DUOLGraphicsEngine/ResourceManager/ResourceManager.h"
#include "DUOLGraphicsEngine/RenderManager/RenderManager.h"
#include "DUOLGraphicsEngine/Util/Hash/Hash.h"
#include "DUOLGraphicsEngine/RenderManager/RenderingPipeline.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/RenderConstantBuffer.h"
#include "DUOLJson/JsonReader.h"

#include <tchar.h>

#include "DUOLGraphicsEngine/TableLoader/TableLoader.h"


namespace DUOLGraphicsEngine
{
	GraphicsEngine::GraphicsEngine(const GraphicsEngineDesc& engineDesc)
	{
		DUOLGraphicsLibrary::RendererDesc renderDesc;

		renderDesc._handle = engineDesc._handle;
		renderDesc._moduleType = static_cast<DUOLGraphicsLibrary::ModuleType>(engineDesc._moduleType);

		DUOLGraphicsLibrary::RenderContextDesc renderContextDesc;

		renderContextDesc._screenDesc._screenSize = engineDesc._screenSize;
		renderContextDesc._screenDesc._isFullscreen = engineDesc._isFullscreen;
		renderContextDesc._screenDesc._isMSAA = engineDesc._isMSAA;
		renderContextDesc._screenDesc._sampleCount = engineDesc._sampleCount;

		_renderer = DUOLGraphicsLibrary::Renderer::CreateRenderer(renderDesc);

		_context = _renderer->CreateRenderContext(renderContextDesc);
		//_resourceManager = std::make_unique<ResourceManager>(_renderer);
		//_renderManager = std::make_unique<RenderManager>(_renderer, _context);

		Initialize();
		_renderManager->OnResize(renderContextDesc._screenDesc._screenSize);
		LoadRenderingPipelineTables(renderContextDesc._screenDesc._screenSize);
		_resourceManager->CreateDebugMaterial(_context->GetBackBufferRenderTarget());
	}

	GraphicsEngine::~GraphicsEngine()
	{
		DUOLGraphicsLibrary::Renderer::DeleteRenderer(_renderer);
	}

	void GraphicsEngine::LoadRenderingPipelineTables(const DUOLMath::Vector2& screenSize)
	{
		TableLoader::LoadRenderTargetTable(_resourceManager.get(), screenSize);
		TableLoader::LoadShaderTable(_resourceManager.get());
		TableLoader::LoadPipelineStateTable(_resourceManager.get());
		TableLoader::LoadRenderingPipelineTable(_resourceManager.get());
	}

	void GraphicsEngine::Initialize()
	{
		_resourceManager = std::make_unique<ResourceManager>(_renderer);
		_renderManager = std::make_unique<RenderManager>(_renderer, _context);
		_resourceManager->AddBackbufferRenderTarget(_context->GetBackBufferRenderTarget());
	}

	void GraphicsEngine::RenderObject(const DUOLGraphicsEngine::RenderObject* object)
	{
		_renderManager->Render(*object);
	}

	void GraphicsEngine::RenderDebugObject(const DUOLGraphicsEngine::RenderObject* object)
	{
		_renderManager->RenderDebug(*object);
	}

	void GraphicsEngine::Execute(const ConstantBufferPerFrame& perFrameInfo)
	{
		_resourceManager->ClearRenderTargets();

		static UINT64 debug = Hash::Hash64(_T("Debug"));
		static UINT64 debugRT = Hash::Hash64(_T("DebugRT"));

		_renderManager->ExecuteDebugRenderPass(_resourceManager->GetRenderingPipeline(debug), perFrameInfo);

		static UINT64 id = Hash::Hash64(_T("Default"));
		static UINT64 deferred = Hash::Hash64(_T("Lighting"));
		static UINT64 merge = Hash::Hash64(_T("Merge"));

		_renderManager->ExecuteRenderingPipeline(_resourceManager->GetRenderingPipeline(id), perFrameInfo);
		_renderManager->ExecuteRenderingPipeline(_resourceManager->GetRenderingPipeline(deferred), perFrameInfo);
		_renderManager->ExecuteRenderingPipeline(_resourceManager->GetRenderingPipeline(merge), perFrameInfo);

		_renderManager->ExecuteDebugRenderTargetPass(_resourceManager->GetRenderingPipeline(debugRT), perFrameInfo);
	}

	void GraphicsEngine::Present()
	{
		_renderManager->Present();
	}

	void GraphicsEngine::OnReszie(const DUOLMath::Vector2& resolution)
	{
		DUOLGraphicsLibrary::ScreenDesc screenDesc;

		screenDesc._screenSize = resolution;
		screenDesc._isMSAA = false;
		screenDesc._isFullscreen = false;
		screenDesc._sampleCount = 1;

		_context->SetScreenDesc(screenDesc);
		_renderManager->OnResize(resolution);
		_resourceManager->OnResize(resolution);
	}

	Mesh* GraphicsEngine::LoadMesh(const DUOLCommon::tstring& objectID)
	{
		return _resourceManager->GetMesh(objectID);
	}

	Mesh* GraphicsEngine::CreateMesh(const DUOLCommon::tstring& objectID, const DUOLCommon::tstring& path)
	{
		return _resourceManager->CreateMesh(objectID, path);
	}

	Mesh* GraphicsEngine::CreateMesh(const DUOLCommon::tstring& objectID, void* vertices, UINT vertexSize, UINT vertexStructureSize, void* indices,
		UINT indexSize)
	{
		return _resourceManager->CreateMesh(objectID, vertices, vertexSize, vertexStructureSize, indices, indexSize);
	}

	void GraphicsEngine::UpdateMesh(Mesh* mesh, void* vertices, UINT vertexSize, void* indices, UINT indexSize)
	{
		_resourceManager->UpdateMesh(mesh, vertices, vertexSize, indices, indexSize);
	}

	Material* GraphicsEngine::LoadMaterial(const DUOLCommon::tstring& objectID)
	{
		return _resourceManager->GetMaterial(objectID);
	}

	void GraphicsEngine::LoadMeshTable(const DUOLCommon::tstring& path)
	{
		auto jsonLoader = DUOLJson::JsonReader::GetInstance();

		auto meshTable = jsonLoader->LoadJson(path);

		const TCHAR* id = _T("ID");
		const TCHAR* resourcePath = _T("ResourcePath");

		for (auto& mesh : meshTable->GetArray())
		{
			if (mesh.HasMember(id) && mesh.HasMember(resourcePath))
			{
				_resourceManager->CreateMesh(mesh[id].GetString(), mesh[resourcePath].GetString());
			}
		}
	}
}

