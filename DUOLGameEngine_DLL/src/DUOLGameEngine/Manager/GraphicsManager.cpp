#include "DUOLGameEngine/Manager/GraphicsManager.h"

#include "DUOLGameEngine/Manager/EventManager.h"

#include <algorithm>
#include <spdlog/fmt/bundled/core.h>

#include "DUOLGraphicsEngine/RenderManager/RenderingPipeline/RenderingPipeline.h"
#include "DUOLCommon/Log/LogHelper.h"
#include "DUOLGameEngine/Engine.h"
#include "DUOLGameEngine/ECS/Component/Camera.h"
#include "DUOLGameEngine/Manager/TimeManager.h"
#include "DUOLGameEngine/Manager/UIManager.h"
#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"
#include "DUOLGameEngine/Util/Geometries.h"
#include "DUOLGameEngine/Util/Quadtree.h"
#include "DUOLGameEngine/Util/Octree.h"

#include "DUOLGraphicsEngine/GraphicsEngineFlags.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/Mesh.h"
#include "DUOLGraphicsLibrary/Renderer/RenderTarget.h"
#include "DUOLGraphicsLibrary/Renderer/Texture.h"


namespace DUOLGameEngine
{
	GraphicsManager::GraphicsManager() :
		_renderingPipelineSetups({})
		, _currentSceneInfo({})
	{
	}

	GraphicsManager::~GraphicsManager()
	{
	}

	DUOLGraphicsEngine::CurrentSceneInfo* GraphicsManager::GetCurrentSceneInfo()
	{
		return &_currentSceneInfo;
	}

	void GraphicsManager::ReserveRenderObject(DUOLGraphicsEngine::RenderObject* renderObjectInfo)
	{
		_renderObjectList.push_back(renderObjectInfo);
	}

	void GraphicsManager::ReserveRenderDebugObject(DUOLGraphicsEngine::RenderObject* renderObjectInfo)
	{
		_graphicsEngine->RenderDebugObject(renderObjectInfo);
	}

	void GraphicsManager::ReserveCanvas(DUOLGraphicsLibrary::ICanvas* canvas)
	{
		_canvasList.push_back(canvas);
	}

	void GraphicsManager::ClearConstantBufferPerFrame()
	{
		_currentSceneInfo._lightCount = 0;
	}

	void GraphicsManager::ClearAllRenderTarget()
	{
		_graphicsEngine->ClearRenderTargets();
	}

	void* GraphicsManager::GetShaderResourceAddress(const DUOLCommon::tstring& id) const
	{
		DUOLGraphicsLibrary::Texture* texture = _graphicsEngine->LoadTexture(id);

		return texture->GetShaderResourceAddress();
	}

	void GraphicsManager::Initialize(const EngineSpecification& gameSpecification)
	{
		DUOLGraphicsEngine::GraphicsEngineDesc graphicsEngineDesc;

		graphicsEngineDesc._isFullscreen = false;
		graphicsEngineDesc._isMSAA = false;
		graphicsEngineDesc._sampleCount = 0;
		graphicsEngineDesc._screenSize =
			DUOLMath::Vector2(static_cast<float>(gameSpecification._screenWidth),
				static_cast<float>(gameSpecification._screenHeight));

		graphicsEngineDesc._handle = reinterpret_cast<long>(gameSpecification.hWnd);
		graphicsEngineDesc._moduleType = DUOLGraphicsEngine::RendererModuleType::DIRECTX11;

		_graphicsEngine =
			std::make_shared<DUOLGraphicsEngine::GraphicsEngine>(graphicsEngineDesc);

		if (_graphicsEngine == nullptr)
		{
			DUOL_CRITICAL("GraphicsEngine Nullptr");
		}

		_screenSize = DUOLMath::Vector2{ static_cast<float>(gameSpecification._screenWidth), static_cast<float>(gameSpecification._screenHeight) };

		// GraphicsManager OnResize event handler register. (어차피 엔진과 함께 계속 존속할 매니저이므로 ID를 받을 필요는 없다 ..)
		const std::function<void(std::any)> functor =
			std::bind(&GraphicsManager::OnResize, this, std::placeholders::_1);

		EventManager::GetInstance()->AddEventFunction(TEXT("Resize"), functor);

		// Pipeline setup과 관련된 초기화를 진행합니다.
		InitializeGraphicsPipelineSetups();

		DUOL_INFO(DUOL_FILE, "GraphicsManager Initialize Success !");
	}

	void GraphicsManager::UnInitialize()
	{
		_graphicsEngine.reset();
	}

	void GraphicsManager::InitializeGraphicsPipelineSetups()
	{
		static const TCHAR* defaultT = (_T("Default"));
		static const TCHAR* deferred = (_T("Lighting"));
		static const TCHAR* sceneView = (_T("SceneView"));
		static const TCHAR* sceneView_Sample = (_T("SceneView_Sample"));

		static const TCHAR* particle = (_T("OITLayerCreate"));
		static const TCHAR* oit = (_T("OIT"));
		static const TCHAR* fxaa = (_T("FXAA"));

		static const TCHAR* idOutline = (_T("IDOutline"));
		static const TCHAR* outlineMerge = (_T("MergeOutline"));

		static const TCHAR* sceneViewGizmo = (_T("SceneView_Gizmo"));

		static const TCHAR* bloomSampling = (_T("BloomSampling"));

		static const TCHAR* downSampling16 = (_T("DownSample16"));
		static const TCHAR* downSampling8 = (_T("DownSample8"));
		static const TCHAR* downSampling32 = (_T("DownSample32"));
		static const TCHAR* downSampling64 = (_T("DownSample64"));
		static const TCHAR* downSampling128 = (_T("DownSample128"));
		static const TCHAR* downSampling4 = (_T("DownSample4"));
		static const TCHAR* downSampling2 = (_T("DownSample2"));

		static const TCHAR* upblur64 = (_T("UpBlur64"));
		static const TCHAR* upblur32 = (_T("UpBlur32"));
		static const TCHAR* upblur16 = (_T("UpBlur16"));
		static const TCHAR* upblur8 = (_T("UpBlur8"));
		static const TCHAR* upblur4 = (_T("UpBlur4"));
		static const TCHAR* upblur2 = (_T("UpBlur2"));
		static const TCHAR* upblurorigin = (_T("UpBlurOrigin"));

		static const TCHAR* blur128X = (_T("Blur128X"));
		static const TCHAR* blur128Y = (_T("Blur128Y"));
		static const TCHAR* blur64X = (_T("Blur64X"));
		static const TCHAR* blur64Y = (_T("Blur64Y"));
		static const TCHAR* blur32X = (_T("Blur32X"));
		static const TCHAR* blur32Y = (_T("Blur32Y"));
		static const TCHAR* blur16X = (_T("Blur16X"));
		static const TCHAR* blur16Y = (_T("Blur16Y"));
		static const TCHAR* blur8X = (_T("Blur8X"));
		static const TCHAR* blur8Y = (_T("Blur8Y"));
		static const TCHAR* blur4X = (_T("Blur4X"));
		static const TCHAR* blur4Y = (_T("Blur4Y"));
		static const TCHAR* blur2X = (_T("Blur2X"));
		static const TCHAR* blur2Y = (_T("Blur2Y"));

		static const TCHAR* bloom = (_T("Bloom"));
		static const TCHAR* tonemapping = (_T("ToneMapping"));

		static const TCHAR* camMotionBlur = _T("CameraMotionBlur");
		static const TCHAR* mergeCamMotionBlur = _T("MergeCameraMotionBlur");

		//무조건적으로 스카이박스는 Opaque와 Transparency 사이에 그려줘야 합니다..... 근데 이거 어떻게해요?
		static const TCHAR* skybox = _T("SkyBox");


		static const TCHAR* ssrUV = (_T("SSRUV"));
		static const TCHAR* ssrLightBlurX = (_T("BlurLightX"));
		static const TCHAR* ssrLightBlurY = (_T("BlurLightY"));

		static const TCHAR* SSR = (_T("SSR"));

		static const TCHAR* exponentialHeightFog = (_T("ExponentialHeightFog"));
		static const TCHAR* rimLight = (_T("RimLight"));

		static const TCHAR* debugGame = _T("Debug_Game");

#pragma region GAME_SETUP
		_pipelineSetups.insert({ TEXT("Game"), {} });

		auto&& gameSetup = _pipelineSetups.at(TEXT("Game"));

		gameSetup._pipelineListName = TEXT("Game");

		gameSetup._opaquePipelines.push_back(_graphicsEngine->LoadRenderingPipeline(defaultT));
		gameSetup._opaquePipelines.push_back(_graphicsEngine->LoadRenderingPipeline(deferred));

#pragma region preSSR
		gameSetup._opaquePipelines.push_back(_graphicsEngine->LoadRenderingPipeline(ssrLightBlurX));
		gameSetup._opaquePipelines.push_back(_graphicsEngine->LoadRenderingPipeline(ssrLightBlurY));
		gameSetup._opaquePipelines.push_back({ _graphicsEngine->LoadTexture(_T("SSRLightBlur")) });

#pragma endregion

		gameSetup._transparencyPipelines.push_back({ _graphicsEngine->LoadTexture(_T("Normal")), _graphicsEngine->LoadTexture(_T("PostNormal")) });
		gameSetup._transparencyPipelines.push_back({ _graphicsEngine->LoadTexture(_T("World")), _graphicsEngine->LoadTexture(_T("PostWorld")) });
		gameSetup._transparencyPipelines.push_back({ _graphicsEngine->LoadTexture(_T("MetalRoughnessAO")), _graphicsEngine->LoadTexture(_T("PostMetalRoughnessAO")) });
		gameSetup._transparencyPipelines.push_back({ _graphicsEngine->LoadTexture(_T("DefaultDepth")), _graphicsEngine->LoadTexture(_T("PostDefaultDepth")) });
		gameSetup._transparencyPipelines.push_back({ _graphicsEngine->LoadTexture(_T("ObjectID")), _graphicsEngine->LoadTexture(_T("PostObjectID")) });
		gameSetup._transparencyPipelines.push_back({ _graphicsEngine->LoadTexture(_T("Effect")), _graphicsEngine->LoadTexture(_T("PostEffect")) });


		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(particle));
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oit));
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(fxaa));
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(rimLight));
		gameSetup._transparencyPipelines.back()._procedure._procedurePipeline._perObjectBufferData = &_graphicsSetting._rimLight._clipPoint;
		gameSetup._transparencyPipelines.back()._procedure._procedurePipeline._dataSize = sizeof(RimLight);


#pragma region Bloom
		// Bloom curve
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(bloomSampling));
		gameSetup._transparencyPipelines.back()._procedure._procedurePipeline._perObjectBufferData = &_graphicsSetting._bloom._intensity;
		gameSetup._transparencyPipelines.back()._procedure._procedurePipeline._dataSize = sizeof(Bloom);

		// DownSampling_2
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(downSampling2));
		// DownSampling_4
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(downSampling4));
		//DownSampling x8
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(downSampling8));
		// DownSampling_16
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(downSampling16));
		// DownSampling_32
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(downSampling32));
		// DownSampling_64
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(downSampling64));
		//DownSampling x128
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(downSampling128));

		// Blur_128
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur128X));
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur128Y));
		// Blur 128 + DownSampling 64
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(upblur64));

		// Blur_64
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur64X));
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur64Y));
		// Blur 64 + DownSampling 32
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(upblur32));

		// Blur_32
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur32X));
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur32Y));
		// Blur 32 + DownSampling 16
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(upblur16));

		// Blur_16
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur16X));
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur16Y));
		// Blur 8 + DownSampling 4
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(upblur8));

		// Blur_8
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur8X));
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur8Y));
		// Blur 8 + DownSampling 4
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(upblur4));

		// Blur_4
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur4X));
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur4Y));
		// Blur 4 + DownSampling 2
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(upblur2));

		//// Blur_2
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur2X));
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur2Y));
		// Blur 2 + Origin
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(upblurorigin));
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(bloom));

#pragma endregion
#pragma region SSR
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(ssrUV));
		gameSetup._transparencyPipelines.back()._procedure._procedurePipeline._perObjectBufferData = &_graphicsSetting._screenSpaceReflection._stride;
		gameSetup._transparencyPipelines.back()._procedure._procedurePipeline._dataSize = sizeof(ScreenSpaceReflection);

		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(SSR));

#pragma endregion

#pragma region ToneMapping
		// tone mapping
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(tonemapping));
		gameSetup._transparencyPipelines.back()._procedure._procedurePipeline._perObjectBufferData = &_graphicsSetting._toneMapping._exposure;
		gameSetup._transparencyPipelines.back()._procedure._procedurePipeline._dataSize = 16;
#pragma endregion
#pragma region ExponentialHeightFog
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(exponentialHeightFog));
		gameSetup._transparencyPipelines.back()._procedure._procedurePipeline._perObjectBufferData = &_graphicsSetting._exponentialHeightFog._fogDensity;
		gameSetup._transparencyPipelines.back()._procedure._procedurePipeline._dataSize = sizeof(ExponentialHeightFog);
#pragma endregion


		// TODO - 이거 나중에 포스트 프로세싱 파이프 라인은 따로 나누어야함.
#if defined(_DEBUG)
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(debugGame));
#endif

		gameSetup._drawDebug = false;
		gameSetup._drawGameViewToBackBuffer = true;
#pragma endregion

#pragma region GAME_VIEW_SETUP
		_pipelineSetups.insert({ TEXT("GameView"), {} });

		auto&& gameViewSetup = _pipelineSetups.at(TEXT("GameView"));

		gameViewSetup._pipelineListName = TEXT("GameView");

		gameViewSetup._opaquePipelines.push_back(_graphicsEngine->LoadRenderingPipeline(defaultT));
		gameViewSetup._opaquePipelines.push_back(_graphicsEngine->LoadRenderingPipeline(deferred));

#pragma region preSSR
		gameViewSetup._opaquePipelines.push_back(_graphicsEngine->LoadRenderingPipeline(ssrLightBlurX));
		gameViewSetup._opaquePipelines.push_back(_graphicsEngine->LoadRenderingPipeline(ssrLightBlurY));
		gameViewSetup._transparencyPipelines.push_back({ _graphicsEngine->LoadTexture(_T("SSRLightBlur")) });
#pragma endregion

		gameViewSetup._transparencyPipelines.push_back({ _graphicsEngine->LoadTexture(_T("Normal")), _graphicsEngine->LoadTexture(_T("PostNormal")) });
		gameViewSetup._transparencyPipelines.push_back({ _graphicsEngine->LoadTexture(_T("World")), _graphicsEngine->LoadTexture(_T("PostWorld")) });
		gameViewSetup._transparencyPipelines.push_back({ _graphicsEngine->LoadTexture(_T("MetalRoughnessAO")), _graphicsEngine->LoadTexture(_T("PostMetalRoughnessAO")) });
		gameViewSetup._transparencyPipelines.push_back({ _graphicsEngine->LoadTexture(_T("DefaultDepth")), _graphicsEngine->LoadTexture(_T("PostDefaultDepth")) });
		gameViewSetup._transparencyPipelines.push_back({ _graphicsEngine->LoadTexture(_T("ObjectID")), _graphicsEngine->LoadTexture(_T("PostObjectID")) });
		gameViewSetup._transparencyPipelines.push_back({ _graphicsEngine->LoadTexture(_T("Effect")), _graphicsEngine->LoadTexture(_T("PostEffect")) });

		gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(particle));
		gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oit));
		gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(fxaa));
		gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(rimLight));
		gameViewSetup._transparencyPipelines.back()._procedure._procedurePipeline._perObjectBufferData = &_graphicsSetting._rimLight._clipPoint;
		gameViewSetup._transparencyPipelines.back()._procedure._procedurePipeline._dataSize = sizeof(RimLight);
		// TODO - 이거 나중에 포스트 프로세싱 파이프 라인은 따로 나누어야함.
#pragma region Bloom
		// Bloom curve
		gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(bloomSampling));
		gameViewSetup._transparencyPipelines.back()._procedure._procedurePipeline._perObjectBufferData = &_graphicsSetting._bloom._intensity;
		gameViewSetup._transparencyPipelines.back()._procedure._procedurePipeline._dataSize = sizeof(Bloom);
		// DownSampling_2
		gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(downSampling2));
		// DownSampling_4
		gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(downSampling4));
		//DownSampling x8
		gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(downSampling8));
		// DownSampling_16
		gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(downSampling16));
		// DownSampling_32
		gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(downSampling32));
		// DownSampling_64
		gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(downSampling64));
		//DownSampling x128
		gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(downSampling128));

		// Blur_128
		gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur128X));
		gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur128Y));
		// Blur 128 + DownSampling 64
		gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(upblur64));

		// Blur_64
		gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur64X));
		gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur64Y));
		// Blur 64 + DownSampling 32
		gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(upblur32));

		// Blur_32
		gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur32X));
		gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur32Y));
		// Blur 32 + DownSampling 16
		gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(upblur16));

		// Blur_16
		gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur16X));
		gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur16Y));
		// Blur 8 + DownSampling 4
		gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(upblur8));

		// Blur_8
		gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur8X));
		gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur8Y));
		// Blur 8 + DownSampling 4
		gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(upblur4));

		gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur4X));
		gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur4Y));
		// Blur 4 + DownSampling 2
		gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(upblur2));

		//// Blur_2
		gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur2X));
		gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur2Y));
		// Blur 2 + Origin
		gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(upblurorigin));
		gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(bloom));
#pragma endregion

#pragma region SSR
		gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(ssrUV));
		gameViewSetup._transparencyPipelines.back()._procedure._procedurePipeline._perObjectBufferData = &_graphicsSetting._screenSpaceReflection._stride;
		gameViewSetup._transparencyPipelines.back()._procedure._procedurePipeline._dataSize = sizeof(ScreenSpaceReflection);

		gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(SSR));
		gameViewSetup._transparencyPipelines.push_back({ _graphicsEngine->LoadTexture(_T("SSRLightBlur")) });

#pragma endregion

#pragma region ToneMapping
		// tone mapping
		gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(tonemapping));
		gameViewSetup._transparencyPipelines.back()._procedure._procedurePipeline._perObjectBufferData = &_graphicsSetting._toneMapping._exposure;
		gameViewSetup._transparencyPipelines.back()._procedure._procedurePipeline._dataSize = 16;
#pragma endregion
#pragma region ExponentialHeightFog
		gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(exponentialHeightFog));
		gameViewSetup._transparencyPipelines.back()._procedure._procedurePipeline._perObjectBufferData = &_graphicsSetting._exponentialHeightFog._fogDensity;
		gameViewSetup._transparencyPipelines.back()._procedure._procedurePipeline._dataSize = sizeof(ExponentialHeightFog);
#pragma endregion

		gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(sceneView));

		gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(sceneView));
#pragma endregion

#pragma region SCENE_VIEW_SETUP
		_pipelineSetups.insert({ TEXT("Scene"), {} });

		auto&& sceneSetup = _pipelineSetups.at(TEXT("Scene"));

		sceneSetup._pipelineListName = TEXT("Scene");
		sceneSetup._drawDebug = true;

		// 2. 사실 Scene View는 Game을 그리는 것에서 기즈모 오브젝트가 추가되고, 카메라 옵션, Size 옵션이 다를 뿐이지
		// 기본적인 파이프라인 패스는 같다.
		sceneSetup._opaquePipelines.push_back(_graphicsEngine->LoadRenderingPipeline(defaultT));
		sceneSetup._opaquePipelines.push_back(_graphicsEngine->LoadRenderingPipeline(deferred));

		sceneSetup._transparencyPipelines.push_back({ _graphicsEngine->LoadTexture(_T("Normal")), _graphicsEngine->LoadTexture(_T("PostNormal")) });
		sceneSetup._transparencyPipelines.push_back({ _graphicsEngine->LoadTexture(_T("World")), _graphicsEngine->LoadTexture(_T("PostWorld")) });
		sceneSetup._transparencyPipelines.push_back({ _graphicsEngine->LoadTexture(_T("MetalRoughnessAO")), _graphicsEngine->LoadTexture(_T("PostMetalRoughnessAO")) });
		sceneSetup._transparencyPipelines.push_back({ _graphicsEngine->LoadTexture(_T("DefaultDepth")), _graphicsEngine->LoadTexture(_T("PostDefaultDepth")) });
		sceneSetup._transparencyPipelines.push_back({ _graphicsEngine->LoadTexture(_T("ObjectID")), _graphicsEngine->LoadTexture(_T("PostObjectID")) });
		sceneSetup._transparencyPipelines.push_back({ _graphicsEngine->LoadTexture(_T("Effect")), _graphicsEngine->LoadTexture(_T("PostEffect")) });


		sceneSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(particle));
		sceneSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oit));
		sceneSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(fxaa));

		sceneSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(sceneView_Sample));
		sceneSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(sceneView));
#pragma endregion

#pragma region SCENE_VIEW_IDOUTLINE_SETUP
		_pipelineSetups.insert({ TEXT("IDOutline"), {} });

		auto&& idOutlineSetup = _pipelineSetups.at(TEXT("IDOutline"));

		idOutlineSetup._pipelineListName = TEXT("IDOutline");

		// Outline을 그린다.
		idOutlineSetup._opaquePipelines.push_back(_graphicsEngine->LoadRenderingPipeline(idOutline));

		idOutlineSetup._opaquePipelines.back()._procedure._procedurePipeline._dataSize = sizeof(uint64_t);

		idOutlineSetup._opaquePipelines.push_back(_graphicsEngine->LoadRenderingPipeline(outlineMerge));
#pragma endregion

#pragma region DEFAULT_SETUP
		_pipelineSetups.insert({ TEXT("Default") , {} });

		auto&& defaultSetup = _pipelineSetups.at(TEXT("Default"));

		defaultSetup._pipelineListName = TEXT("Default");

		defaultSetup._opaquePipelines.push_back(_graphicsEngine->LoadRenderingPipeline(defaultT));
		defaultSetup._drawSkybox = false;
#pragma endregion

#pragma region CAMERA_MOTION_BLUR_SETUP
		_pipelineSetups.insert({ TEXT("CameraMotionBlur"), {} });

		auto&& camMotionBlurSetup = _pipelineSetups.at(TEXT("CameraMotionBlur"));

		camMotionBlurSetup._pipelineListName = TEXT("CameraMotionBlur");

		camMotionBlurSetup._opaquePipelines.push_back(_graphicsEngine->LoadRenderingPipeline(camMotionBlur));
		camMotionBlurSetup._opaquePipelines.push_back(_graphicsEngine->LoadRenderingPipeline(mergeCamMotionBlur));

		camMotionBlurSetup._drawSkybox = false;
#pragma endregion
	}

	void GraphicsManager::OctreeCulling(const DUOLGraphicsEngine::RenderingPipelinesList& renderingPipelineList,
		DUOLGameEngine::Octree* octree, std::vector<DUOLGraphicsEngine::RenderObject*>& culledRenderObject)
	{
		Frustum frustum;

		GeometryHelper::CreateFrustumFromCamera(*renderingPipelineList._cameraData, frustum);

		std::vector<DUOLGraphicsEngine::RenderObject*> renderObjects;

		// 키 값으로 Culled object check.
		std::unordered_map<void*, bool> datas;

		octree->ViewFrustumCullingAllNodes(frustum, datas);

		// datas에 있는 RenderObject 만 최종적으로 보낸다 ..!
		for (auto renderObject : _renderObjectList)
		{
			culledRenderObject.push_back(renderObject);

			continue;

			// 스태틱 메쉬에 대해서만 컬링한다.
			if (renderObject->_mesh->GetMeshType() != DUOLGraphicsEngine::MeshBase::MeshType::Mesh)
			{
				culledRenderObject.push_back(renderObject);

				continue;
			}

			// 스태틱 메쉬인데 앞의 옥트리 컬링을 통과한 녀석만 넣어준다.
			if (datas.contains(renderObject))
			{
				culledRenderObject.push_back(renderObject);
			}
		}
	}

	void GraphicsManager::QuadtreeCulling(const DUOLGraphicsEngine::RenderingPipelinesList& renderingPipelineList,
		DUOLGameEngine::Quadtree* quadtree, std::vector<DUOLGraphicsEngine::RenderObject*>& culledRenderObject)
	{
		Frustum frustum;

		GeometryHelper::CreateFrustumFromCamera(*renderingPipelineList._cameraData, frustum);

		std::vector<DUOLGraphicsEngine::RenderObject*> renderObjects;

		// 키 값으로 Culled object check.
		std::unordered_map<void*, bool> datas;

		quadtree->ViewFrustumCullingAllNodes(frustum, datas);

		// datas에 있는 RenderObject 만 최종적으로 보낸다 ..!
		for (auto renderObject : _renderObjectList)
		{
			// 스태틱 메쉬에 대해서만 컬링한다.
			if (renderObject->_mesh->GetMeshType() != DUOLGraphicsEngine::MeshBase::MeshType::Mesh)
			{
				culledRenderObject.push_back(renderObject);

				continue;
			}

			// 스태틱 메쉬인데 앞의 옥트리 컬링을 통과한 녀석만 넣어준다.
			if (datas.contains(renderObject))
			{
				culledRenderObject.push_back(renderObject);
			}
		}
	}

	void* GraphicsManager::GetGraphicsDevice()
	{
		return _graphicsEngine->GetModuleInfo()._device;
	}

	void* GraphicsManager::GetGraphicsDeviceContext()
	{
		return _graphicsEngine->GetModuleInfo()._deviceContext;
	}

	bool GraphicsManager::GetGraphicsRenderingResultInfo(DUOLGraphicsLibrary::QueryInfo& outData)
	{
		return _graphicsEngine->GetRenderData(outData);
	}

	void GraphicsManager::PrePresent()
	{
		_graphicsEngine->PrePresent();
	}

	void GraphicsManager::Present()
	{
		_graphicsEngine->Present();
	}

	void GraphicsManager::OnResize(std::any screenSize)
	{
		_screenSize = *std::any_cast<DUOLMath::Vector2*>(screenSize);

		// 그래픽스 엔진의 컨트롤은 'DUOLGameEngine::GraphicsManager' 에서 ..!
		_graphicsEngine->OnResize(_screenSize);

		SetScreenSize(_screenSize);

		// OnResize가 되면 UI도 전부 Size가 바껴야한다. 
		DUOLGameEngine::UIManager::GetInstance()->OnResize(_screenSize.x, _screenSize.y);
	}

	void GraphicsManager::PreExecute(const DUOLCommon::tstring& setupName)
	{
		// 해당 파이프라인 셋업에서 사용하는 모든 렌더 타겟들을 클리어합니다.
		if (_pipelineSetups.contains(setupName))
		{
			auto&& setup = _pipelineSetups.at(setupName);

			for (auto& pipeline : setup._opaquePipelines)
			{
				if (pipeline._procedureType == DUOLGraphicsEngine::RendererProcedureType::Pipeline)
				{
					auto&& renderPass = pipeline._procedure._procedurePipeline._renderingPipeline->GetRenderPass();

					if (renderPass->_depthStencilViewRef != nullptr)
						_graphicsEngine->ClearRenderTarget(renderPass->_depthStencilViewRef);

					for (auto& renderTarget : renderPass->_renderTargetViewRefs)
					{
						_graphicsEngine->ClearRenderTarget(renderTarget);
					}
				}
			}

			for (auto& pipeline : setup._transparencyPipelines)
			{
				if (pipeline._procedureType == DUOLGraphicsEngine::RendererProcedureType::Pipeline)
				{
					auto&& renderPass = pipeline._procedure._procedurePipeline._renderingPipeline->GetRenderPass();

					if (renderPass->_depthStencilViewRef != nullptr)
						_graphicsEngine->ClearRenderTarget(renderPass->_depthStencilViewRef);

					for (auto& renderTarget : renderPass->_renderTargetViewRefs)
					{
						_graphicsEngine->ClearRenderTarget(renderTarget);
					}
				}
			}
		}
	}

	void GraphicsManager::ClearRenderObjectList()
	{
		_renderObjectList.clear();
		_canvasList.clear();
	}

	DUOLGraphicsEngine::RenderingPipelinesList* GraphicsManager::GetRenderingPipelineList(
		const DUOLCommon::tstring& pipelineListName)
	{
		return _pipelineSetups.contains(pipelineListName) ? &_pipelineSetups.at(pipelineListName) : nullptr;
	}

	void GraphicsManager::RenderCurrentScene()
	{
		EventManager::GetInstance()->InvokeEvent(TEXT("SceneRendering"));
	}

	void GraphicsManager::UpdateCurrentSceneInfo(const DUOLMath::Vector2& screenSize)
	{
		_currentSceneInfo._screenSize[0] = static_cast<int>(screenSize.x);

		_currentSceneInfo._screenSize[1] = static_cast<int>(screenSize.y);

		_currentSceneInfo._timeStep = TimeManager::GetInstance()->GetDeltaTime();

		_currentSceneInfo._gamePlayTime = TimeManager::GetInstance()->GetRealtimeSinceStartup();

		//SSR의 MipSize갱신..
		const float maxSize = std::max<float>({ _screenSize.x, _screenSize.y });
		const auto log2Size = static_cast<unsigned>(std::log2(maxSize));

		_graphicsSetting._screenSpaceReflection.SetNumMips(1u + log2Size);
	}

	void GraphicsManager::UpdateCameraInfo(const DUOLGraphicsEngine::Camera* cameraInfo)
	{
		_cbPerCamera._camera = *cameraInfo;
	}

	void GraphicsManager::Execute(const DUOLCommon::tstring& setupName, bool cleanContext, bool clearRenderTarget)
	{
		if (clearRenderTarget)
			PreExecute(setupName);

		if (_pipelineSetups.contains(setupName))
		{
			auto&& setup = _pipelineSetups.at(setupName);

			//if (setup._skyBoxPipeline != nullptr)
			//	_graphicsEngine->Execute(_renderObjectList, setup._opaquePipelines, setup._skyBoxPipeline, setup._transparencyPipelines, _cbPerFrame, _canvasList);
			//else
			//	_graphicsEngine->Execute(_renderObjectList, setup._opaquePipelines, setup._transparencyPipelines, _cbPerFrame);
		}

		if (cleanContext)
		{
			ClearConstantBufferPerFrame();

			ClearRenderObjectList();
		}
	}

	void GraphicsManager::Execute(const std::vector<DUOLGraphicsEngine::RenderingPipelinesList>& renderingPipelineLists,
		bool cleanContext, bool clearRenderTarget)
	{
		// 프리 익서큐트.
		if (clearRenderTarget)
		{
			for (auto& renderingPipelineList : renderingPipelineLists)
			{
				PreExecute(renderingPipelineList._pipelineListName);
			}
		}

#pragma region OCTREE_CULLING
		/*	Octree* octree = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->_octree;

			std::vector<DUOLGraphicsEngine::RenderObject*> culledRenderObjects;

			OctreeCulling(renderingPipelineLists.back(), octree, culledRenderObjects);

			_graphicsEngine->Execute(culledRenderObjects, renderingPipelineLists, _canvasList, _currentSceneInfo);*/
#pragma endregion

#pragma region QUADTREE_CULLING
			/*Quadtree* quadtree = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->_quadtree;

			std::vector<DUOLGraphicsEngine::RenderObject*> culledRenderObjects;

			QuadtreeCulling(renderingPipelineLists.back(), quadtree, culledRenderObjects);

			_graphicsEngine->Execute(culledRenderObjects, renderingPipelineLists, _canvasList, _currentSceneInfo);*/
#pragma endregion

		_graphicsEngine->Execute(_renderObjectList, renderingPipelineLists, _canvasList, _currentSceneInfo);

		// 정리 옵션
		if (cleanContext)
		{
			ClearConstantBufferPerFrame();

			ClearRenderObjectList();
		}
	}

	void GraphicsManager::ClearRenderingPipelineSetups(const DUOLCommon::tstring& setupName)
	{
		PreExecute(setupName);
	}

	void GraphicsManager::CopyTexture(const DUOLCommon::tstring& destTextureID, const DUOLCommon::tstring& srcTextureID)
	{
		DUOLGraphicsLibrary::Texture* destTexture = _graphicsEngine->LoadTexture(destTextureID);
		DUOLGraphicsLibrary::Texture* srcTexture = _graphicsEngine->LoadTexture(srcTextureID);

		_graphicsEngine->CopyTexture(destTexture, srcTexture);
	}

	DUOLCommon::UUID GraphicsManager::FastPicking(const DUOLMath::Vector2& currentTextureSize, const DUOLMath::Vector2& pixel)
	{
		// 음수 픽셀은 받지 않습니다.
		if ((pixel.x <= 0 || pixel.y <= 0.f))
			return DUOLCommon::UUID{};

		const DUOLMath::Vector2 sizeRatio = _screenSize / currentTextureSize;

		uint64_t text = _graphicsEngine->FastPicking(pixel * sizeRatio);

		return DUOLCommon::UUID{ text };
	}

	void GraphicsManager::StartRenderingForGame()
	{
		_graphicsEngine->Begin();

		// 0. Clear all render target.
		ClearAllRenderTarget();

		// 1. SceneRender Event Invoke
		EventManager::GetInstance()->InvokeEvent(TEXT("SceneRendering"));

		// 2. SceneLighting Event Invoke
		EventManager::GetInstance()->InvokeEvent(TEXT("SceneLighting"));

		auto&& gameSetup = _pipelineSetups.at(TEXT("Game"));

		// 3. Camera Info (For game update)
		DUOLGameEngine::Camera* mainCam =
			DUOLGameEngine::Camera::GetMainCamera();

		if (mainCam == nullptr)
		{
			return;
		}

		_cbPerCamera._camera = mainCam->GetCameraInfo();

		// 4. Screen Size Info
		UpdateCurrentSceneInfo(_screenSize);

		// 5. Execute
		std::vector<DUOLGraphicsEngine::RenderingPipelinesList> renderPipelineLists;
		gameSetup._cameraData = &_cbPerCamera._camera;
		renderPipelineLists.push_back(gameSetup);

#pragma region OCTREE_CULLING
		/*Octree* octree = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->_octree;

		std::vector<DUOLGraphicsEngine::RenderObject*> culledRenderObjects;

		OctreeCulling(gameSetup, octree, culledRenderObjects);

		_graphicsEngine->Execute(culledRenderObjects, renderPipelineLists, _canvasList, _currentSceneInfo);*/
#pragma endregion

#pragma region QUADTREE_CULLING
		/* Quadtree* quadtree = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->_quadtree;

		std::vector<DUOLGraphicsEngine::RenderObject*> culledRenderObjects;

		QuadtreeCulling(gameSetup, quadtree, culledRenderObjects);

		_graphicsEngine->Execute(culledRenderObjects, renderPipelineLists, _canvasList, _currentSceneInfo);*/
#pragma endregion

		_graphicsEngine->Execute(_renderObjectList, renderPipelineLists, _canvasList, _currentSceneInfo);

		// 6. Clear constant buffer per frame. (light count ..)
		ClearConstantBufferPerFrame();

		// 7. Clear render object list. (== Clearing render queue.)
		ClearRenderObjectList();

		// Pre-Present game screen.
		PrePresent();

		// Present game screen.
		Present();

		_graphicsEngine->End();
	}

	void GraphicsManager::EndRenderingForGame()
	{
		// 나머지 정리 ..!
	}

	DUOLGraphicsLibrary::PipelineState* GraphicsManager::GetPipelineState(const DUOLCommon::tstring& objectID)
	{
		return _graphicsEngine->LoadPipelineState(objectID);
	}

	void GraphicsManager::SetGraphicSetting(GraphicsSetting& setting)
	{
		_graphicsSetting._exponentialHeightFog = *setting._exponentialHeightFog.get();
		_graphicsSetting._screenSpaceReflection = *setting._screenSpaceReflection.get();
		_graphicsSetting._toneMapping = *setting._toneMapping.get();
		_graphicsSetting._rimLight = *setting._rimLight.get();
		_graphicsSetting._bloom = *setting._bloom.get();
	}
}
