#include "DUOLGraphicsLibrary/Renderer/Renderer.h"
#include "DUOLGraphicsEngine/GraphicsEngine/GraphicsEngine.h"
#include "DUOLGraphicsEngine/ResourceManager/ResourceManager.h"
#include "DUOLGraphicsEngine/RenderManager/RenderManager.h"
#include "DUOLGraphicsEngine/Util/Hash/Hash.h"
#include "DUOLGraphicsEngine/RenderManager/RenderingPipeline.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/RenderConstantBuffer.h"
#include "DUOLJson/JsonReader.h"

#include <tchar.h>

#include "DUOLGraphicsEngine/GeometryGenerator.h"
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
		Initialize();

		_renderManager->OnResize(renderContextDesc._screenDesc._screenSize);
		LoadRenderingPipelineTables(renderContextDesc._screenDesc._screenSize);

		_resourceManager->CreateDebugMaterial();

		_renderManager->SetStreamOutShader(_resourceManager->GetPipelineState(Hash::Hash64(_T("StreamOut"))));

		UINT64 merge = Hash::Hash64(_T("BackBuffer"));
		UINT64 depth = Hash::Hash64(_T("MergeDepth"));

		CreateSkyBox();

		_backbufferRenderPass = std::make_unique<DUOLGraphicsLibrary::RenderPass>();
		_backbufferRenderPass->_renderTargetViewRefs.push_back(_resourceManager->GetRenderTarget(merge));
		_backbufferRenderPass->_depthStencilViewRef = _resourceManager->GetRenderTarget(depth);
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

	void GraphicsEngine::ReadFromStaticMesh(MeshBase* const mesh, std::vector<DUOLMath::Vector3>& vertexInfo,
		std::vector<UINT32>& indexInfo)
	{
		auto staticMesh = static_cast<Mesh*>(mesh);

#pragma region GetVertexInfo 
		{
			auto vertexBufferSize = mesh->_vertexBuffer->GetBufferDesc()._size;
			auto vertexStructSize = mesh->_vertexBuffer->GetBufferDesc()._stride;

			int vertexCnt = vertexBufferSize / vertexStructSize;
			vertexInfo.resize(vertexCnt);

			for (int vertexIdx = 0; vertexIdx < vertexCnt; vertexIdx++)
			{
				memcpy(&vertexInfo[vertexIdx], &staticMesh->_vertices[vertexIdx], sizeof(DUOLMath::Vector3));
			}

		}

		int indexCnt = 0;

		for (auto subMesh : mesh->_subMeshs)
		{
			indexCnt += subMesh._indexBuffer->GetBufferDesc()._size / sizeof(UINT32);
		}
#pragma endregion
#pragma region GetIndexInfo 

		indexInfo.resize(indexCnt);
		int indexPivot = 0;

		for (auto subMesh : mesh->_subMeshs)
		{
			auto indexBufferSize = subMesh._indexBuffer->GetBufferDesc()._size;
			const auto indexBufferCnt = subMesh._indexBuffer->GetBufferDesc()._size / sizeof(UINT32);

			for (int indexIdx = 0; indexIdx < indexBufferCnt; indexIdx++)
			{
				memcpy(&indexInfo[indexPivot], &subMesh._indices[indexIdx], sizeof(UINT32));
				indexPivot++;
			}
		}
#pragma endregion 
	}

	void GraphicsEngine::ReadFromSkinnedMesh(MeshBase* mesh, std::vector<DUOLMath::Vector3>& vertexInfo,
		std::vector<UINT32>& indexInfo)
	{
		auto skinnedMesh = static_cast<SkinnedMesh*>(mesh);

#pragma region GetVertexInfo 
		{
			auto vertexBufferSize = mesh->_vertexBuffer->GetBufferDesc()._size;
			auto vertexStructSize = mesh->_vertexBuffer->GetBufferDesc()._stride;

			int vertexCnt = vertexBufferSize / vertexStructSize;
			vertexInfo.resize(vertexCnt);

			for (int vertexIdx = 0; vertexIdx < vertexCnt; vertexIdx++)
			{
				memcpy(&vertexInfo[vertexIdx], &skinnedMesh->_vertices[vertexIdx], sizeof(DUOLMath::Vector3));
			}
		}

		int indexCnt = 0;

		for (auto subMesh : mesh->_subMeshs)
		{
			indexCnt += subMesh._indexBuffer->GetBufferDesc()._size / sizeof(UINT32);
		}
#pragma endregion
#pragma region GetIndexInfo 

		indexInfo.resize(indexCnt);
		int indexPivot = 0;

		for (auto subMesh : mesh->_subMeshs)
		{
			auto indexBufferSize = subMesh._indexBuffer->GetBufferDesc()._size;
			const auto indexBufferCnt = subMesh._indexBuffer->GetBufferDesc()._size / sizeof(UINT32);

			for (int indexIdx = 0; indexIdx < indexBufferCnt; indexIdx++)
			{
				memcpy(&indexInfo[indexPivot], &subMesh._indices[indexIdx], sizeof(UINT32));
				indexPivot++;
			}
		}
#pragma endregion 
	}

	void GraphicsEngine::Initialize()
	{
		_resourceManager = std::make_unique<ResourceManager>(_renderer);
		_renderManager = std::make_unique<RenderManager>(_renderer, _context);
		_resourceManager->AddBackbufferRenderTarget(_context->GetBackBufferRenderTarget());
	}

	void GraphicsEngine::CreateSkyBox()
	{
		//test code
		std::vector<StaticMeshVertex> vertices;
		std::vector<UINT32> indices;

		GeometryGenerator::CreateSphere(100, 24, 24, vertices, indices);

		DUOLGraphicsLibrary::BufferDesc vetexBufferDesc;

		vetexBufferDesc._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::VERTEXBUFFER);
		vetexBufferDesc._usage = DUOLGraphicsLibrary::ResourceUsage::USAGE_DYNAMIC;
		vetexBufferDesc._stride = sizeof(StaticMeshVertex);
		vetexBufferDesc._size = vetexBufferDesc._stride * vertices.size();
		vetexBufferDesc._cpuAccessFlags = static_cast<long>(DUOLGraphicsLibrary::CPUAccessFlags::WRITE);

		DUOLGraphicsLibrary::BufferDesc indexBufferDesc;

		indexBufferDesc._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::INDEXBUFFER);
		indexBufferDesc._usage = DUOLGraphicsLibrary::ResourceUsage::USAGE_DYNAMIC;
		indexBufferDesc._stride = sizeof(unsigned int);
		indexBufferDesc._size = indexBufferDesc._stride * indices.size();
		indexBufferDesc._format = DUOLGraphicsLibrary::ResourceFormat::FORMAT_R32_UINT;
		indexBufferDesc._cpuAccessFlags = static_cast<long>(DUOLGraphicsLibrary::CPUAccessFlags::WRITE);

		_skyboxVertex = _renderer->CreateBuffer(Hash::Hash64(_T("SkyBoxVertex")), vetexBufferDesc, vertices.data());
		_skyboxIndex = _renderer->CreateBuffer(Hash::Hash64(_T("SkyBoxIndex")), indexBufferDesc, indices.data());

		DUOLGraphicsLibrary::TextureDesc skyboxTextureDesc;
		skyboxTextureDesc._texturePath = "Asset/Texture/TropicalRuins_8k.jpg";

		_skyboxTexture = _resourceManager->CreateTexture(Hash::Hash64(_T("SkyBoxTexture")), skyboxTextureDesc);

		//texture type이 2d면... 이녀석은 panorama texture이므로 cubemap으로 바꿔주어야 한다.
		if (_skyboxTexture->GetTextureDesc()._type == DUOLGraphicsLibrary::TextureType::TEXTURE2D)
		{
			_skyboxTexture = CreateCubeMapFromPanoramaImage(_skyboxTexture);
		}
		_skyboxIrradianceTexture = BakeIBLIrradianceMap(_skyboxTexture);
		_skyboxPreFilteredTexture = BakeIBLPreFilteredMap(_skyboxTexture, 5, 128, 128);
		_skyboxBRDFLookUpTexture = BakeBRDFLookUpTable(512, 512);

		auto pipeline = _resourceManager->GetRenderingPipeline(_T("Lighting"));
		pipeline->ChangeTexture(_skyboxIrradianceTexture, 4);
		pipeline->ChangeTexture(_skyboxPreFilteredTexture, 5);
		pipeline->ChangeTexture(_skyboxBRDFLookUpTexture, 6);
	}

	DUOLGraphicsEngine::ModuleInfo GraphicsEngine::GetModuleInfo()
	{
		auto ret = _renderer->GetModuleInfo();

		DUOLGraphicsEngine::ModuleInfo moduleInfo;
		moduleInfo._moduleType = RendererModuleType::DIRECTX11;
		moduleInfo._device = ret._device;
		moduleInfo._deviceContext = ret._deviceContext;

		return moduleInfo;
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
#if defined(_DEBUG) || defined(DEBUG)
		_renderer->BeginEvent(L"Clear RenderTargets");
#endif
		_resourceManager->ClearRenderTargets();
#if defined(_DEBUG) || defined(DEBUG)
		_renderer->EndEvent();
#endif
		_renderManager->SetPerFrameBuffer(_resourceManager->GetPerFrameBuffer(), perFrameInfo);

		static UINT64 debug = Hash::Hash64(_T("Debug"));
		static UINT64 debugRT = Hash::Hash64(_T("DebugRT"));

		_renderManager->ExecuteDebugRenderPass(_resourceManager->GetRenderingPipeline(debug));

		static UINT64 id = Hash::Hash64(_T("Default"));
		static UINT64 deferred = Hash::Hash64(_T("Lighting"));
		static UINT64 merge = Hash::Hash64(_T("Merge"));
		static UINT64 oit0 = Hash::Hash64(_T("OIT0"));
		static UINT64 oit1 = Hash::Hash64(_T("OIT1"));
		static UINT64 oit2 = Hash::Hash64(_T("OIT2"));
		static UINT64 oit3 = Hash::Hash64(_T("OIT3"));
		static UINT64 oit4 = Hash::Hash64(_T("OIT4"));
		static UINT64 oit5 = Hash::Hash64(_T("OIT5"));
		static UINT64 oitMerge0 = Hash::Hash64(_T("OITMerge0"));
		static UINT64 oitMerge1 = Hash::Hash64(_T("OITMerge1"));
		static UINT64 oitMerge2 = Hash::Hash64(_T("OITMerge2"));
		static UINT64 oitMerge3 = Hash::Hash64(_T("OITMerge3"));
		static UINT64 oitMerge4 = Hash::Hash64(_T("OITMerge4"));
		static UINT64 oitMerge5 = Hash::Hash64(_T("OITMerge5"));
		static UINT64 skybox = Hash::Hash64(_T("SkyBox"));

		_renderManager->ExecuteRenderingPipeline(_resourceManager->GetRenderingPipeline(id));
		_renderManager->ExecuteRenderingPipeline(_resourceManager->GetRenderingPipeline(deferred));
		_renderManager->RenderSkyBox(_resourceManager->GetRenderingPipeline(skybox), _skyboxTexture, _skyboxVertex, _skyboxIndex);
		_renderManager->ExecuteRenderingPipeline(_resourceManager->GetRenderingPipeline(merge));

		_renderManager->ExecuteRenderingPipeline(_resourceManager->GetRenderingPipeline(oit0));
		_renderManager->ExecuteRenderingPipeline(_resourceManager->GetRenderingPipeline(oit1));
		_renderManager->ExecuteRenderingPipeline(_resourceManager->GetRenderingPipeline(oit2));
		_renderManager->ExecuteRenderingPipeline(_resourceManager->GetRenderingPipeline(oit3));
		_renderManager->ExecuteRenderingPipeline(_resourceManager->GetRenderingPipeline(oit4));
		_renderManager->ExecuteRenderingPipeline(_resourceManager->GetRenderingPipeline(oit5));

		_renderManager->ExecuteRenderingPipeline(_resourceManager->GetRenderingPipeline(oitMerge0));
		_renderManager->ExecuteRenderingPipeline(_resourceManager->GetRenderingPipeline(oitMerge1));
		_renderManager->ExecuteRenderingPipeline(_resourceManager->GetRenderingPipeline(oitMerge2));
		_renderManager->ExecuteRenderingPipeline(_resourceManager->GetRenderingPipeline(oitMerge3));
		_renderManager->ExecuteRenderingPipeline(_resourceManager->GetRenderingPipeline(oitMerge4));
		_renderManager->ExecuteRenderingPipeline(_resourceManager->GetRenderingPipeline(oitMerge5));
		_renderManager->ExecuteRenderingPipeline(_resourceManager->GetRenderingPipeline(oitMerge5));

		_renderManager->ExecuteDebugRenderTargetPass(_resourceManager->GetRenderingPipeline(debugRT));
	}

	void GraphicsEngine::PrePresent()
	{
		_renderManager->BindBackBuffer(_backbufferRenderPass.get());
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

	MeshBase* GraphicsEngine::LoadMesh(const DUOLCommon::tstring& objectID)
	{
		return _resourceManager->GetMesh(objectID);
	}

	Model* GraphicsEngine::CreateModelFromFBX(const DUOLCommon::tstring& objectID, const DUOLCommon::tstring& path)
	{
		return _resourceManager->CreateModelFromFBX(objectID, path);
	}

	MeshBase* GraphicsEngine::CreateMesh(const DUOLCommon::tstring& objectID, void* vertices, UINT vertexSize, UINT vertexStructureSize, void* indices,
		UINT indexSize)
	{
		return _resourceManager->CreateMesh(objectID, vertices, vertexSize, vertexStructureSize, indices, indexSize);
	}

	Material* GraphicsEngine::CreateMaterial(const DUOLCommon::tstring& objectID,
		DUOLGraphicsEngine::MaterialDesc& materialDesc)
	{
		return _resourceManager->CreateMaterial(objectID, materialDesc);
	}

	MeshBase* GraphicsEngine::CreateParticle(const DUOLCommon::tstring& objectID, int maxParticle, int emitterSize)
	{
		return _resourceManager->CreateParticleBuffer(objectID, maxParticle, emitterSize);
	}

	DUOLGraphicsLibrary::Texture* GraphicsEngine::BakeIBLIrradianceMap(DUOLGraphicsLibrary::Texture* cubeMap)
	{
		auto originTexture = cubeMap;

		auto& originTextureDesc = originTexture->GetTextureDesc();
		DUOLGraphicsLibrary::TextureDesc textureDesc;

		if (originTextureDesc._type == DUOLGraphicsLibrary::TextureType::TEXTURECUBE)
		{
			textureDesc._textureExtent = DUOLMath::Vector3{ 512, 512, 0 };
			textureDesc._arraySize = 6;
			textureDesc._type = DUOLGraphicsLibrary::TextureType::TEXTURECUBE;
			textureDesc._format = textureDesc._format;
			textureDesc._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::RENDERTARGET) | static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE);

			DUOLCommon::tstring textureID = _T("SkyBoxIrradianceMap");

			auto irradianceMap = _resourceManager->CreateTexture(textureID, textureDesc);

			DUOLGraphicsLibrary::RenderTargetDesc renderTargetDesc;

			renderTargetDesc._type = DUOLGraphicsLibrary::RenderTargetType::Color;
			renderTargetDesc._mipLevel = 0;
			renderTargetDesc._texture = irradianceMap;

			DUOLGraphicsLibrary::RenderTarget* cubeIrradianceRenderTarget[6];

			for (int i = 0; i < 6; ++i)
			{
				renderTargetDesc._arrayLayer = i;
				DUOLCommon::tstring renderTargetID = textureID + DUOLCommon::StringHelper::ToTString(i);
				cubeIrradianceRenderTarget[i] = _resourceManager->CreateRenderTarget(renderTargetID, renderTargetDesc);
			}

			auto pipeline = _resourceManager->GetPipelineState(Hash::Hash64(_T("CubeMapToIrradianceMap")));
			auto depth = _resourceManager->GetRenderTarget(Hash::Hash64(_T("DefaultDepth")));

			_renderManager->CreateCubeMapFromPanoramaImage(originTexture, cubeIrradianceRenderTarget, pipeline, depth, _resourceManager->GetPerObjectBuffer());

			for (int i = 0; i < 6; ++i)
			{
				DUOLCommon::tstring renderTargetID = textureID + DUOLCommon::StringHelper::ToTString(i);
				_resourceManager->DeleteRenderTarget(renderTargetID);
			}

			return irradianceMap;
		}

		return nullptr;
	}

	DUOLGraphicsLibrary::Texture* GraphicsEngine::BakeIBLPreFilteredMap(DUOLGraphicsLibrary::Texture* cubeMap, int mipSize, float width, float height)
	{
		auto originTexture = cubeMap;

		auto& originTextureDesc = originTexture->GetTextureDesc();
		DUOLGraphicsLibrary::TextureDesc textureDesc;

		if (originTextureDesc._type == DUOLGraphicsLibrary::TextureType::TEXTURECUBE)
		{
			textureDesc._textureExtent = DUOLMath::Vector3{ width, height, 0 };
			textureDesc._arraySize = 6;
			textureDesc._type = DUOLGraphicsLibrary::TextureType::TEXTURECUBE;
			textureDesc._format = textureDesc._format;
			textureDesc._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::RENDERTARGET) | static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE);
			textureDesc._mipLevels = mipSize;

			DUOLCommon::tstring textureID = _T("SkyBoxRadianceMap");

			auto radianceMap = _resourceManager->CreateTexture(textureID, textureDesc);

			DUOLGraphicsLibrary::RenderTargetDesc renderTargetDesc;

			renderTargetDesc._type = DUOLGraphicsLibrary::RenderTargetType::Color;
			renderTargetDesc._texture = radianceMap;

			DUOLGraphicsLibrary::RenderTarget** radianceRenderTarget = new DUOLGraphicsLibrary::RenderTarget * [6 * mipSize];

			for (int j = 0; j < mipSize; ++j)
			{
				for (int i = 0; i < 6; ++i)
				{
					renderTargetDesc._mipLevel = j;
					renderTargetDesc._arrayLayer = i;
					int arrayIdx = 6 * j + i;
					DUOLCommon::tstring renderTargetID = textureID + DUOLCommon::StringHelper::ToTString(arrayIdx);
					radianceRenderTarget[arrayIdx] = _resourceManager->CreateRenderTarget(renderTargetID, renderTargetDesc);
				}
			}

			auto pipeline = _resourceManager->GetPipelineState(Hash::Hash64(_T("CubeMapToRadianceMap")));
			auto depth = _resourceManager->GetRenderTarget(Hash::Hash64(_T("DefaultDepth")));

			_renderManager->CreatePreFilteredMapFromCubeImage(originTexture, radianceRenderTarget, pipeline, depth, _resourceManager->GetPerObjectBuffer(), mipSize, width, height);

			for (int j = 0; j < mipSize; ++j)
			{
				for (int i = 0; i < 6; ++i)
				{
					int arrayIdx = 6 * j + i;
					DUOLCommon::tstring renderTargetID = textureID + DUOLCommon::StringHelper::ToTString(arrayIdx);
					_resourceManager->DeleteRenderTarget(renderTargetID);
				}
			}

			delete[] radianceRenderTarget;

			return radianceMap;
		}

		return nullptr;
	}

	DUOLGraphicsLibrary::Texture* GraphicsEngine::BakeBRDFLookUpTable(float width, float height)
	{
		DUOLGraphicsLibrary::TextureDesc textureDesc;

		textureDesc._textureExtent = DUOLMath::Vector3{ width, height, 0 };
		textureDesc._arraySize = 6;
		textureDesc._type = DUOLGraphicsLibrary::TextureType::TEXTURE2D;
		textureDesc._format = textureDesc._format;
		textureDesc._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::RENDERTARGET) | static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE);

		DUOLCommon::tstring textureID = _T("SkyBoxBRDFLookUpTable");

		auto _skyboxBRDFLookUpTexture = _resourceManager->CreateTexture(textureID, textureDesc);

		DUOLGraphicsLibrary::RenderTargetDesc renderTargetDesc;

		renderTargetDesc._type = DUOLGraphicsLibrary::RenderTargetType::Color;
		renderTargetDesc._mipLevel = 0;
		renderTargetDesc._texture = _skyboxBRDFLookUpTexture;

		DUOLGraphicsLibrary::RenderTarget* BRDFRenderTarget;
		BRDFRenderTarget = _resourceManager->CreateRenderTarget(textureID, renderTargetDesc);

		auto pipeline = _resourceManager->GetPipelineState(Hash::Hash64(_T("BRDFLookUpTable")));
		auto depth = _resourceManager->GetRenderTarget(Hash::Hash64(_T("DefaultDepth")));

		_renderManager->CreateBRDFLookUpTable(BRDFRenderTarget, pipeline, depth, _resourceManager->GetPerObjectBuffer(), width, height);
		_resourceManager->DeleteRenderTarget(textureID);

		return _skyboxBRDFLookUpTexture;
	}

	DUOLGraphicsLibrary::Texture* GraphicsEngine::CreateCubeMapFromPanoramaImage(DUOLGraphicsLibrary::Texture* panorama)
	{
		auto originTexture = panorama;

		auto& originTextureDesc = originTexture->GetTextureDesc();
		DUOLGraphicsLibrary::TextureDesc textureDesc;

		if (originTextureDesc._type == DUOLGraphicsLibrary::TextureType::TEXTURE2D)
		{
			textureDesc._textureExtent = DUOLMath::Vector3{ 512, 512, 0 };
			textureDesc._arraySize = 6;
			textureDesc._type = DUOLGraphicsLibrary::TextureType::TEXTURECUBE;
			textureDesc._format = textureDesc._format;
			textureDesc._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::RENDERTARGET) | static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE);

			DUOLCommon::tstring textureID = _T("SkyboxCubeMap");

			auto skyBox = _resourceManager->CreateTexture(textureID, textureDesc);

			DUOLGraphicsLibrary::RenderTargetDesc renderTargetDesc;

			renderTargetDesc._type = DUOLGraphicsLibrary::RenderTargetType::Color;
			renderTargetDesc._mipLevel = 0;
			renderTargetDesc._texture = skyBox;

			DUOLGraphicsLibrary::RenderTarget* skyBoxRenderTargets[6];

			for (int i = 0; i < 6; ++i)
			{
				renderTargetDesc._arrayLayer = i;
				DUOLCommon::tstring renderTargetID = textureID + DUOLCommon::StringHelper::ToTString(i);
				skyBoxRenderTargets[i] = _resourceManager->CreateRenderTarget(renderTargetID, renderTargetDesc);
			}

			auto pipeline = _resourceManager->GetPipelineState(Hash::Hash64(_T("PanoramaToCubeMap")));
			auto depth = _resourceManager->GetRenderTarget(Hash::Hash64(_T("DefaultDepth")));

			_renderManager->CreateCubeMapFromPanoramaImage(panorama, skyBoxRenderTargets, pipeline, depth, _resourceManager->GetPerObjectBuffer());

			for (int i = 0; i < 6; ++i)
			{
				DUOLCommon::tstring renderTargetID = textureID + DUOLCommon::StringHelper::ToTString(i);
				_resourceManager->DeleteRenderTarget(renderTargetID);
			}

			return skyBox;
		}

		return nullptr;
	}

	Model* GraphicsEngine::LoadModel(const DUOLCommon::tstring& objectID)
	{
		return  _resourceManager->GetModel(objectID);
	}

	bool GraphicsEngine::ReadMeshInfo(const DUOLCommon::tstring& objectID, std::vector<DUOLMath::Vector3>& vertexInfo,
		std::vector<UINT32>& indexInfo)
	{
		auto mesh = _resourceManager->GetMesh(objectID);
		if (mesh == nullptr)
		{
			return false;
		}

		switch (mesh->GetMeshType())
		{
		case MeshBase::MeshType::Mesh:
		{
			ReadFromStaticMesh(mesh, vertexInfo, indexInfo);
			break;
		}
		case MeshBase::MeshType::SkinnedMesh:
		{
			ReadFromSkinnedMesh(mesh, vertexInfo, indexInfo);
			break;
		}
		case MeshBase::MeshType::Particle:
		default:
			return false;
		}

		return true;
	}

	bool GraphicsEngine::ReadMeshInfo(MeshBase* mesh, std::vector<DUOLMath::Vector3>& vertexInfo,
		std::vector<UINT32>& indexInfo)
	{
		if (mesh == nullptr)
		{
			return false;
		}

		switch (mesh->GetMeshType())
		{
		case MeshBase::MeshType::Mesh:
		{
			ReadFromStaticMesh(mesh, vertexInfo, indexInfo);
			break;
		}
		case MeshBase::MeshType::SkinnedMesh:
		{
			ReadFromSkinnedMesh(mesh, vertexInfo, indexInfo);
			break;
		}
		case MeshBase::MeshType::Particle:
		default:
			return false;
		}

		return true;
	}

	void GraphicsEngine::UpdateMesh(MeshBase* mesh, void* vertices, UINT vertexSize, void* indices, UINT indexSize)
	{
		_resourceManager->UpdateMesh(mesh, vertices, vertexSize, indices, indexSize);
	}

	Material* GraphicsEngine::LoadMaterial(const DUOLCommon::tstring& objectID)
	{
		return _resourceManager->GetMaterial(objectID);
	}

	AnimationClip* GraphicsEngine::LoadAnimationClip(const DUOLCommon::tstring& objectID)
	{
		return _resourceManager->GetAnimationClip(objectID);
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
				DUOLCommon::tstring meshid = mesh[id].GetString();
				DUOLCommon::tstring meshPath = mesh[resourcePath].GetString();

				_resourceManager->CreateModelFromFBX(meshid, meshPath);
			}
		}
	}
}

