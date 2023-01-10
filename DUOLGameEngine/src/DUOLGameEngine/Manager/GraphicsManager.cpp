#include "DUOLGameEngine/Manager/GraphicsManager.h"

#include <algorithm>

#include "DUOLGameEngine/Engine.h"
#include "DUOLGameEngine/ECS/Component/Camera.h"

#include "DUOLGraphicsEngine/GraphicsEngineFlags.h"
#include "DUOLGraphicsLibrary/Renderer/Texture.h"


namespace DUOLGameEngine
{
	GraphicsManager::GraphicsManager() :
		_renderingPipelineSetups({})
		, _cbPerFrames({})
	{
	}

	GraphicsManager::~GraphicsManager()
	{

	}

	DUOLGraphicsEngine::ConstantBufferPerFrame* GraphicsManager::GetConstantBufferPerFrame()
	{
		return &_cbPerFrames[0];
	}

	void GraphicsManager::ReserveRenderObject(DUOLGraphicsEngine::RenderObject& renderObjectInfo)
	{
		_reservedRenderObjects.push_back(&renderObjectInfo);
	}

	void GraphicsManager::ReserveRenderDebugObject(DUOLGraphicsEngine::RenderObject& renderObjectInfo)
	{
		_graphicsEngine->RenderDebugObject(&renderObjectInfo);
	}

	void GraphicsManager::UpdateConstantBufferPerFrame(int index, float deltaTime)
	{
		// 1. Update Camera Information
		const std::shared_ptr<DUOLGameEngine::Camera> mainCam =
			DUOLGameEngine::Camera::GetMainCamera();

		if (mainCam != nullptr)
			_cbPerFrames[index]._camera = mainCam->GetCameraInfo();

		_cbPerFrames[index]._screenSize[0] = _screenWidth;
		_cbPerFrames[index]._screenSize[1] = _screenHeight;
		_cbPerFrames[index]._timeStep = deltaTime;

		// 2. Update Scene Light Information
		// Light -> Light Component���� �˾Ƽ� ä���ֽ��ϴ�.
	}

	void GraphicsManager::ClearConstantBufferPerFrame(int index)
	{
		_cbPerFrames[index]._lightCount = 0;
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

		_screenWidth = gameSpecification.screenWidth;
		_screenHeight = gameSpecification.screenHeight;

		// GraphicsManager OnResize event handler register.
		const std::function<void(const uint32_t&, const uint32_t&)> functor = 
			std::bind(&GraphicsManager::OnResize, this, std::placeholders::_1, std::placeholders::_2);

		Engine::GetInstance()->GetResizeEvent().AddListener(functor);

		InitializeGraphicsPipelineSetups();
	}

	void GraphicsManager::UnInitialize()
	{
		_graphicsEngine.reset();
	}

	void GraphicsManager::PreUpdate()
	{
		_graphicsEngine->ClearRenderTargets();
	}

	void GraphicsManager::Update(float deltaTime)
	{
		// ��� ����Ÿ�ٵ��� Ŭ�����մϴ�.
		PreUpdate();

		// 1 - 1. ������Ʈ��κ��� ���� �̺�Ʈ �ڵ鷯���� ȣ���Ѵ�. (RendererBase�� Render �� ..)
		Render();

		for (size_t index = 0 ; index < _renderingPipelineSetups.size() ; index++)
		{
			auto&& setup = _renderingPipelineSetups[index];

			UpdateConstantBufferPerFrame(index, deltaTime);

			_graphicsEngine->Execute(_reservedRenderObjects, setup._opaquePipelines, setup._transparencyPipelines, _cbPerFrames[index]);

			ClearConstantBufferPerFrame(index);
		}

		_reservedRenderObjects.clear();
	}

	void GraphicsManager::InitializeGraphicsPipelineSetups()
	{
		static const TCHAR* defaultT =		(_T("Default"));
		static const TCHAR* deferred =	(_T("Lighting"));
		static const TCHAR* sceneView =		(_T("SceneView"));

		static const TCHAR* oit0 =		(_T("OIT0"));
		static const TCHAR* oit1 =		(_T("OIT1"));
		static const TCHAR* oit2 =		(_T("OIT2"));
		static const TCHAR* oit3 =		(_T("OIT3"));
		static const TCHAR* oit4 =		(_T("OIT4"));
		static const TCHAR* oit5 =		(_T("OIT5"));
		static const TCHAR* oitMerge0 = (_T("OITMerge0"));
		static const TCHAR* oitMerge1 = (_T("OITMerge1"));
		static const TCHAR* oitMerge2 = (_T("OITMerge2"));
		static const TCHAR* oitMerge3 = (_T("OITMerge3"));
		static const TCHAR* oitMerge4 = (_T("OITMerge4"));
		static const TCHAR* oitMerge5 = (_T("OITMerge5"));

		static const TCHAR* drawBackBuffer = (_T("DrawBackBuffer"));

#pragma region GAME_VIEW_SETUP
		// 1. Create Game View Pipeline set-up and _cbPerFrame.
		_renderingPipelineSetups.push_back({});

		_renderingPipelineSetups[0]._opaquePipelines.push_back(_graphicsEngine->LoadRenderingPipeline(defaultT));
		_renderingPipelineSetups[0]._opaquePipelines.push_back(_graphicsEngine->LoadRenderingPipeline(deferred));

		_renderingPipelineSetups[0]._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oit0));
		_renderingPipelineSetups[0]._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oit1));
		_renderingPipelineSetups[0]._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oit2));
		_renderingPipelineSetups[0]._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oit3));
		_renderingPipelineSetups[0]._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oit4));
		_renderingPipelineSetups[0]._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oit5));
		_renderingPipelineSetups[0]._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oitMerge0));
		_renderingPipelineSetups[0]._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oitMerge1));
		_renderingPipelineSetups[0]._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oitMerge2));
		_renderingPipelineSetups[0]._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oitMerge3));
		_renderingPipelineSetups[0]._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oitMerge4));
		_renderingPipelineSetups[0]._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oitMerge5));

		// TODO - �̰� ���߿� ����Ʈ ���μ��� ������ ������ ���� ���������.
		_renderingPipelineSetups[0]._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(sceneView));
		_renderingPipelineSetups[0]._transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(drawBackBuffer));

		_cbPerFrames.push_back({});

		_reservedRenderObjectLists.push_back({});
#pragma endregion

#pragma region SCENE_VIEW_SETUP

#pragma endregion

#pragma region MATERIAL_EDITTING_VIEW

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

	void GraphicsManager::OnResize(const uint32_t& screenWidth, const uint32_t& screenHeight)
	{
		// - Screen size update.
		_screenWidth = screenWidth;
		_screenHeight = screenHeight;

		// - �׷��Ƚ� �������� �ش� ����� �˸���.

		_graphicsEngine->OnReszie(DUOLMath::Vector2{static_cast<float>(screenWidth), static_cast<float>(screenHeight)});

		// - ����ϳ� ī�޶� ������Ʈ�� ��ü ��� �����ؾ��� ?
		_onResizeEvent.Invoke(screenWidth, screenHeight);
	}

	void GraphicsManager::Render()
	{
		// - �� �� ���� ������ ..?


		// - ��ϵ� RendererBase Component�鿡 ���Ͽ� �̺�Ʈ ����
		// => �ٵ� �̰� �� ���� ���Ƽ� �ع����� ���� �� ���Ͽ� ���� �� ���� ������ ..?
		_renderEventHandlers.Invoke();
	}

	DUOLCommon::EventListenerID GraphicsManager::AddRenderEventHandler(std::function<void()> functor)
	{
		return _renderEventHandlers += functor;
	}

	bool GraphicsManager::RemoveRenderEventHandler(DUOLCommon::EventListenerID id)
	{
		return _renderEventHandlers -= id;
	}
}
