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

		auto sr = DUOLGameEngine::GraphicsManager::GetInstance()->GetScreenSize();

		DUOLGameEngine::GraphicsManager::GetInstance()->UpdateRenderScreenSize(sr);

		// 3. Camera Info (For game update)
		DUOLGameEngine::Camera* mainCam =
			DUOLGameEngine::Camera::GetMainCamera();

		if (mainCam != nullptr)
		{
			// Game View 화면 사이즈에 맞게 카메라 세팅 변경
			mainCam->OnResize(&sr);

			DUOLGameEngine::GraphicsManager::GetInstance()->UpdateCameraInfo(&mainCam->GetCameraInfo());
		}

		// 4. Execute
		std::vector<DUOLGraphicsEngine::RenderingPipelinesList> pipelineLists = {};

		auto&& gameView = *DUOLGameEngine::GraphicsManager::GetInstance()->GetRenderingPipelineList(TEXT("GameView"));

		gameView._cameraData = const_cast<DUOLGraphicsEngine::Camera*>(&mainCam->GetCameraInfo());

		pipelineLists.push_back(gameView);

		DUOLGameEngine::GraphicsManager::GetInstance()->Execute(pipelineLists, false);
		
		// 2. Get textureID of game view.
		// TODO - 다음 Scene View에서 'GameView' 재활용함 .. CopyGameView를 사용하자.
		_image->_textureID = DUOLGameEngine::GraphicsManager::GetInstance()->GetShaderResourceAddress(TEXT("CopyGameView"));
	}
}