#include "DUOLGameEngine/Manager/GraphicsManager.h"

#include <algorithm>

#include "DUOLGameEngine/Engine.h"
#include "DUOLGameEngine/ECS/Component/Camera.h"
#include "DUOLGraphicsEngine/GraphicsEngineFlags.h"


namespace DUOLGameEngine
{
	GraphicsManager::GraphicsManager()
	{
	}

	GraphicsManager::~GraphicsManager()
	{

	}

	DUOLGraphicsEngine::ConstantBufferPerFrame* GraphicsManager::GetConstantBufferPerFrame()
	{
		return &_cbPerFrame;
	}

	void GraphicsManager::ReserveRenderObject(DUOLGraphicsEngine::RenderObject& renderObjectInfo)
	{
		_reservedRenderObjects.push_back(&renderObjectInfo);
	}

	void GraphicsManager::ReserveRenderDebugObject(DUOLGraphicsEngine::RenderObject& renderObjectInfo)
	{
		_graphicsEngine->RenderDebugObject(&renderObjectInfo);
	}

	void GraphicsManager::UpdateConstantBufferPerFrame(float deltaTime)
	{
		// 1. Update Camera Information
		const std::shared_ptr<DUOLGameEngine::Camera> mainCam =
			DUOLGameEngine::Camera::GetMainCamera();

		if (mainCam != nullptr)
			_cbPerFrame._camera = mainCam->GetCameraInfo();

		_cbPerFrame._screenSize[0] = _screenWidth;
		_cbPerFrame._screenSize[1] = _screenHeight;
		_cbPerFrame._timeStep = deltaTime;

		// _cbPerFrame._light[0].

		// 2. Update Scene Light Information
		

	}

	void GraphicsManager::ClearConstantBufferPerFrame()
	{
		_cbPerFrame._lightCount = 0;
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

		GraphicsPipelineSetUp();
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

		// 2 - 1. Update ConstantBufferPerFrame
		UpdateConstantBufferPerFrame(deltaTime);

		_graphicsEngine->Execute(_reservedRenderObjects, _opaquePipelines, _transparencyPipelines, _cbPerFrame);

		// 4. Reserved queue clear !
		_reservedRenderObjects.clear();

		// 5. constant buffer per frame clear.
		ClearConstantBufferPerFrame();
	}

	void GraphicsManager::GraphicsPipelineSetUp()
	{
		static const TCHAR* id =		(_T("Default"));
		static const TCHAR* deferred =	(_T("Lighting"));
		static const TCHAR* merge =		(_T("Merge"));
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

		_opaquePipelines.push_back(_graphicsEngine->LoadRenderingPipeline(id));
		_opaquePipelines.push_back(_graphicsEngine->LoadRenderingPipeline(deferred));

		_transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oit0));
		_transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oit1));
		_transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oit2));
		_transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oit3));
		_transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oit4));
		_transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oit5));
		_transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oitMerge0));
		_transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oitMerge1));
		_transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oitMerge2));
		_transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oitMerge3));
		_transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oitMerge4));
		_transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(oitMerge5));

		// TODO - �̰� ���߿� ����Ʈ ���μ��� ������ ������ ���� ���������.
		_transparencyPipelines.push_back(_graphicsEngine->LoadRenderingPipeline(merge));
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
