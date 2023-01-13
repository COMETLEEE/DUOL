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

#pragma region ���� �׳� ����Ÿ�� ���� �������ȴ�. ������ �ʿ� ���� ..!
		// 0. ������ Game View (== Game) �� �׷�����. �����س���
		DUOLGameEngine::GraphicsManager::GetInstance()->CopyTexture(TEXT("CopyGameView"), TEXT("GameView"));
#pragma endregion

		// 1. SceneView�� �̹����� ȣ���ؾ��� �ؽ�ó�� ������Ʈ�մϴ�.
		_image->_size = GetSafeSize();

#pragma region GameView�� �׸� �� Context�� �ʱ�ȭ���� �ʾұ� ������ Rendering, Lighting ���� �̺�Ʈ�� �� �� �� ����ų �ʿ䰡 �����ϴ� ..!
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
			// Game View ȭ�� ����� �°� ī�޶� ���� ����
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

			// ���� View �������� ���콺�� ��ġ�� �Ű��ݴϴ�.
			mousePosition.x -= _position.x;
			mousePosition.y = mousePosition.y - _position.y - ImGui::GetFrameHeight();

			ObjectPicking(mousePosition);
		}
	}
}