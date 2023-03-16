#include "DUOLGameEngine/Manager/GraphicsManager.h"

#include "DUOLGameEngine/Manager/EventManager.h"

#include <algorithm>
#include <spdlog/fmt/bundled/core.h>

#include "DUOLGraphicsEngine/RenderManager/RenderingPipeline/RenderingPipeline.h"
#include "DUOLCommon/Log/LogHelper.h"
#include "DUOLGameEngine/Engine.h"
#include "DUOLGameEngine/ECS/Component/Camera.h"
#include "DUOLGameEngine/Manager/TimeManager.h"

#include "DUOLGraphicsEngine/GraphicsEngineFlags.h"
#include "DUOLGraphicsLibrary/Renderer/RenderTarget.h"
#include "DUOLGraphicsLibrary/Renderer/Texture.h"


namespace DUOLGameEngine
{
	GraphicsManager::GraphicsManager() :
		_renderingPipelineSetups({})
		, _cbPerFrame({})
	{
	}

	GraphicsManager::~GraphicsManager()
	{
	}

	DUOLGraphicsEngine::ConstantBufferPerFrame* GraphicsManager::GetConstantBufferPerFrame()
	{
		return &_cbPerFrame;
	}

	BloomScreenSize* GraphicsManager::GetConstantBufferScreenSize(int idx)
	{
		return &_bloomScreenSize[idx];
	}

	temptonemapping* GraphicsManager::GetToneMappingExposure()
	{
		return &_cbToneMappingExposure;
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
		_cbPerFrame._lightCount = 0;
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
			DUOLMath::Vector2(static_cast<float>(gameSpecification.screenWidth),
				static_cast<float>(gameSpecification.screenHeight));

		graphicsEngineDesc._handle = reinterpret_cast<long>(gameSpecification.hWnd);
		graphicsEngineDesc._moduleType = DUOLGraphicsEngine::RendererModuleType::DIRECTX11;

		_graphicsEngine =
			std::make_shared<DUOLGraphicsEngine::GraphicsEngine>(graphicsEngineDesc);

		if(_graphicsEngine== nullptr)
		{
			DUOL_CRITICAL("GraphicsEngine Nullptr");
		}

		_screenSize = DUOLMath::Vector2{ static_cast<float>(gameSpecification.screenWidth), static_cast<float>(gameSpecification.screenHeight) };

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

		static const TCHAR* oit0 = (_T("OIT0"));
		static const TCHAR* oit1 = (_T("OIT1"));
		static const TCHAR* oit2 = (_T("OIT2"));
		static const TCHAR* oit3 = (_T("OIT3"));
		static const TCHAR* oit4 = (_T("OIT4"));
		static const TCHAR* oit5 = (_T("OIT5"));
		static const TCHAR* oitMerge0 = (_T("OITMerge0"));
		static const TCHAR* oitMerge1 = (_T("OITMerge1"));
		static const TCHAR* oitMerge2 = (_T("OITMerge2"));
		static const TCHAR* oitMerge3 = (_T("OITMerge3"));
		static const TCHAR* oitMerge4 = (_T("OITMerge4"));
		static const TCHAR* oitMerge5 = (_T("OITMerge5"));

		static const TCHAR* idOutline = (_T("IDOutline"));
		static const TCHAR* outlineMerge = (_T("MergeOutline"));

		static const TCHAR* sceneViewGizmo = (_T("SceneView_Gizmo"));

		static const TCHAR* bloomSampling= (_T("BloomSampling"));

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
		
		//무조건적으로 스카이박스는 Opaque와 Transparency 사이에 그려줘야 합니다..... 근데 이거 어떻게해요?
		static const TCHAR* skybox = _T("SkyBox");

#pragma region GAME_SETUP
		_pipelineSetups.insert({ TEXT("Game"), {} });

		auto&& gameSetup = _pipelineSetups.at(TEXT("Game"));

		gameSetup._pipelineListName = TEXT("Game");

		gameSetup._opaquePipelines.push_back(_graphicsEngine->LoadRenderingPipeline(defaultT));
		gameSetup._opaquePipelines.push_back(_graphicsEngine->LoadRenderingPipeline(deferred));

		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oit0));
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oit1));
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oit2));
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oit3));
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oit4));
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oit5));
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oitMerge0));
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oitMerge1));
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oitMerge2));
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oitMerge3));
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oitMerge4));
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oitMerge5));

#pragma region Bloom
		{
		// Bloom curve
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(bloomSampling));

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
		BloomScreenSizeSet(128, 0);
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur128X));
		gameSetup._transparencyPipelines.back()._perObjectBufferData = GetConstantBufferScreenSize(0);
		gameSetup._transparencyPipelines.back()._dataSize = 16;

		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur128Y));
		gameSetup._transparencyPipelines.back()._perObjectBufferData = GetConstantBufferScreenSize(0);
		gameSetup._transparencyPipelines.back()._dataSize = 16;

		// Blur 128 + DownSampling 64
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(upblur64));

		// Blur_64
		BloomScreenSizeSet(64, 1);
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur64X));
		gameSetup._transparencyPipelines.back()._perObjectBufferData = GetConstantBufferScreenSize(1);
		gameSetup._transparencyPipelines.back()._dataSize = 16;

		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur64Y));
		gameSetup._transparencyPipelines.back()._perObjectBufferData = GetConstantBufferScreenSize(1);
		gameSetup._transparencyPipelines.back()._dataSize = 16;

		// Blur 64 + DownSampling 32
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(upblur32));

		// Blur_32
		BloomScreenSizeSet(32, 2);
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur32X));
		gameSetup._transparencyPipelines.back()._perObjectBufferData = GetConstantBufferScreenSize(2);
		gameSetup._transparencyPipelines.back()._dataSize = 16;

		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur32Y));
		gameSetup._transparencyPipelines.back()._perObjectBufferData = GetConstantBufferScreenSize(2);
		gameSetup._transparencyPipelines.back()._dataSize = 16;

		// Blur 32 + DownSampling 16
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(upblur16));

		// Blur_16
		BloomScreenSizeSet(16, 3);
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur16X));
		gameSetup._transparencyPipelines.back()._perObjectBufferData = GetConstantBufferScreenSize(3);
		gameSetup._transparencyPipelines.back()._dataSize = 16;

		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur16Y));
		gameSetup._transparencyPipelines.back()._perObjectBufferData = GetConstantBufferScreenSize(3);
		gameSetup._transparencyPipelines.back()._dataSize = 16;

		// Blur 8 + DownSampling 4
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(upblur8));

		// Blur_8
		BloomScreenSizeSet(8, 4);
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur8X));
		gameSetup._transparencyPipelines.back()._perObjectBufferData = GetConstantBufferScreenSize(4);
		gameSetup._transparencyPipelines.back()._dataSize = 16;

		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur8Y));
		gameSetup._transparencyPipelines.back()._perObjectBufferData = GetConstantBufferScreenSize(4);
		gameSetup._transparencyPipelines.back()._dataSize = 16;


		// Blur 8 + DownSampling 4
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(upblur4));

		BloomScreenSizeSet(4, 5);
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur4X));
		gameSetup._transparencyPipelines.back()._perObjectBufferData = GetConstantBufferScreenSize(5);
		gameSetup._transparencyPipelines.back()._dataSize = 16;

		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur4Y));
		gameSetup._transparencyPipelines.back()._perObjectBufferData = GetConstantBufferScreenSize(5);
		gameSetup._transparencyPipelines.back()._dataSize = 16;
	
		// Blur 4 + DownSampling 2
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(upblur2));

		//// Blur_2
		BloomScreenSizeSet(2, 6);
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur2X));
		gameSetup._transparencyPipelines.back()._perObjectBufferData = GetConstantBufferScreenSize(6);
		gameSetup._transparencyPipelines.back()._dataSize = 16;

		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur2Y));
		gameSetup._transparencyPipelines.back()._perObjectBufferData = GetConstantBufferScreenSize(6);
		gameSetup._transparencyPipelines.back()._dataSize = 16;

		// Blur 2 + Origin
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(upblurorigin));

		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(bloom));

		// tone mapping
		ToneMappingExposureSet(1.5f);
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(tonemapping));
		gameSetup._transparencyPipelines.back()._perObjectBufferData = GetToneMappingExposure();
		gameSetup._transparencyPipelines.back()._dataSize = 16;
		}
#pragma endregion

		// TODO - 이거 나중에 포스트 프로세싱 파이프 라인은 따로 나누어야함.
		//gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(sceneView));
		gameSetup._drawGameViewToBackBuffer = true;

#pragma endregion

#pragma region GAME_VIEW_SETUP
		_pipelineSetups.insert({ TEXT("GameView"), {} });

		auto&& gameViewSetup = _pipelineSetups.at(TEXT("GameView"));

		gameViewSetup._pipelineListName = TEXT("GameView");

		gameViewSetup._opaquePipelines.push_back(_graphicsEngine->LoadRenderingPipeline(defaultT));
		gameViewSetup._opaquePipelines.push_back(_graphicsEngine->LoadRenderingPipeline(deferred));

		gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oit0));
		gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oit1));
		gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oit2));
		gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oit3));
		gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oit4));
		gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oit5));
		gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oitMerge0));
		gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oitMerge1));
		gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oitMerge2));
		gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oitMerge3));
		gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oitMerge4));
		gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oitMerge5));

		// TODO - 이거 나중에 포스트 프로세싱 파이프 라인은 따로 나누어야함.
		gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(sceneView));

#pragma region Bloom
		{
			// Bloom curve
			gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(bloomSampling));

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
			BloomScreenSizeSet(128, 0);
			gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur128X));
			gameViewSetup._transparencyPipelines.back()._perObjectBufferData = GetConstantBufferScreenSize(0);
			gameViewSetup._transparencyPipelines.back()._dataSize = 16;

			gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur128Y));
			gameViewSetup._transparencyPipelines.back()._perObjectBufferData = GetConstantBufferScreenSize(0);
			gameViewSetup._transparencyPipelines.back()._dataSize = 16;

			// Blur 128 + DownSampling 64
			gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(upblur64));

			// Blur_64
			BloomScreenSizeSet(64, 1);
			gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur64X));
			gameViewSetup._transparencyPipelines.back()._perObjectBufferData = GetConstantBufferScreenSize(1);
			gameViewSetup._transparencyPipelines.back()._dataSize = 16;

			gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur64Y));
			gameViewSetup._transparencyPipelines.back()._perObjectBufferData = GetConstantBufferScreenSize(1);
			gameViewSetup._transparencyPipelines.back()._dataSize = 16;

			// Blur 64 + DownSampling 32
			gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(upblur32));

			// Blur_32
			BloomScreenSizeSet(32, 2);
			gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur32X));
			gameViewSetup._transparencyPipelines.back()._perObjectBufferData = GetConstantBufferScreenSize(2);
			gameViewSetup._transparencyPipelines.back()._dataSize = 16;

			gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur32Y));
			gameViewSetup._transparencyPipelines.back()._perObjectBufferData = GetConstantBufferScreenSize(2);
			gameViewSetup._transparencyPipelines.back()._dataSize = 16;

			// Blur 32 + DownSampling 16
			gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(upblur16));

			// Blur_16
			BloomScreenSizeSet(16, 3);
			gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur16X));
			gameViewSetup._transparencyPipelines.back()._perObjectBufferData = GetConstantBufferScreenSize(3);
			gameViewSetup._transparencyPipelines.back()._dataSize = 16;

			gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur16Y));
			gameViewSetup._transparencyPipelines.back()._perObjectBufferData = GetConstantBufferScreenSize(3);
			gameViewSetup._transparencyPipelines.back()._dataSize = 16;

			// Blur 8 + DownSampling 4
			gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(upblur8));

			// Blur_8
			BloomScreenSizeSet(8, 4);
			gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur8X));
			gameViewSetup._transparencyPipelines.back()._perObjectBufferData = GetConstantBufferScreenSize(4);
			gameViewSetup._transparencyPipelines.back()._dataSize = 16;

			gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur8Y));
			gameViewSetup._transparencyPipelines.back()._perObjectBufferData = GetConstantBufferScreenSize(4);
			gameViewSetup._transparencyPipelines.back()._dataSize = 16;


			// Blur 8 + DownSampling 4
			gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(upblur4));

			BloomScreenSizeSet(4, 5);
			gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur4X));
			gameViewSetup._transparencyPipelines.back()._perObjectBufferData = GetConstantBufferScreenSize(5);
			gameViewSetup._transparencyPipelines.back()._dataSize = 16;

			gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur4Y));
			gameViewSetup._transparencyPipelines.back()._perObjectBufferData = GetConstantBufferScreenSize(5);
			gameViewSetup._transparencyPipelines.back()._dataSize = 16;

			// Blur 4 + DownSampling 2
			gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(upblur2));

			//// Blur_2
			BloomScreenSizeSet(2, 6);
			gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur2X));
			gameViewSetup._transparencyPipelines.back()._perObjectBufferData = GetConstantBufferScreenSize(6);
			gameViewSetup._transparencyPipelines.back()._dataSize = 16;

			gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur2Y));
			gameViewSetup._transparencyPipelines.back()._perObjectBufferData = GetConstantBufferScreenSize(6);
			gameViewSetup._transparencyPipelines.back()._dataSize = 16;

			// Blur 2 + Origin
			gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(upblurorigin));

			gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(bloom));

			// tone mapping
			ToneMappingExposureSet(1.5f);
			gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(tonemapping));
			gameViewSetup._transparencyPipelines.back()._perObjectBufferData = GetToneMappingExposure();
			gameViewSetup._transparencyPipelines.back()._dataSize = 16;
		}
#pragma endregion

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

		sceneSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oit0));
		sceneSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oit1));
		sceneSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oit2));
		sceneSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oit3));
		sceneSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oit4));
		sceneSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oit5));
		sceneSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oitMerge0));
		sceneSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oitMerge1));
		sceneSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oitMerge2));
		sceneSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oitMerge3));
		sceneSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oitMerge4));
		sceneSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oitMerge5));

#pragma region Bloom
		{
			// Bloom curve
			sceneSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(bloomSampling));

			// DownSampling_2
			sceneSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(downSampling2));

			// DownSampling_4
			sceneSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(downSampling4));

			//DownSampling x8
			sceneSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(downSampling8));

			// DownSampling_16
			sceneSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(downSampling16));

			// DownSampling_32
			sceneSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(downSampling32));

			// DownSampling_64
			sceneSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(downSampling64));

			//DownSampling x128
			sceneSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(downSampling128));

			// Blur_128
			BloomScreenSizeSet(128, 0);
			sceneSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur128X));
			sceneSetup._transparencyPipelines.back()._perObjectBufferData = GetConstantBufferScreenSize(0);
			sceneSetup._transparencyPipelines.back()._dataSize = 16;

			sceneSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur128Y));
			sceneSetup._transparencyPipelines.back()._perObjectBufferData = GetConstantBufferScreenSize(0);
			sceneSetup._transparencyPipelines.back()._dataSize = 16;

			// Blur 128 + DownSampling 64
			sceneSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(upblur64));

			// Blur_64
			BloomScreenSizeSet(64, 1);
			sceneSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur64X));
			sceneSetup._transparencyPipelines.back()._perObjectBufferData = GetConstantBufferScreenSize(1);
			sceneSetup._transparencyPipelines.back()._dataSize = 16;

			sceneSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur64Y));
			sceneSetup._transparencyPipelines.back()._perObjectBufferData = GetConstantBufferScreenSize(1);
			sceneSetup._transparencyPipelines.back()._dataSize = 16;

			// Blur 64 + DownSampling 32
			sceneSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(upblur32));

			// Blur_32
			BloomScreenSizeSet(32, 2);
			sceneSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur32X));
			sceneSetup._transparencyPipelines.back()._perObjectBufferData = GetConstantBufferScreenSize(2);
			sceneSetup._transparencyPipelines.back()._dataSize = 16;

			sceneSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur32Y));
			sceneSetup._transparencyPipelines.back()._perObjectBufferData = GetConstantBufferScreenSize(2);
			sceneSetup._transparencyPipelines.back()._dataSize = 16;

			// Blur 32 + DownSampling 16
			sceneSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(upblur16));

			// Blur_16
			BloomScreenSizeSet(16, 3);
			sceneSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur16X));
			sceneSetup._transparencyPipelines.back()._perObjectBufferData = GetConstantBufferScreenSize(3);
			sceneSetup._transparencyPipelines.back()._dataSize = 16;

			sceneSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur16Y));
			sceneSetup._transparencyPipelines.back()._perObjectBufferData = GetConstantBufferScreenSize(3);
			sceneSetup._transparencyPipelines.back()._dataSize = 16;

			// Blur 8 + DownSampling 4
			sceneSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(upblur8));

			// Blur_8
			BloomScreenSizeSet(8, 4);
			sceneSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur8X));
			sceneSetup._transparencyPipelines.back()._perObjectBufferData = GetConstantBufferScreenSize(4);
			sceneSetup._transparencyPipelines.back()._dataSize = 16;

			sceneSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur8Y));
			sceneSetup._transparencyPipelines.back()._perObjectBufferData = GetConstantBufferScreenSize(4);
			sceneSetup._transparencyPipelines.back()._dataSize = 16;


			// Blur 8 + DownSampling 4
			sceneSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(upblur4));

			BloomScreenSizeSet(4, 5);
			sceneSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur4X));
			sceneSetup._transparencyPipelines.back()._perObjectBufferData = GetConstantBufferScreenSize(5);
			sceneSetup._transparencyPipelines.back()._dataSize = 16;

			sceneSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur4Y));
			sceneSetup._transparencyPipelines.back()._perObjectBufferData = GetConstantBufferScreenSize(5);
			sceneSetup._transparencyPipelines.back()._dataSize = 16;

			// Blur 4 + DownSampling 2
			sceneSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(upblur2));

			//// Blur_2
			BloomScreenSizeSet(2, 6);
			sceneSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur2X));
			sceneSetup._transparencyPipelines.back()._perObjectBufferData = GetConstantBufferScreenSize(6);
			sceneSetup._transparencyPipelines.back()._dataSize = 16;

			sceneSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(blur2Y));
			sceneSetup._transparencyPipelines.back()._perObjectBufferData = GetConstantBufferScreenSize(6);
			sceneSetup._transparencyPipelines.back()._dataSize = 16;

			// Blur 2 + Origin
			sceneSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(upblurorigin));

			sceneSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(bloom));

			// tone mapping
			ToneMappingExposureSet(1.5f);
			sceneSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(tonemapping));
			sceneSetup._transparencyPipelines.back()._perObjectBufferData = GetToneMappingExposure();
			sceneSetup._transparencyPipelines.back()._dataSize = 16;
		}
#pragma endregion


		sceneSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(sceneView_Sample));
		sceneSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(sceneView));
#pragma endregion

#pragma region SCENE_VIEW_IDOUTLINE_SETUP
		_pipelineSetups.insert({ TEXT("IDOutline"), {} });

		auto&& idOutlineSetup = _pipelineSetups.at(TEXT("IDOutline"));

		idOutlineSetup._pipelineListName = TEXT("IDOutline");

		// Outline을 그린다.
		idOutlineSetup._opaquePipelines.push_back(_graphicsEngine->LoadRenderingPipeline(idOutline));

		idOutlineSetup._opaquePipelines.push_back(_graphicsEngine->LoadRenderingPipeline(outlineMerge));

		// 외부에서 검색을 통해 컨스턴트 버퍼를 컨트롤할 수 있도록 매핑해둡니다.
		_renderingPipelineLayouts.insert({ TEXT("IDOutline"), &idOutlineSetup._opaquePipelines.front() });
#pragma endregion

#pragma region DEFAULT_SETUP
		_pipelineSetups.insert({ TEXT("Default") , {} });

		auto&& defaultSetup = _pipelineSetups.at(TEXT("Default"));

		defaultSetup._pipelineListName = TEXT("Default");

		defaultSetup._opaquePipelines.push_back(_graphicsEngine->LoadRenderingPipeline(defaultT));
		defaultSetup._drawSkybox = false;
#pragma endregion

#pragma region SCENE_VIEW_GIZMO_SETUP
		_pipelineSetups.insert({ TEXT("SceneView_Gizmo"), {} });

		auto&& sceneViewGizmoSetup = _pipelineSetups.at(TEXT("SceneView_Gizmo"));

		sceneViewGizmoSetup._pipelineListName = TEXT("SceneView_Gizmo");

		sceneViewGizmoSetup._opaquePipelines.push_back(_graphicsEngine->LoadRenderingPipeline(sceneViewGizmo));
#pragma endregion
	}

	void GraphicsManager::BloomScreenSizeSet(int divide, int bloomIdx)
	{
		_bloomScreenSize[bloomIdx]._screenSize.x = 0.f;
		_bloomScreenSize[bloomIdx]._screenSize.y = 0.f;
		_bloomScreenSize[bloomIdx]._screenSize.z = ceil(GetScreenSize().x / divide);
		_bloomScreenSize[bloomIdx]._screenSize.w = ceil(GetScreenSize().x / divide);
	}

	void GraphicsManager::ToneMappingExposureSet(float expoureset)
	{
		_cbToneMappingExposure.exposure = expoureset;
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
	}

	void GraphicsManager::PreExecute(const DUOLCommon::tstring& setupName)
	{
		// 해당 파이프라인 셋업에서 사용하는 모든 렌더 타겟들을 클리어합니다.
		if (_pipelineSetups.contains(setupName))
		{
			auto&& setup = _pipelineSetups.at(setupName);

			for (auto& opaquePipeline : setup._opaquePipelines)
			{
				auto&& renderPass = opaquePipeline._renderingPipeline->GetRenderPass();

				if (renderPass->_depthStencilViewRef != nullptr)
					_graphicsEngine->ClearRenderTarget(renderPass->_depthStencilViewRef);

				for (auto& renderTarget : renderPass->_renderTargetViewRefs)
				{
					_graphicsEngine->ClearRenderTarget(renderTarget);
				}
			}

			for (auto& pipeline : setup._transparencyPipelines)
			{
				auto&& renderPass = pipeline._renderingPipeline->GetRenderPass();
				if(renderPass->_depthStencilViewRef != nullptr)
					_graphicsEngine->ClearRenderTarget(renderPass->_depthStencilViewRef);

				for (auto& renderTarget : renderPass->_renderTargetViewRefs)
				{
					_graphicsEngine->ClearRenderTarget(renderTarget);
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

	void GraphicsManager::UpdateRenderScreenSize(const DUOLMath::Vector2& screenSize)
	{
		_cbPerFrame._screenSize[0] = static_cast<int>(screenSize.x);

		_cbPerFrame._screenSize[1] = static_cast<int>(screenSize.y);
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

		// 익서큐트합니다.
		_graphicsEngine->Execute(_renderObjectList, renderingPipelineLists, _canvasList, _cbPerFrame);

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

		if (mainCam != nullptr)
			_cbPerCamera._camera = mainCam->GetCameraInfo();

		// 4. Screen Size Info
		UpdateRenderScreenSize(_screenSize);
		_cbPerFrame._timeStep = TimeManager::GetInstance()->GetDeltaTime();
		_cbPerFrame._gamePlayTime = TimeManager::GetInstance()->GetRealtimeSinceStartup();

		// 5. Execute
		std::vector<DUOLGraphicsEngine::RenderingPipelinesList> renderPipelineLists;
		gameSetup._cameraData = &_cbPerCamera._camera;
		renderPipelineLists.push_back(gameSetup);

		_graphicsEngine->Execute(_renderObjectList, renderPipelineLists, _canvasList, _cbPerFrame);


		//	gameSetup._opaquePipelines, gameSetup._transparencyPipelines, _cbPerFrame);
		//_graphicsEngine->Execute(_renderObjectList, gameSetup._opaquePipelines, gameSetup._skyBoxPipeline, gameSetup._transparencyPipelines, _cbPerFrame, _canvasList);

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

	}

	DUOLGraphicsLibrary::PipelineState* GraphicsManager::GetPipelineState(const DUOLCommon::tstring& objectID)
	{
		return _graphicsEngine->LoadPipelineState(objectID);
	}
}
