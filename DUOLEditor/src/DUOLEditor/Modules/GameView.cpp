#include "DUOLEditor/Modules/GameView.h"

#include "DUOLCommon/Log/LogHelper.h"
#include "DUOLEditor/UI/Widgets/Display/Image.h"

#include "DUOLGameEngine/ECS/Component/Camera.h"

#include "DUOLGameEngine/Manager/EventManager.h"
#include "DUOLGameEngine/Manager/GraphicsManager.h"
#include "DUOLGameEngine/Manager/InputManager.h"
#include "DUOLGameEngine/Manager/UIManager.h"


namespace DUOLEditor
{
	GameView::GameView(const DUOLCommon::tstring& title, bool isOpened, const PanelWindowSetting& windowSetting) :
		ViewBase(title, isOpened, windowSetting)
		, _isInGameMode(false)
	{

	}

	GameView::~GameView()
	{
	}

	void GameView::Update(float deltaTime)
	{
		// Hovered + Left Click => InGameMode
		if (GetIsHovered() && !_isInGameMode &&  DUOLGameEngine::InputManager::GetInstance()->GetMouseButtonDown(DUOLGameEngine::MouseCode::Left))
		{
			DUOLGameEngine::InputManager::GetInstance()->SetGameLockMode(true);

			_isInGameMode = true;
		}
		else if (_isInGameMode && DUOLGameEngine::InputManager::GetInstance()->GetKeyDown(DUOLGameEngine::KeyCode::Escape))
		{
			DUOLGameEngine::InputManager::GetInstance()->SetGameLockMode(false);

			DUOLGameEngine::InputManager::GetInstance()->SetLockRect(DUOLMath::Vector4::Zero);

			_isInGameMode = false;
		}

		// -1. Mouse lock rect setting.
		if (_isInGameMode)
		{
			const DUOLMath::Vector2& size =	GetSafeSize();

			const DUOLMath::Vector2& position = GetPosition();

			DUOLGameEngine::InputManager::GetInstance()->SetLockRect(DUOLMath::Vector4{ position.x, position.y, position.x + size.x, position.y + size.y });
		}

		// 0. Get view size.
		_image->_size = GetSafeSize();

		// 1. SceneRender Event Invoke
		DUOLGameEngine::EventManager::GetInstance()->InvokeEvent(TEXT("SceneRendering"));

		// 2. SceneLighting Event Invoke
		DUOLGameEngine::EventManager::GetInstance()->InvokeEvent(TEXT("SceneLighting"));

		auto sr = DUOLGameEngine::GraphicsManager::GetInstance()->GetScreenSize();

		DUOLGameEngine::GraphicsManager::GetInstance()->UpdateCurrentSceneInfo(sr);

		// 3. Camera Info (For game update)
		DUOLGameEngine::Camera* mainCam =
			DUOLGameEngine::Camera::GetMainCamera();

		if (mainCam != nullptr)
		{
			// Game View 화면 사이즈에 맞게 카메라 세팅 변경
			mainCam->OnResize(&sr);
		}
		else
			return;

		auto camInfo = &mainCam->GetCameraInfo();

		DUOLGameEngine::GraphicsManager::GetInstance()->UpdateCameraInfo(camInfo);

		// 4. Execute
		std::vector<DUOLGraphicsEngine::RenderingPipelinesList> pipelineLists = {};

		auto&& gameView = *DUOLGameEngine::GraphicsManager::GetInstance()->GetRenderingPipelineList(TEXT("GameView"));

		gameView._cameraData = const_cast<DUOLGraphicsEngine::Camera*>(camInfo);

		pipelineLists.push_back(gameView);

		DUOLGameEngine::GraphicsManager::GetInstance()->Execute(pipelineLists, false);

		// 2. Get textureID of game view.
		// TODO - 다음 Scene View에서 'GameView' 재활용함 .. CopyGameView를 사용하자.
		_image->_textureID = DUOLGameEngine::GraphicsManager::GetInstance()->GetShaderResourceAddress(TEXT("CopyGameView"));

		// UIManager에 GameView의 pos를 넣어준다.
		// 좌상단
		auto pos = DUOLGameEngine::InputManager::GetInstance()->GetScreenPositionInScreen();;
		auto gamePos = GetPosition();
		auto gameViewPos = gamePos - pos;
		auto size = GetSize();

		gameViewPos.y -= ImGui::GetFrameHeight();

		DUOLGameEngine::UIManager::GetInstance()->SetGameViewPosition(gameViewPos);
		DUOLGameEngine::UIManager::GetInstance()->SetGameViewSize(size);


	}
}