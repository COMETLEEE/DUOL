#include "DUOLEditor/Modules/GameView.h"

#include "DUOLEditor/UI/Widgets/Display/Image.h"

#include "DUOLGameEngine/ECS/Component/Camera.h"

#include "DUOLGameEngine/Manager/EventManager.h"
#include "DUOLGameEngine/Manager/GraphicsManager.h"

namespace DUOLEditor
{
	GameView::GameView(const DUOLCommon::tstring& title, bool isOpened, const PanelWindowSetting& windowSetting) :
		ViewBase(title, isOpened, windowSetting)
	{

	}

	GameView::~GameView()
	{
	}

	void GameView::Update(float deltaTime)
	{
		// 0. Get view size.
		_image->_size = GetSafeSize();

		// 1. SceneRender Event Invoke
		DUOLGameEngine::EventManager::GetInstance()->InvokeEvent(TEXT("SceneRendering"));

		// 2. SceneLighting Event Invoke
		DUOLGameEngine::EventManager::GetInstance()->InvokeEvent(TEXT("SceneLighting"));

		DUOLGameEngine::GraphicsManager::GetInstance()->UpdateRenderScreenSize(_image->_size);

		// 3. Camera Info (For game update)
		const std::shared_ptr<DUOLGameEngine::Camera> mainCam =
			DUOLGameEngine::Camera::GetMainCamera();

		if (mainCam != nullptr)
		{
			// Game View ȭ�� ����� �°� ī�޶� ���� ����
			mainCam->OnResize(&_image->_size);

			DUOLGameEngine::GraphicsManager::GetInstance()->UpdateCameraInfo(&mainCam->GetCameraInfo());
		}

		// 4. Execute
		DUOLGameEngine::GraphicsManager::GetInstance()->Execute(TEXT("GameView"), false);
		
		// 2. Get textureID of game view.
		// TODO - ���� Scene View���� 'GameView' ��Ȱ���� .. CopyGameView�� �������.
		_image->_textureID = DUOLGameEngine::GraphicsManager::GetInstance()->GetShaderResourceAddress(TEXT("CopyGameView"));
	}
}