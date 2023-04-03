#include "DUOLGraphicsEngine/GraphicsEngine/GraphicsEngine.h"

#include "DUOLGraphicsEngine/Util/Hash/Hash.h"
#include "DUOLJson/JsonReader.h"
#include "DUOLGraphicsLibrary/Renderer/Renderer.h"
#include "DUOLGraphicsEngine/ResourceManager/ResourceManager.h"
#include "DUOLGraphicsEngine/RenderManager/RenderManager.h"
#include "DUOLGraphicsEngine/RenderManager/RenderingPipeline/RenderingPipeline.h"

#include "DUOLGraphicsEngine/GeometryGenerator.h"
#include "DUOLGraphicsEngine/RenderManager/CullingHelper.h"
#include "DUOLGraphicsEngine/RenderManager/Renderer/OrderIndependentTransparencyRenderer.h"
#include "DUOLGraphicsEngine/ResourceManager/LightManager.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/CascadeShadow.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/SkyBox.h"
#include "DUOLGraphicsEngine/ResourceManager/LightManager.h"
#include "DUOLGraphicsEngine/TableLoader/TableLoader.h"

#include "DUOLCommon/StringHelper.h"
#include "DUOLCommon/Log/LogHelper.h"

namespace DUOLGraphicsEngine
{
	GraphicsEngine::GraphicsEngine(const GraphicsEngineDesc& engineDesc)
	{
		DUOLCommon::LogHelper::Initialize();

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
		LoadRenderingPipelineTables(renderContextDesc._screenDesc._screenSize);
		_resourceManager->CreateDebugMaterial();

		_lightManager = std::make_unique<LightManager>(_resourceManager.get());

		UINT64 backbuffer = Hash::Hash64(_T("BackBuffer"));
		UINT64 depth = Hash::Hash64(_T("BackBufferDepth"));

		CreateSkyBox();
		CreateOcclusionCulling();
		CreateCascadeShadow(2048, 4);
		//oit Renderer 등록
		_oitRenderer = std::make_unique<OrderIndependentTransparencyRenderer>(_resourceManager.get(), engineDesc._screenSize, 16);
		_renderManager->SetOITRenderer(_oitRenderer.get());

		auto pipeline = _resourceManager->GetRenderingPipeline(_T("Lighting"));
		auto textureLayout = pipeline->GetTextureResourceViewLayout();

		pipeline->SetTexture(_skyBox->GetSkyboxIrradianceTexture(), 4);
		pipeline->SetTexture(_skyBox->GetSkyboxPreFilteredTexture(), 5);
		pipeline->SetTexture(_skyBox->GetSkyboxBRDFLookUpTexture(), 6);
		pipeline->SetTexture(_cascadeShadow->GetShadowMap(), 7);
		pipeline->SetTexture(_lightManager->GetSpotShadowMaps(), 8);
		pipeline->SetTexture(_lightManager->GetPointLightShadowMaps(), 9);

		_backbufferRenderPass = std::make_unique<DUOLGraphicsLibrary::RenderPass>();
		_backbufferRenderPass->_renderTargetViewRefs.push_back(_resourceManager->GetRenderTarget(backbuffer));
		_backbufferRenderPass->_depthStencilViewRef = _resourceManager->GetRenderTarget(depth);

		_drawCanvasOnGameViewPipeline = _resourceManager->GetRenderingPipeline(_T("DrawCanvasOnGameView"));
		_drawGameViewOnBakcBufferPipeline = _resourceManager->GetRenderingPipeline(_T("DrawBackBuffer"));
	}

	GraphicsEngine::~GraphicsEngine()
	{
		DUOLCommon::LogHelper::UnInitialize();

		DUOLGraphicsLibrary::Renderer::DeleteRenderer(_renderer);
	}

	void GraphicsEngine::LoadRenderingPipelineTables(const DUOLMath::Vector2& screenSize)
	{

		DUOL_ENGINE_INFO(DUOL_CONSOLE, "   Start| Load GraphicResource");
		TableLoader::LoadRenderTargetTable(_resourceManager.get(), screenSize);
		DUOL_ENGINE_INFO(DUOL_CONSOLE, "Complete| Load RenderTargets");
		TableLoader::LoadSampler(_resourceManager.get());
		DUOL_ENGINE_INFO(DUOL_CONSOLE, "Complete| Load Samplers");
		TableLoader::LoadShaderTable(_resourceManager.get());
		DUOL_ENGINE_INFO(DUOL_CONSOLE, "Complete| Load Shaders");
		TableLoader::LoadPipelineStateTable(_resourceManager.get());
		DUOL_ENGINE_INFO(DUOL_CONSOLE, "Complete| Load PipelineStates");
		TableLoader::LoadRenderingPipelineTable(_resourceManager.get());
		DUOL_ENGINE_INFO(DUOL_CONSOLE, "Complete| Load RenderingPipelines");
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
		_renderManager = std::make_unique<RenderManager>(_renderer, _context, _resourceManager->GetPerFrameBuffer(), _resourceManager->GetPerCameraBuffer(), _resourceManager->GetPerObjectBuffer());
		_fontEngine = _renderer->GetFontEngine();
		_resourceManager->AddBackbufferRenderTarget(_context->GetBackBufferRenderTarget());
	}

	void GraphicsEngine::CreateSkyBox()
	{
		auto skyboxPipeline = _resourceManager->GetRenderingPipeline(_T("SkyBox"));

		_skyBox = std::make_unique<SkyBox>(this, _T("Asset/Texture/Cloudymorning4k.hdr"), skyboxPipeline);
	}

	void GraphicsEngine::CreateOcclusionCulling()
	{
		_occlusionCulling = std::make_unique<OcclusionCulling>(this);

		static UINT64 downSampling = Hash::Hash64(_T("DownSampling"));
		static UINT64 mipmapDepth = Hash::Hash64(_T("OcclusionCullingDepth"));
		static UINT64 renderDepth = Hash::Hash64(_T("DefaultDepth"));

		_occlusionCulling->SetDownSampling(_resourceManager->GetPipelineState(downSampling));
		_occlusionCulling->SetRenderDepth(_resourceManager->GetTexture(renderDepth));
		_occlusionCulling->SetMipmapDepth(_resourceManager->GetTexture(mipmapDepth));
		_occlusionCulling->OnResize(this);
	}

	void GraphicsEngine::CreateCascadeShadow(int textureSize, int sliceCount)
	{
		_cascadeShadow = std::make_unique<CascadeShadow>(this, textureSize, sliceCount);
	}

	void GraphicsEngine::RegistRenderQueue(const std::vector<RenderObject*>& renderObjects, const Frustum& cameraFrustum)
	{
		//TODO::
		//컬링 옵션에 따라 소프트 컬링(절두체)
		//혹은 정렬까지.. 여기서 하면 될 것 같은데?
		//근데 그림자 그리는건 어떻하지?


		// 렌더 큐를 등록하기 전, 기존 그래픽스 엔진에 있던 내역들을 모두 제거합니다.
		_opaqueOccluderRenderQueue.clear();
		_opaqueRenderQueue.clear();
		_opaqueOccludeCulledRenderQueue.clear();
		_transparencyRenderQueue.clear();
		_debugRenderQueue.clear();


		// 뷰프러스텀 컬링을 진행한다.
		// 통과했을시에는 오클루더 여부에 따라 큐를 다른곳에 넣는다.
		for (auto& renderObject : renderObjects)
		{
			DecomposedRenderData decomposedRenderData;

			switch (renderObject->_mesh->GetMeshType())
			{
			case MeshBase::MeshType::Particle:
			{
				for (int materialIdx = 0; materialIdx < renderObject->_materials->size(); ++materialIdx)
				{
					decomposedRenderData._mesh = renderObject->_mesh;
					decomposedRenderData._material = renderObject->_materials->at(materialIdx);
					decomposedRenderData._renderInfo = renderObject->_renderInfo;
				}

				//todo 코드 추가해야합니다. 일단... 나머지 다만들고
				_transparencyRenderQueue.emplace_back(decomposedRenderData);
				break;
			}
			case MeshBase::MeshType::Mesh:
			{
				auto info = static_cast<MeshInfo*>(renderObject->_renderInfo);
				auto transform = info->GetTransformPointer();
				renderObject->_mesh->_center;

				if (CullingHelper::ViewFrustumCullingBoundingBox(transform->_world, renderObject->_mesh->_halfExtents, renderObject->_mesh->_center, cameraFrustum, decomposedRenderData._worldTranslatedBoundingBox._worldTranslatedBoundingBoxExtent, decomposedRenderData._worldTranslatedBoundingBox._worldTranslatedBoundingBoxCenterPos))
				{
					//머터리얼 유무에 따라 출력할지 말지를 결정합니다.
					for (int materialIdx = 0; materialIdx < renderObject->_materials->size(); ++materialIdx)
					{
						//서브메쉬와 머테리얼을 분리해서 데이터를 넘겨줍니다.
						decomposedRenderData._material = renderObject->_materials->at(materialIdx);
						decomposedRenderData._subMesh = renderObject->_mesh->GetSubMesh(materialIdx);

						if (decomposedRenderData._subMesh == nullptr)
							continue;

						decomposedRenderData._mesh = renderObject->_mesh;
						decomposedRenderData._renderInfo = renderObject->_renderInfo;

						switch (decomposedRenderData._material->GetRenderingMode())
						{
						case Material::RenderingMode::Opaque:
						{
							if (info->GetIsOccluder())
							{
								_opaqueOccluderRenderQueue.emplace_back(decomposedRenderData);
							}
							else
							{
								_opaqueRenderQueue.emplace_back(decomposedRenderData);
							}
							break;
						}
						case Material::RenderingMode::Transparency:
						{
							_transparencyRenderQueue.emplace_back(decomposedRenderData);
							break;
						}
						default:;
						}
					}
				}

				break;
			}
			case MeshBase::MeshType::SkinnedMesh:
			{
				auto info = static_cast<SkinnedMeshInfo*>(renderObject->_renderInfo);
				auto transform = info->GetTransformPointer();

				if (CullingHelper::ViewFrustumCullingBoundingBox(transform->_world, renderObject->_mesh->_halfExtents, renderObject->_mesh->_center, cameraFrustum, decomposedRenderData._worldTranslatedBoundingBox._worldTranslatedBoundingBoxExtent, decomposedRenderData._worldTranslatedBoundingBox._worldTranslatedBoundingBoxCenterPos))
				{
					//머터리얼 유무에 따라 출력할지 말지를 결정합니다.
					for (int materialIdx = 0; materialIdx < renderObject->_materials->size(); ++materialIdx)
					{
						//서브메쉬와 머테리얼을 분리해서 데이터를 넘겨줍니다.

						decomposedRenderData._material = renderObject->_materials->at(materialIdx);
						decomposedRenderData._subMesh = renderObject->_mesh->GetSubMesh(materialIdx);

						if (decomposedRenderData._subMesh == nullptr)
							continue;

						decomposedRenderData._mesh = renderObject->_mesh;
						decomposedRenderData._renderInfo = renderObject->_renderInfo;

						switch (decomposedRenderData._material->GetRenderingMode())
						{
						case Material::RenderingMode::Opaque:
						{
							if (info->GetIsOccluder())
							{
								_opaqueRenderQueue.emplace_back(decomposedRenderData);
							}
							else
							{
								_opaqueRenderQueue.emplace_back(decomposedRenderData);
							}
							break;
						}
						case Material::RenderingMode::Transparency:
						{
							_transparencyRenderQueue.emplace_back(decomposedRenderData);
							break;
						}
						default:;
						}
					}
				}

				break;
			}
			default:;
			}
		}
	}

	void GraphicsEngine::RegistLight(Light* const* lights, int lightCount, const Frustum& cameraFrustum, ConstantBufferPerCamera& perCamera, Light** currentSceneLight)
	{
		perCamera._lightCount = 0;

		for (int lightIdx = 0; lightIdx < lightCount; ++lightIdx)
		{
			if (lights[lightIdx]->GetLightType() == LightType::Direction)
			{
				perCamera._light[perCamera._lightCount] = lights[lightIdx]->GetLightData();
				currentSceneLight[perCamera._lightCount] = lights[lightIdx];
				perCamera._lightCount++;

			}
			else
			{
				if (CullingHelper::ViewFrustumCullingBoundingVolume(lights[lightIdx]->GetPosition(), lights[lightIdx]->GetAttenuationRadius(), cameraFrustum))
				{
					perCamera._light[perCamera._lightCount] = lights[lightIdx]->GetLightData();
					currentSceneLight[perCamera._lightCount] = lights[lightIdx];
					perCamera._lightCount++;
				}
			}
		}
	}

	void GraphicsEngine::PrepareBakeShadows(const ConstantBufferPerFrame& perFrameInfo, ConstantBufferPerCamera& perCameraInfo, Light** currentSceneLight)
	{

		//라이트들의 matrix들을 계산해야한다.
		for (int lightIdx = 0; lightIdx < perCameraInfo._lightCount; ++lightIdx)
		{
			currentSceneLight[lightIdx]->CalculateShadowMatrix();

			auto& lightData = currentSceneLight[lightIdx]->GetLightData();
			perCameraInfo._light[lightIdx] = lightData;
		}

		//Calc CascadeShadow
		{
			float cascadeOffset[4]{ 0.05f, 0.18f, 0.6f, 1.0f };

			//todo:: 라이팅 구조 개선할 것. (디렉셔널.. 포인트.. 스팟.. 라이트를 분리해야할 것 같음!)
			int lightIdx = 0;

			//디렉셔널 라이트 셰도우를 찾습니다.
			for (lightIdx = 0; lightIdx < 30; ++lightIdx)
			{
				if (perCameraInfo._light[lightIdx]._lightType == LightType::Direction && perCameraInfo._light[lightIdx]._shadowDynamicMapIdx == 0)
				{

					CascadeShadowSlice slice[4];
					ShadowHelper::CalculateCascadeShadowSlices2(perCameraInfo, perCameraInfo._light[lightIdx]._shadowMatrix, perCameraInfo._camera._cameraNear, perCameraInfo._camera._cameraFar, perCameraInfo._camera._cameraVerticalFOV, perCameraInfo._camera._aspectRatio, cascadeOffset, slice, _cascadeShadow->GetTextureSize(), perCameraInfo._light[lightIdx]._direction);
					for (int sliceIdx = 0; sliceIdx < 4; ++sliceIdx)
					{
						ShadowHelper::CalcuateViewProjectionMatrixFromCascadeSlice2(slice[sliceIdx], perCameraInfo._light[lightIdx]._direction, perCameraInfo._cascadeShadowInfo.shadowMatrix[sliceIdx]);
					}

					perCameraInfo._cascadeShadowInfo._cascadeSliceOffset[0] = cascadeOffset[0];
					perCameraInfo._cascadeShadowInfo._cascadeSliceOffset[1] = cascadeOffset[1];
					perCameraInfo._cascadeShadowInfo._cascadeSliceOffset[2] = cascadeOffset[2];
					perCameraInfo._cascadeShadowInfo._cascadeSliceOffset[3] = cascadeOffset[3];

					perCameraInfo._cascadeShadowInfo._cascadeScale[0] = slice[0]._frustumRadius * 2;
					perCameraInfo._cascadeShadowInfo._cascadeScale[1] = slice[1]._frustumRadius * 2;
					perCameraInfo._cascadeShadowInfo._cascadeScale[2] = slice[2]._frustumRadius * 2;
					perCameraInfo._cascadeShadowInfo._cascadeScale[3] = slice[3]._frustumRadius * 2;

					break;
				}
			}

		}
	}

	void GraphicsEngine::BakeShadows(const ConstantBufferPerFrame& perFrameInfo, ConstantBufferPerCamera& perCameraInfo, Light** currentSceneLight, const std::vector<DUOLGraphicsEngine::RenderObject*>& renderObjects)
	{
		//여기서 각각 라이트마다 오브젝트들을 컬링하고..
		//dynamic 오브젝트들을 구워준다

		//static이 처음 베이킹하거나, 움직였을 경우에는 다시 굽는다

		//라이트들의 matrix들을 계산해야한다.
#if defined(_DEBUG) || defined(DEBUG)
		_renderer->BeginEvent(_T("Shadow"));
#endif
		for (int lightIdx = 0; lightIdx < perCameraInfo._lightCount; ++lightIdx)
		{
			if (currentSceneLight[lightIdx]->GetLightType() != LightType::Direction)
			{
				switch (currentSceneLight[lightIdx]->GetLightState())
				{
				case LightState::Static:
				{
					if (currentSceneLight[lightIdx]->GetNeedBakeStaticShadowMap())
					{
						currentSceneLight[lightIdx]->ClearRenderTarget(_renderer);

						for (auto& renderObject : renderObjects)
						{
							//모든오브젝트들에 대해서 한번만 굽습니다.
							if (perCameraInfo._light[lightIdx]._lightType == LightType::Spot || perCameraInfo._light[lightIdx]._lightType == LightType::Area)
								_renderManager->RenderShadow(_lightManager->GetSpotStaticPipeline(), _lightManager->GetSpotSkinnedPipeline(), _lightManager->GetSpotRenderTargets(), renderObject, lightIdx, true);
							else if (perCameraInfo._light[lightIdx]._lightType == LightType::Point)
								_renderManager->RenderShadow(_lightManager->GetPointStaticPipeline(), _lightManager->GetPointSkinnedPipeline(), _lightManager->GetPointRenderTargets(), renderObject, lightIdx, true);

						}
						currentSceneLight[lightIdx]->SetNeedBakeStaticShadowMap(false);
					}
				}
				break;
				case LightState::Mixed:
				{
					bool isNeedStaticShadow = currentSceneLight[lightIdx]->GetNeedBakeStaticShadowMap();
					bool isNeedDynamicShadow = currentSceneLight[lightIdx]->GetNeedBakeDynamicShadowMap();

					for (auto& renderObject : renderObjects)
					{

						currentSceneLight[lightIdx]->ClearRenderTarget(_renderer);

						//이미 구웠다면 다시 구워도 되지 않기떄문에 넘어갑니다.
						//todo:: 빛이 움직였을때에는 다시 스태틱오브젝트들을 대상으로 다시 구워줍니다.
						if (isNeedStaticShadow)
						{
							//스태틱오브젝트들만 그려줍니다
							if (renderObject->_renderInfo->IsStatic())
							{
								if (perCameraInfo._light[lightIdx]._lightType == LightType::Spot || perCameraInfo._light[lightIdx]._lightType == LightType::Area)
									_renderManager->RenderShadow(_lightManager->GetSpotStaticPipeline(), _lightManager->GetSpotSkinnedPipeline(), _lightManager->GetSpotRenderTargets(), renderObject, lightIdx, true);
								else if (perCameraInfo._light[lightIdx]._lightType == LightType::Point)
									_renderManager->RenderShadow(_lightManager->GetPointStaticPipeline(), _lightManager->GetPointSkinnedPipeline(), _lightManager->GetPointRenderTargets(), renderObject, lightIdx, true);
							}

							currentSceneLight[lightIdx]->SetNeedBakeStaticShadowMap(false);
						}
						if (isNeedDynamicShadow)
						{
							if (!renderObject->_renderInfo->IsStatic())
							{
								if (perCameraInfo._light[lightIdx]._lightType == LightType::Spot || perCameraInfo._light[lightIdx]._lightType == LightType::Area)
									_renderManager->RenderShadow(_lightManager->GetSpotStaticPipeline(), _lightManager->GetSpotSkinnedPipeline(), _lightManager->GetSpotRenderTargets(), renderObject, lightIdx, false);
								else if (perCameraInfo._light[lightIdx]._lightType == LightType::Point)
									_renderManager->RenderShadow(_lightManager->GetPointStaticPipeline(), _lightManager->GetPointSkinnedPipeline(), _lightManager->GetPointRenderTargets(), renderObject, lightIdx, false);
							}

							currentSceneLight[lightIdx]->SetNeedBakeDynamicShadowMap(false);
						}
					}
				}
				break;
				case LightState::Dynamic:
				{
					if (currentSceneLight[lightIdx]->GetNeedBakeDynamicShadowMap())
					{
						currentSceneLight[lightIdx]->ClearRenderTarget(_renderer);

						//다이나믹오브젝트들만 그려줍니다.	
						for (auto& renderObject : renderObjects)
						{

							if (perCameraInfo._light[lightIdx]._lightType == LightType::Spot || perCameraInfo._light[lightIdx]._lightType == LightType::Area)
								_renderManager->RenderShadow(_lightManager->GetSpotStaticPipeline(), _lightManager->GetSpotSkinnedPipeline(), _lightManager->GetSpotRenderTargets(), renderObject, lightIdx, false);
							else if (perCameraInfo._light[lightIdx]._lightType == LightType::Point)
								_renderManager->RenderShadow(_lightManager->GetPointStaticPipeline(), _lightManager->GetPointSkinnedPipeline(), _lightManager->GetPointRenderTargets(), renderObject, lightIdx, false);
						}
						currentSceneLight[lightIdx]->SetNeedBakeDynamicShadowMap(false);
					}
				}
				break;
				case LightState::Unknown: break;
				default:;
				}
			}
		}

		//todo :: 쉐도우 렌더타겟또한 정리해야함
		ClearRenderTarget(_cascadeShadow->GetShadowMapDepth());;
		_renderManager->RenderCascadeShadow(_cascadeShadow->GetShadowStatic(), _cascadeShadow->GetShadowSkinned(), _cascadeShadow->GetShadowMapDepth(), perFrameInfo, renderObjects);

#if defined(_DEBUG) || defined(DEBUG)
		_renderer->EndEvent();
#endif
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

	void GraphicsEngine::RenderDebugObject(RenderObject* render_object)
	{
		_renderManager->PushDebugObject(render_object);
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
	}

	void GraphicsEngine::ClearRenderTarget(DUOLGraphicsLibrary::RenderTarget* renderTarget)
	{
		_renderer->ClearRenderTarget(renderTarget);
	}

	void GraphicsEngine::ResizeTexture(DUOLGraphicsLibrary::Texture* texture, const DUOLMath::Vector2& resolution)
	{
		_resourceManager->ResizeTexture(texture, resolution);
	}

	void GraphicsEngine::Execute(const std::vector<DUOLGraphicsEngine::RenderObject*>& renderObjects,
		const std::vector<RenderingPipelinesList>& renderPipelinesList,
		const std::vector<DUOLGraphicsLibrary::ICanvas*>& canvases,
		const CurrentSceneInfo& currentSceneInfo)
	{
		//1. 스키닝데이터 계산

		//2. UI출력(백버퍼에 그리지 않는 녀석)

		ConstantBufferPerFrame perFrameInfo(currentSceneInfo);
		_renderManager->SetPerFrameBuffer(perFrameInfo);


		for (auto& renderingPipeline : renderPipelinesList)
		{

			if (renderingPipeline._cameraData == nullptr)
				continue;

			ConstantBufferPerCamera perCameraInfo;
			perCameraInfo._camera = *renderingPipeline._cameraData;

			Frustum cameraFrustum;
			CullingHelper::CreateFrustumFromCamera(perCameraInfo._camera, cameraFrustum);

			//카메라 정보로 라이트 컬링

			//Todo:: 가변적인 라이트개수를 위해... 어떻게 해야할까
			Light* currentCameraLights[30];

			//카메라버퍼에 라이트정보를 갱신함.
			//라이트 볼륨을 기준으로 컬링할 녀석들은 컬링한다..
			RegistLight(currentSceneInfo._lights, currentSceneInfo._lightCount, cameraFrustum, perCameraInfo, currentCameraLights);
			RegistRenderQueue(renderObjects, cameraFrustum);

			//베이킹에 필요한 데이터를 버퍼에 세팅해줍니다.
			PrepareBakeShadows(perFrameInfo, perCameraInfo, currentCameraLights);

			_renderManager->SetPerCameraBuffer(perCameraInfo);

			//
			BakeShadows(perFrameInfo, perCameraInfo, currentCameraLights, renderObjects);

			//if(Occlusion) 현재는 디폴트로 켜놓는다.
			//opaque 파이프라인에 바인딩 된 파이프라인 중, RenderType의 파이프라인만, Occluder 오브젝트들을 렌더 실행시킵니다
			for (auto& pipeline : renderingPipeline._opaquePipelines)
			{
				if (pipeline._renderingPipeline->GetPipelineType() == PipelineType::Render)
					_renderManager->ExecuteRenderingPipeline(pipeline._renderingPipeline, _opaqueOccluderRenderQueue);
			}

			_renderManager->OcclusionCulling(_occlusionCulling.get(), _opaqueRenderQueue, _opaqueOccludeCulledRenderQueue);

			//컬링완료. 컬링한 객체들에 대해서 이제 모든 파이프라인을 실행시켜줍니다.
			for (auto& pipeline : renderingPipeline._opaquePipelines)
			{
				_renderManager->ExecuteRenderingPipeline(pipeline._renderingPipeline, _opaqueOccludeCulledRenderQueue, pipeline._perObjectBufferData, pipeline._dataSize);
			}

			//// 무조건적으로 스카이박스는 Opaque와 Transparency 사이에 그려줘야 합니다.....
			if (renderingPipeline._drawSkybox)
				_renderManager->RenderSkyBox(_skyBox->GetSkyboxPipeline(), _skyBox->GetSkyboxSphereMesh()->_vertexBuffer, _skyBox->GetSkyboxSphereMesh()->_subMeshs[0]._indexBuffer, perCameraInfo._camera);


			if (renderingPipeline._drawDebug)
			{
				static UINT64 debugRT = Hash::Hash64(_T("Debug"));
				_renderManager->ExecuteDebugRenderPass(_resourceManager->GetRenderingPipeline(debugRT));
			}

			_renderManager->ClearOITUAVs();

			//투명한 객체들을 출력해줍니다. 
			for (auto& pipeline : renderingPipeline._transparencyPipelines)
			{
				_renderManager->ExecuteRenderingPipeline(pipeline._renderingPipeline, _transparencyRenderQueue, pipeline._perObjectBufferData, pipeline._dataSize);
			}

			for (auto& canvas : canvases)
			{
				_fontEngine->DrawCanvas(canvas);
			}

			_fontEngine->Execute();

			//각각의 텍스쳐에 그렸던 UI들을 게임뷰에 그려줍니다.
			for (auto& canvas : canvases)
			{
				auto canvasResource = canvas->GetTexture();

				//canvas Resource가 Nullptr일때에는 Backbuffer에 다이렉트로 그리는 캔버스라는 뜻이다.
				if (canvasResource != nullptr)
					_renderManager->RenderCanvas(_drawCanvasOnGameViewPipeline, canvasResource);
			}

			if (renderingPipeline._drawGameViewToBackBuffer)
			{
				_renderManager->ExecuteRenderingPipeline(_drawGameViewOnBakcBufferPipeline, _transparencyRenderQueue, nullptr, 0);
			}

		}

		//라이트 베이킹의 트리거를 리셋합니다
		for (int lightIdx = 0; lightIdx < currentSceneInfo._lightCount; ++lightIdx)
		{
			currentSceneInfo._lights[lightIdx]->ResetLightBakeFlag();
		}

		_renderManager->ClearState();

	}

	void GraphicsEngine::Begin()
	{
		_renderManager->Begin();
	}

	void GraphicsEngine::End()
	{
		_renderManager->End();
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
		_occlusionCulling->UnloadRenderTargets(this);
		_resourceManager->OnResize(resolution);
		_occlusionCulling->OnResize(this);
		_oitRenderer->OnResize(_resourceManager.get(), resolution);
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
			return uint64_t{};

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

	DUOLGraphicsLibrary::Texture* GraphicsEngine::CreateTexture(const DUOLCommon::tstring& objectID,
		float width, float height, int size, void* initialData)
	{
		DUOLGraphicsLibrary::TextureDesc desc;

		desc._textureExtent.x = width;
		desc._textureExtent.y = height;
		desc._format = DUOLGraphicsLibrary::ResourceFormat::FORMAT_R8G8B8A8_UNORM;
		desc._size = size;
		desc._initData = initialData;
		desc._bindFlags |= static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE);

		return _resourceManager->CreateTexture(objectID, desc);
	}

	DUOLGraphicsLibrary::Texture* GraphicsEngine::CreateTexture(const DUOLCommon::tstring& objectID)
	{
		DUOLGraphicsLibrary::TextureDesc desc;

		DUOLCommon::tstring texturePath = TEXT("Asset/Texture/") + objectID;

		std::string st = DUOLCommon::StringHelper::ToString(texturePath);

		desc._texturePath = st.c_str();

		return _resourceManager->CreateTexture(objectID, desc);
	}

	MeshBase* GraphicsEngine::CreateParticle(const DUOLCommon::tstring& objectID, int maxParticle)
	{
		return _resourceManager->CreateParticleBuffer(objectID, maxParticle);
	}

	DUOLGraphicsLibrary::ICanvas* GraphicsEngine::CreateCanvas(DUOLGraphicsLibrary::CanvasRenderMode rendertype,
		const DUOLCommon::tstring& canvasName, int width, int height)
	{
		if (rendertype == DUOLGraphicsLibrary::CanvasRenderMode::Texture)
		{
			DUOLGraphicsLibrary::TextureDesc desc;

			desc._textureExtent = DUOLMath::Vector3{ static_cast<float>(width), static_cast<float>(height), 1.f };
			desc._format = DUOLGraphicsLibrary::ResourceFormat::FORMAT_B8G8R8A8_UNORM;
			desc._bindFlags = (static_cast<long>(DUOLGraphicsLibrary::BindFlags::SHADERRESOURCE) | static_cast<long>(DUOLGraphicsLibrary::BindFlags::RENDERTARGET));

			// OnResize가 통해야 하므로 마지막에 true가 들어가야한다. 
			auto texture = _resourceManager->CreateTexture(canvasName, desc,true);

			return _fontEngine->CreateCanvas(canvasName, rendertype, texture);
		}
		else
		{
			return _fontEngine->CreateCanvas(canvasName, rendertype, nullptr);
		}
	}

	DUOLGraphicsLibrary::IFont* GraphicsEngine::CreateIFont(const DUOLCommon::tstring& fontPath)
	{
		return _fontEngine->CreateFontFromTTF(fontPath);
	}

	DUOLGraphicsEngine::Light* GraphicsEngine::CreateLight(const uint64_t& lightId)
	{
		return _lightManager->CreateLight(lightId);
	}

	bool GraphicsEngine::DeleteLight(const uint64_t& lightId)
	{
		return _lightManager->DeleteLight(lightId);
	}

	DUOLGraphicsLibrary::IFont* GraphicsEngine::GetFont(const DUOLCommon::tstring& fontName)
	{
		return _fontEngine->GetFont(fontName);
	}

	bool GraphicsEngine::GetRenderData(DUOLGraphicsLibrary::QueryInfo& outData)
	{
		return _renderManager->GetRenderData(outData);
	}

	bool GraphicsEngine::DeleteCanvas(const DUOLCommon::tstring& canvasName)
	{
		return _fontEngine->DeleteCanvas(canvasName);
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

	AnimationClip* GraphicsEngine::LoadAnimationClip(int index)
	{
		return _resourceManager->GetAnimationClip(index);
	}

	int GraphicsEngine::LoadAnimationClipSize()
	{
		return _resourceManager->GetAnimationClipSize();
	}


	DUOLGraphicsLibrary::Texture* GraphicsEngine::LoadTexture(const DUOLCommon::tstring& objectID)
	{
		return _resourceManager->GetTexture(objectID);
	}

	RenderingPipeline* GraphicsEngine::LoadRenderingPipeline(const DUOLCommon::tstring& objectID)
	{
		return _resourceManager->GetRenderingPipeline(objectID);
	}

	DUOLGraphicsLibrary::PipelineState* GraphicsEngine::LoadPipelineState(const DUOLCommon::tstring& objectID)
	{
		return _resourceManager->GetPipelineState(Hash::Hash64(objectID));
	}


}

