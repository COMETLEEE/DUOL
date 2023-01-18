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

		UINT64 backbuffer = Hash::Hash64(_T("BackBuffer"));
		UINT64 depth = Hash::Hash64(_T("BackBufferDepth"));

		CreateSkyBox();
		CreateCascadeShadow(2048, 4);

		auto pipeline = _resourceManager->GetRenderingPipeline(_T("Lighting"));
		pipeline->ChangeTexture(_skyboxIrradianceTexture, 4);
		pipeline->ChangeTexture(_skyboxPreFilteredTexture, 5);
		pipeline->ChangeTexture(_skyboxBRDFLookUpTexture, 6);
		pipeline->ChangeTexture(_shadowMap, 7);

		_backbufferRenderPass = std::make_unique<DUOLGraphicsLibrary::RenderPass>();
		_backbufferRenderPass->_renderTargetViewRefs.push_back(_resourceManager->GetRenderTarget(backbuffer));
		_backbufferRenderPass->_depthStencilViewRef = _resourceManager->GetRenderTarget(depth);
	}

	GraphicsEngine::~GraphicsEngine()
	{
		DUOLGraphicsLibrary::Renderer::DeleteRenderer(_renderer);
	}

	void GraphicsEngine::LoadRenderingPipelineTables(const DUOLMath::Vector2& screenSize)
	{
		TableLoader::LoadRenderTargetTable(_resourceManager.get(), screenSize);
		TableLoader::LoadSampler(_resourceManager.get());
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
		_renderManager = std::make_unique<RenderManager>(_renderer, _context, _resourceManager->GetPerFrameBuffer(), _resourceManager->GetPerObjectBuffer());
		_resourceManager->AddBackbufferRenderTarget(_context->GetBackBufferRenderTarget());
	}

	void GraphicsEngine::CreateSkyBox()
	{
		//test code
		std::vector<StaticMeshVertex> vertices;
		std::vector<UINT32> indices;

		GeometryGenerator::CreateSphere(1, 24, 24, vertices, indices);

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
		skyboxTextureDesc._texturePath = "Asset/Texture/CloudedSunGlow4k.hdr";

		_skyboxTexture = _resourceManager->CreateTexture(Hash::Hash64(_T("SkyBoxTexture")), skyboxTextureDesc);

		//texture type이 2d면... 이녀석은 panorama texture이므로 cubemap으로 바꿔주어야 한다.
		if (_skyboxTexture->GetTextureDesc()._type == DUOLGraphicsLibrary::TextureType::TEXTURE2D)
		{
			_skyboxTexture = CreateCubeMapFromPanoramaImage(_skyboxTexture);
			_renderer->GenerateMips(_skyboxTexture);
		}
		_skyboxIrradianceTexture = BakeIBLIrradianceMap(_skyboxTexture, 512, 512);
		_skyboxPreFilteredTexture = BakeIBLPreFilteredMap(_skyboxTexture, 5, 512, 512);
		_skyboxBRDFLookUpTexture = BakeBRDFLookUpTable(512, 512);
	}

	void GraphicsEngine::CreateCascadeShadow(int textureSize, int sliceCount)
	{
		DUOLGraphicsLibrary::TextureDesc textureDesc;
		textureDesc._textureExtent = DUOLMath::Vector3{ (float)textureSize , (float)textureSize, 0 };
		textureDesc._arraySize = sliceCount;
		textureDesc._type = DUOLGraphicsLibrary::TextureType::TEXTURE2DARRAY;
		textureDesc._format = DUOLGraphicsLibrary::ResourceFormat::FORMAT_R24G8_TYPELESS;
		textureDesc._bindFlags = static_cast<long>(DUOLGraphicsLibrary::BindFlags::DEPTHSTENCIL) | static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE);

		_shadowMap = _resourceManager->CreateTexture(Hash::Hash64(_T("CascadeShadowMap")), textureDesc);

		DUOLGraphicsLibrary::RenderTargetDesc renderTargetDesc;

		renderTargetDesc._type = DUOLGraphicsLibrary::RenderTargetType::DepthStencil;
		renderTargetDesc._arraySize = sliceCount;
		renderTargetDesc._resolution = DUOLMath::Vector2{ (float)textureSize, (float)textureSize };
		renderTargetDesc._texture = _shadowMap;

		_shadowMapDepth = _resourceManager->CreateRenderTarget(_T("CascadeShadowMapDepth"), renderTargetDesc);

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

	void GraphicsEngine::RenderDebugObject(DUOLGraphicsEngine::RenderObject* object)
	{
		_renderManager->RenderDebug(object);
	}

	void GraphicsEngine::ClearRenderTargets()
	{
#if defined(_DEBUG) || defined(DEBUG)
		_renderer->BeginEvent(L"Clear RenderTargets");
#endif
		_resourceManager->ClearRenderTargets();
#if defined(_DEBUG) || defined(DEBUG)
		_renderer->EndEvent();
#endif

		// todo:: 꼭 뺴라
		static UINT64 debug = Hash::Hash64(_T("Debug"));

		_renderManager->ExecuteDebugRenderPass(_resourceManager->GetRenderingPipeline(debug));
	}

	void GraphicsEngine::ClearRenderTarget(DUOLGraphicsLibrary::RenderTarget& renderTarget)
	{
		_renderer->ClearRenderTarget(renderTarget);
	}

	void GraphicsEngine::ResizeRenderTarget(DUOLGraphicsLibrary::RenderTarget* renderTarget, const DUOLMath::Vector2& resolution)
	{
		_resourceManager->ResizeRenderTarget(renderTarget, resolution);
	}

	void GraphicsEngine::Execute(const ConstantBufferPerFrame& perFrameInfo)
	{
	}

	void GraphicsEngine::Execute(const std::vector<DUOLGraphicsEngine::RenderObject*>& renderObjects, const std::vector<RenderingPipelineLayout>& opaquePipelines, const std::vector<RenderingPipelineLayout>& transparencyPipelines, const ConstantBufferPerFrame& perFrameInfo)
	{
		_renderManager->SetPerFrameBuffer(_resourceManager->GetPerFrameBuffer(), perFrameInfo);
		_renderManager->RegisterRenderQueue(renderObjects);

		static UINT64 cascadeShadow = Hash::Hash64(_T("CascadeShadow"));
		static UINT64 shadowMesh = Hash::Hash64(_T("ShadowMeshVS"));
		static UINT64 shadowSkinned = Hash::Hash64(_T("ShadowSkinnedVS"));

		//todo :: 쉐도우 렌더타겟또한 정리해야함
		ClearRenderTarget(*_shadowMapDepth);;
		_renderManager->RenderCascadeShadow(_resourceManager->GetRenderingPipeline(cascadeShadow), _resourceManager->GetPipelineState(shadowMesh), _resourceManager->GetPipelineState(shadowSkinned), _shadowMapDepth, perFrameInfo);

		for (auto& pipeline : opaquePipelines)
		{
			_renderManager->ExecuteRenderingPipeline(pipeline._renderingPipeline, pipeline._perObjectBufferData, pipeline._dataSize);
		}

		//무조건적으로 스카이박스는 Opaque와 Transparency 사이에 그려줘야 합니다..... 근데 이거 어떻게해요?
		static UINT64 skybox = Hash::Hash64(_T("SkyBox"));

		_renderManager->RenderSkyBox(_resourceManager->GetRenderingPipeline(skybox), _skyboxPreFilteredTexture, _skyboxVertex, _skyboxIndex, perFrameInfo._camera);

		for (auto& pipeline : transparencyPipelines)
		{
			_renderManager->ExecuteRenderingPipeline(pipeline._renderingPipeline, pipeline._perObjectBufferData, pipeline._dataSize);
		}

		//todo:: 이것도 꼭 뺴라
		static UINT64 debugRT = Hash::Hash64(_T("DebugRT"));
		_renderManager->ExecuteDebugRenderTargetPass(_resourceManager->GetRenderingPipeline(debugRT));
	}

	void GraphicsEngine::Execute(const std::vector<DUOLGraphicsEngine::RenderObject*>& renderObjects,
		const std::vector<RenderingPipelineLayout>& opaquePipelines, RenderingPipeline* skyBoxPipeline,
		const std::vector<RenderingPipelineLayout>& transparencyPipelines, const ConstantBufferPerFrame& perFrameInfo)
	{
		_renderManager->SetPerFrameBuffer(_resourceManager->GetPerFrameBuffer(), perFrameInfo);
		_renderManager->RegisterRenderQueue(renderObjects);

		for (auto& pipeline : opaquePipelines)
		{
			_renderManager->ExecuteRenderingPipeline(pipeline._renderingPipeline, pipeline._perObjectBufferData, pipeline._dataSize);
		}

		// 무조건적으로 스카이박스는 Opaque와 Transparency 사이에 그려줘야 합니다..... 근데 이거 어떻게해요?
		_renderManager->RenderSkyBox(skyBoxPipeline, _skyboxTexture, _skyboxVertex, _skyboxIndex, perFrameInfo._camera);

		for (auto& pipeline : transparencyPipelines)
		{
			_renderManager->ExecuteRenderingPipeline(pipeline._renderingPipeline, pipeline._perObjectBufferData, pipeline._dataSize);
		}

		// todo:: 이것도 꼭 뺴라. 일단 씬 뷰를 그리는 것으로 가정해서 그립니다.
		static UINT64 debugRT = Hash::Hash64(_T("DebugRT"));
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

	void GraphicsEngine::OnResize(const DUOLMath::Vector2& resolution)
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

	void GraphicsEngine::CopyTexture(DUOLGraphicsLibrary::Texture* destTexture,
		DUOLGraphicsLibrary::Texture* srcTexture)
	{
		_renderManager->CopyTexture(destTexture, srcTexture);
	}

	uint64_t GraphicsEngine::FastPicking(const DUOLMath::Vector2& pixel)
	{
		DUOLGraphicsLibrary::Texture* objectID = LoadTexture(TEXT("ObjectID"));

		// 만약, 매개변수로 들어온 Pixel의 위치가 텍스처의 크기를 넘어갔다면
		if ((pixel.x > objectID->GetTextureDesc()._textureExtent.x) || (pixel.y > objectID->GetTextureDesc()._textureExtent.y))
			return uint64_t {};

		DUOLGraphicsLibrary::TextureLocation srcLocation;

		srcLocation._offset.x = pixel.x;

		srcLocation._offset.y = pixel.y;

		srcLocation._mipLevel = objectID->GetTextureDesc()._mipLevels;

		srcLocation._arrayLayer = objectID->GetTextureDesc()._arraySize;

		char data[16];

		// 실패하면 false, 
		_renderer->ReadTexture(objectID, srcLocation, data, 16);

		uint64_t id;

		memcpy(&id, data, sizeof(uint64_t));

		return id;
	}

	MeshBase* GraphicsEngine::LoadMesh(const DUOLCommon::tstring& objectID)
	{
		return _resourceManager->GetMesh(objectID);
	}

	Model* GraphicsEngine::CreateModelFromFBX(const DUOLCommon::tstring& objectID, std::pair<std::vector<uint64>, std::vector<uint64>>& modeldatas)
	{
		return _resourceManager->CreateModelFromFBX(objectID, modeldatas);
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

	DUOLGraphicsLibrary::Texture* GraphicsEngine::BakeIBLIrradianceMap(DUOLGraphicsLibrary::Texture* cubeMap, float width, float height)
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
			auto sampler = _resourceManager->GetSampler(Hash::Hash64(_T("SamLinear")));

			_renderManager->CreateCubeMapFromPanoramaImage(originTexture, cubeIrradianceRenderTarget, pipeline, depth, _resourceManager->GetPerObjectBuffer(), sampler);

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
			auto sampler = _resourceManager->GetSampler(Hash::Hash64(_T("SamLinear")));

			_renderManager->CreatePreFilteredMapFromCubeImage(originTexture, radianceRenderTarget, pipeline, depth, _resourceManager->GetPerObjectBuffer(), sampler, mipSize, width, height);

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
			textureDesc._textureExtent = DUOLMath::Vector3{ 1024, 1024, 0 };
			textureDesc._arraySize = 6;
			textureDesc._type = DUOLGraphicsLibrary::TextureType::TEXTURECUBE;
			textureDesc._format = textureDesc._format;
			textureDesc._mipLevels = 5;
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
			auto sampler = _resourceManager->GetSampler(Hash::Hash64(_T("SamLinear")));

			_renderManager->CreateCubeMapFromPanoramaImage(panorama, skyBoxRenderTargets, pipeline, depth, _resourceManager->GetPerObjectBuffer(), sampler);

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

	void GraphicsEngine::SetDataName(std::vector<std::pair<uint64, DUOLCommon::tstring>>& materialname, std::vector<std::pair<uint64, DUOLCommon::tstring>>& animationname)
	{
		_resourceManager->SetDataName(materialname, animationname);
	}

	Material* GraphicsEngine::LoadMaterial(const DUOLCommon::tstring& objectID)
	{
		return _resourceManager->GetMaterial(objectID);
	}

	AnimationClip* GraphicsEngine::LoadAnimationClip(const DUOLCommon::tstring& objectID)
	{
		return _resourceManager->GetAnimationClip(objectID);
	}

	DUOLGraphicsLibrary::Texture* GraphicsEngine::LoadTexture(const DUOLCommon::tstring& objectID)
	{
		return _resourceManager->GetTexture(objectID);
	}

	RenderingPipeline* GraphicsEngine::LoadRenderingPipeline(const DUOLCommon::tstring& objectID)
	{
		return _resourceManager->GetRenderingPipeline(objectID);
	}

	/// 이거 쓰는곳 없어서 주석합니다.
	//void GraphicsEngine::LoadMeshTable(const DUOLCommon::tstring& path)
	//{
	//	auto jsonLoader = DUOLJson::JsonReader::GetInstance();

	//	auto meshTable = jsonLoader->LoadJson(path);

	//	const TCHAR* id = _T("ID");
	//	const TCHAR* resourcePath = _T("ResourcePath");

	//	for (auto& mesh : meshTable->GetArray())
	//	{
	//		if (mesh.HasMember(id) && mesh.HasMember(resourcePath))
	//		{
	//			DUOLCommon::tstring meshid = mesh[id].GetString();
	//			DUOLCommon::tstring meshPath = mesh[resourcePath].GetString();

	//			_resourceManager->CreateModelFromFBX(meshid);
	//		}
	//	}
	//}
}

