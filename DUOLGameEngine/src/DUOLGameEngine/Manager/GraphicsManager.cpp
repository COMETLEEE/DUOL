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
	}

	void GraphicsManager::UnInitialize()
	{
		_graphicsEngine.reset();
	}

	void GraphicsManager::Update(float deltaTime)
	{
		// 1. Mask, Layer � ���� �ø�, ���� ������ �� ..?!

		// 1 - 1. ������Ʈ��κ��� ���� �̺�Ʈ �ڵ鷯���� ȣ���Ѵ�. (RendererBase�� Render �� ..)
		Render();

		// 2. �׸���� ����� �׷��Ƚ� ���� ť�� �״´�.
		for (auto& object : _reservedRenderObjects)
		{
			_graphicsEngine->RenderObject(object);
		}

		// 2 - 1. Update ConstantBufferPerFrame
		UpdateConstantBufferPerFrame(deltaTime);

		// 3. Execute ! => Execute ���� ī�޶� ���� ��ŭ (Scene view, Game view, ...)
		_graphicsEngine->Execute(_cbPerFrame);

		// 4. Reserved queue clear !
		_reservedRenderObjects.clear();

		// 5. constant buffer per frame clear.
		ClearConstantBufferPerFrame();
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
