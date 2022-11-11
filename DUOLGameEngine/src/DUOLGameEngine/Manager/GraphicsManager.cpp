#include "DUOLGameEngine/Manager/GraphicsManager.h"

#include <algorithm>

#include "DUOLGameEngine/Engine.h"
#include "DUOLGameEngine/ECS/Component/Camera.h"


namespace DUOLGameEngine
{
	GraphicsManager::GraphicsManager()
	{
	}

	GraphicsManager::~GraphicsManager()
	{

	}

	void GraphicsManager::ReserveRenderObject(DUOLGraphicsEngine::RenderObject& renderObjectInfo)
	{
		_reservedRenderObjects.push_back(&renderObjectInfo);
	}

	void GraphicsManager::UpdateConstantBufferPerFrame()
	{
		// 1. Update Camera Information
		const std::shared_ptr<DUOLGameEngine::Camera> mainCam =
			DUOLGameEngine::Camera::GetMainCamera();

		if (mainCam != nullptr)
			_cbPerFrame._camera = mainCam->GetCameraInfo();

		// 2. Update Scene Light Information

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

		// Engine OnResize event handler register.
		std::function<void(const uint32_t&, const uint32_t&)> functor = 
			std::bind(&GraphicsManager::OnResize, this, std::placeholders::_1, std::placeholders::_2);

		Engine::GetInstance()->GetOnResizeEvent().AddListener(functor);
	}

	void GraphicsManager::UnInitialize()
	{
		_graphicsEngine.reset();
	}

	void GraphicsManager::Update(float deltaTime)
	{
		// 1. Mask, Layer 등에 따른 컬링, 영역 나누기 등 ..?!


		// 1 - 1. 컴포넌트들로부터 받은 이벤트들을 수행한다. (RendererBase의 Render 등 ..)
		OnRender();

		// 2. 그리라는 명령을 그래픽스 엔진 큐에 쌓는다.
		for (auto& object : _reservedRenderObjects)
		{
			_graphicsEngine->RenderObject(object);
		}

		// 2 - 1. Update ConstantBufferPerFrame
		UpdateConstantBufferPerFrame();

		// 3. Execute !
		_graphicsEngine->Execute(_cbPerFrame);

		// 4. Present !
		_graphicsEngine->Present();

		// 5. Vector Clear
		_reservedRenderObjects.clear();
	}

	void GraphicsManager::OnResize(const uint32_t& screenWidth, const uint32_t& screenHeight)
	{
		// - 그래픽스 엔진에게 해당 사실을 알리기.


		// - 어떻게하냐 카메라 컴포넌트는 대체 어디서 관리해야함 ?
		_onResizeEvent.Invoke(screenWidth, screenHeight);
	}

	void GraphicsManager::OnRender()
	{
		// - 더 할 수 있는게 있을까 ..?


		// - 등록된 RendererBase Component들에 대하여 이벤트 시작
		_onRenderEvent.Invoke();
	}
}
