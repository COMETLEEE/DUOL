#include "DUOLEditor/Modules/SceneView.h"
#include "DUOLEditor/UI/Widgets/Display/Image.h"

#include "DUOLGameEngine/ECS/Component/Camera.h"

#include "DUOLGameEngine/Manager/GraphicsManager.h"
#include "DUOLGameEngine/Manager/EventManager.h"
#include "DUOLGameEngine/Manager/InputManager.h"


namespace DUOLEditor
{
	SceneView::SceneView(const DUOLCommon::tstring& title, bool isOpened, const PanelWindowSetting& windowSetting) :
		ControllableViewBase(title, isOpened, windowSetting)
	{

	}

	SceneView::~SceneView()
	{

	}

	void SceneView::Update(float deltaTime)
	{
		ControllableViewBase::Update(deltaTime);

#pragma region 이제 그냥 렌더타겟 전부 나눠버렸다. 복사할 필요 없다 ..!
		// 0. 이전에 Game View (== Game) 가 그려졌다. 복사해놓자
		DUOLGameEngine::GraphicsManager::GetInstance()->CopyTexture(TEXT("CopyGameView"), TEXT("GameView"));
#pragma endregion

		// 1. SceneView의 이미지가 호출해야할 텍스처를 업데이트합니다.
		_image->_size = GetSafeSize();

#pragma region GameView를 그릴 때 Context를 초기화하지 않았기 때문에 Rendering, Lighting 등의 이벤트를 한 번 더 일으킬 필요가 없습니다 ..!
		// 1. SceneRender Event Invoke
		// DUOLGameEngine::EventManager::GetInstance()->InvokeEvent(TEXT("SceneRendering"));

		// 2. SceneLighting Event Invoke
		// DUOLGameEngine::EventManager::GetInstance()->InvokeEvent(TEXT("SceneLighting"));
#pragma endregion

		// 1-1. Gizmo Render Event
		DUOLGameEngine::GraphicsManager::GetInstance()->UpdateRenderScreenSize(_image->_size);

		// 3. Camera Info (For game update)
		if (_camera != nullptr)
		{
			// Game View 화면 사이즈에 맞게 카메라 세팅 변경
			_camera->OnResize(&_image->_size);

			DUOLGameEngine::GraphicsManager::GetInstance()->UpdateCameraInfo(&_camera->GetCameraInfo());
		}

		// 4. Execute
		DUOLGameEngine::GraphicsManager::GetInstance()->Execute(TEXT("Scene"), true);

		_image->_textureID = DUOLGameEngine::GraphicsManager::GetInstance()->GetShaderResourceAddress(TEXT("SceneView"));

		// Object ID Picking
		if (DUOLGameEngine::InputManager::GetInstance()->GetMouseButtonDown(DUOLGameEngine::MouseCode::Left))
		{
			DUOLMath::Vector2 mousePosition = DUOLGameEngine::InputManager::GetInstance()->GetMousePosition();

			// 현재 View 기준으로 마우스의 위치를 옮겨줍니다.
			mousePosition.x -= _position.x;
			mousePosition.y = mousePosition.y - _position.y - ImGui::GetFrameHeight();

			ObjectPicking(mousePosition);
		}
	}
}