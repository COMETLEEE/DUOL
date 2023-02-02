#include "DUOLGameEngine/Manager/GraphicsManager.h"

#include "DUOLGameEngine/Manager/EventManager.h"

#include <algorithm>
#include <spdlog/fmt/bundled/core.h>

#include "../../../../DUOLGraphicsEngine/src/DUOLGraphicsEngine/RenderManager/RenderingPipeline.h"
#include "DUOLGameEngine/Engine.h"
#include "DUOLGameEngine/ECS/Component/Camera.h"
#include "DUOLGameEngine/Manager/TimeManager.h"

#include "DUOLGraphicsEngine/GraphicsEngineFlags.h"
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

	void GraphicsManager::ReserveRenderObject(DUOLGraphicsEngine::RenderObject* renderObjectInfo)
	{
		_renderObjectList.push_back(renderObjectInfo);
	}

	void GraphicsManager::ReserveRenderDebugObject(DUOLGraphicsEngine::RenderObject* renderObjectInfo)
	{
		_graphicsEngine->RenderDebugObject(renderObjectInfo);
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

		_screenSize = DUOLMath::Vector2{ static_cast<float>(gameSpecification.screenWidth), static_cast<float>(gameSpecification.screenHeight) };

		// GraphicsManager OnResize event handler register. (������ ������ �Բ� ��� ������ �Ŵ����̹Ƿ� ID�� ���� �ʿ�� ���� ..)
		const std::function<void(std::any)> functor =
			std::bind(&GraphicsManager::OnResize, this, std::placeholders::_1);

		EventManager::GetInstance()->AddEventFunction(TEXT("Resize"), functor);

		// Pipeline setup�� ���õ� �ʱ�ȭ�� �����մϴ�.
		InitializeGraphicsPipelineSetups();
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

		static const TCHAR* drawBackBuffer = (_T("DrawBackBuffer"));

		static const TCHAR* idOutline = (_T("IDOutline"));
		static const TCHAR* outlineMerge = (_T("MergeOutline"));

		static const TCHAR* sceneViewGizmo = (_T("SceneView_Gizmo"));

		//������������ ��ī�̹ڽ��� Opaque�� Transparency ���̿� �׷���� �մϴ�..... �ٵ� �̰� ����ؿ�?
		static const TCHAR* skybox = _T("SkyBox");

#pragma region GAME_SETUP
		_pipelineSetups.insert({ TEXT("Game"), {} });

		auto&& gameSetup = _pipelineSetups.at(TEXT("Game"));

		gameSetup._opaquePipelines.push_back(_graphicsEngine->LoadRenderingPipeline(defaultT));
		gameSetup._opaquePipelines.push_back(_graphicsEngine->LoadRenderingPipeline(deferred));

		gameSetup._skyBoxPipeline = _graphicsEngine->LoadRenderingPipeline(skybox);

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

		// TODO - �̰� ���߿� ����Ʈ ���μ��� ������ ������ ���� ���������.
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(sceneView));
		gameSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(drawBackBuffer));
#pragma endregion

#pragma region GAME_VIEW_SETUP
		_pipelineSetups.insert({ TEXT("GameView"), {} });

		auto&& gameViewSetup = _pipelineSetups.at(TEXT("GameView"));

		gameViewSetup._opaquePipelines.push_back(_graphicsEngine->LoadRenderingPipeline(defaultT));
		gameViewSetup._opaquePipelines.push_back(_graphicsEngine->LoadRenderingPipeline(deferred));

		gameViewSetup._skyBoxPipeline = _graphicsEngine->LoadRenderingPipeline(skybox);

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

		// TODO - �̰� ���߿� ����Ʈ ���μ��� ������ ������ ���� ���������.
		gameViewSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(sceneView));
#pragma endregion

#pragma region SCENE_VIEW_SETUP
		_pipelineSetups.insert({ TEXT("Scene"), {} });

		auto&& sceneSetup = _pipelineSetups.at(TEXT("Scene"));

		// 2. ��� Scene View�� Game�� �׸��� �Ϳ��� ����� ������Ʈ�� �߰��ǰ�, ī�޶� �ɼ�, Size �ɼ��� �ٸ� ������
		// �⺻���� ���������� �н��� ����.
		sceneSetup._opaquePipelines.push_back(_graphicsEngine->LoadRenderingPipeline(defaultT));
		sceneSetup._opaquePipelines.push_back(_graphicsEngine->LoadRenderingPipeline(deferred));

		sceneSetup._skyBoxPipeline = _graphicsEngine->LoadRenderingPipeline(skybox);

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

		sceneSetup._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(sceneView));
#pragma endregion

#pragma region SCENE_VIEW_IDOUTLINE_SETUP
		_pipelineSetups.insert({ TEXT("IDOutline"), {} });

		auto&& idOutlineSetup = _pipelineSetups.at(TEXT("IDOutline"));

		// Outline�� �׸���.
		idOutlineSetup._opaquePipelines.push_back(_graphicsEngine->LoadRenderingPipeline(idOutline));

		idOutlineSetup._opaquePipelines.push_back(_graphicsEngine->LoadRenderingPipeline(outlineMerge));

		// �ܺο��� �˻��� ���� ������Ʈ ���۸� ��Ʈ���� �� �ֵ��� �����صӴϴ�.
		_renderingPipelineLayouts.insert({ TEXT("IDOutline"), &idOutlineSetup._opaquePipelines.front() });
#pragma endregion

#pragma region DEFAULT_SETUP
		_pipelineSetups.insert({ TEXT("Default") , {} });

		auto&& defaultSetup = _pipelineSetups.at(TEXT("Default"));

		defaultSetup._opaquePipelines.push_back(_graphicsEngine->LoadRenderingPipeline(defaultT));
#pragma endregion

#pragma region SCENE_VIEW_GIZMO_SETUP
		_pipelineSetups.insert({ TEXT("SceneView_Gizmo"), {} });

		auto&& sceneViewGizmoSetup = _pipelineSetups.at(TEXT("SceneView_Gizmo"));

		sceneViewGizmoSetup._opaquePipelines.push_back(_graphicsEngine->LoadRenderingPipeline(sceneViewGizmo));
#pragma endregion
	}

	void* GraphicsManager::GetGraphicsDevice()
	{
		return _graphicsEngine->GetModuleInfo()._device;
	}

	void* GraphicsManager::GetGraphicsDeviceContext()
	{
		return _graphicsEngine->GetModuleInfo()._deviceContext;
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

		// �׷��Ƚ� ������ ��Ʈ���� 'DUOLGameEngine::GraphicsManager' ���� ..!
		_graphicsEngine->OnResize(_screenSize);
	}

	void GraphicsManager::PreExecute(const DUOLCommon::tstring& setupName)
	{
		// �ش� ���������� �¾����� ����ϴ� ��� ���� Ÿ�ٵ��� Ŭ�����մϴ�.
		if (_pipelineSetups.contains(setupName))
		{
			auto&& setup = _pipelineSetups.at(setupName);

			for (auto& opaquePipeline : setup._opaquePipelines)
			{
				auto&& renderPass = opaquePipeline._renderingPipeline->GetRenderPass();

				_graphicsEngine->ClearRenderTarget(renderPass->_depthStencilViewRef);

				for (auto& renderTarget : renderPass->_renderTargetViewRefs)
				{
					_graphicsEngine->ClearRenderTarget(renderTarget);
				}
			}

			for (auto& pipeline : setup._transparencyPipelines)
			{
				auto&& renderPass = pipeline._renderingPipeline->GetRenderPass();

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
		_cbPerFrame._camera = *cameraInfo;
	}

	void GraphicsManager::Execute(const DUOLCommon::tstring& setupName, bool cleanContext, bool clearRenderTarget)
	{
		if (clearRenderTarget)
			PreExecute(setupName);

		if (_pipelineSetups.contains(setupName))
		{
			auto&& setup = _pipelineSetups.at(setupName);

			if (setup._skyBoxPipeline != nullptr)
				_graphicsEngine->Execute(_renderObjectList, setup._opaquePipelines, setup._skyBoxPipeline, setup._transparencyPipelines, _cbPerFrame);
			else
				_graphicsEngine->Execute(_renderObjectList, setup._opaquePipelines, setup._transparencyPipelines, _cbPerFrame);
		}

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

	DUOLGameEngine::UUID GraphicsManager::FastPicking(const DUOLMath::Vector2& currentTextureSize, const DUOLMath::Vector2& pixel)
	{
		// ���� �ȼ��� ���� �ʽ��ϴ�.
		if ((pixel.x <= 0 || pixel.y <= 0.f))
			return UUID{};

		const DUOLMath::Vector2 sizeRatio = _screenSize / currentTextureSize;

		uint64_t text = _graphicsEngine->FastPicking(pixel * sizeRatio);

		return UUID{ text };
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
		const std::shared_ptr<DUOLGameEngine::Camera> mainCam =
			DUOLGameEngine::Camera::GetMainCamera();

		if (mainCam != nullptr)
			_cbPerFrame._camera = mainCam->GetCameraInfo();

		// 4. Screen Size Info
		UpdateRenderScreenSize(_screenSize);
		_cbPerFrame._timeStep = TimeManager::GetInstance()->GetDeltaTime();
		_cbPerFrame._gamePlayTime = TimeManager::GetInstance()->GetRealtimeSinceStartup();

		// 5. Execute
		//_graphicsEngine->Execute(_renderObjectList,
		//	gameSetup._opaquePipelines, gameSetup._transparencyPipelines, _cbPerFrame);
		_graphicsEngine->Execute(_renderObjectList, gameSetup._opaquePipelines, gameSetup._skyBoxPipeline, gameSetup._transparencyPipelines, _cbPerFrame);

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