#include "DUOLEditor/Modules/SceneView.h"

#include "DUOLEditor/Modules/EditorEventManager.h"
#include "DUOLEditor/UI/Widgets/Display/Image.h"

#include "DUOLGameEngine/ECS/Component/Camera.h"
#include "DUOLGameEngine/ECS/Component/Light.h"

#include "DUOLGameEngine/Manager/GraphicsManager.h"
#include "DUOLGameEngine/Manager/EventManager.h"
#include "DUOLGameEngine/Manager/InputManager.h"
#include "DUOLGameEngine/Manager/ResourceManager.h"
#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Object/Material.h"
#include "DUOLGameEngine/ECS/Object/Mesh.h"

#include "DUOLEditor/Util/EditorSettings.h"

namespace DUOLEditor
{
	SceneView::SceneView(const DUOLCommon::tstring& title, bool isOpened, const PanelWindowSetting& windowSetting) :
		ControllableViewBase(title, isOpened, windowSetting)
		, _currentOperation(IMGUIZMO_NAMESPACE::OPERATION::TRANSLATE)
		, _currentMode(IMGUIZMO_NAMESPACE::MODE::WORLD)
	{
		// Select GameObject Event 등록
		DUOLEditor::EditorEventManager::GetInstance()->GetGameObjectSelectedEvent() +=
			std::bind([this](DUOLGameEngine::GameObject* gameObject) { this->_selectedGameObject = gameObject; },
				std::placeholders::_1);

		// UnSelect GameObject Event 등록
		DUOLEditor::EditorEventManager::GetInstance()->GetGameObjectUnselectedEvent() +=
			std::bind([this]() { this->_selectedGameObject = nullptr; });

		// 기즈 액시스 플립 허용 X
		IMGUIZMO_NAMESPACE::AllowAxisFlip(false);

		_panelWindowCallbacksAfter += [this]()
		{
#pragma region IMGUIZMO
			// 윈도우가 Focus가 된 상태 및 우클릭 (Flying Mode) 가 아닌 상태에서 에서 클릭을 받을 때
			if (GetIsFocused() && !DUOLGameEngine::InputManager::GetInstance()->GetMouseButtonPressed(DUOLGameEngine::MouseCode::Right))
			{
				if (DUOLGameEngine::InputManager::GetInstance()->GetKeyDown(DUOLGameEngine::KeyCode::W) == true)
				{
					_currentOperation = IMGUIZMO_NAMESPACE::OPERATION::TRANSLATE;
				}
				else if (DUOLGameEngine::InputManager::GetInstance()->GetKeyDown(DUOLGameEngine::KeyCode::E) == true)
				{
					_currentOperation = IMGUIZMO_NAMESPACE::OPERATION::ROTATE;

				}
				else if (DUOLGameEngine::InputManager::GetInstance()->GetKeyDown(DUOLGameEngine::KeyCode::R) == true)
				{
					_currentOperation = IMGUIZMO_NAMESPACE::OPERATION::SCALE;
				}
				else if (DUOLGameEngine::InputManager::GetInstance()->GetKeyDown(DUOLGameEngine::KeyCode::T) == true)
				{
					_currentMode == IMGUIZMO_NAMESPACE::MODE::LOCAL ? _currentMode = IMGUIZMO_NAMESPACE::MODE::WORLD : _currentMode = IMGUIZMO_NAMESPACE::MODE::LOCAL;
				}
			}

			if (_selectedGameObject != nullptr)
			{
				IMGUIZMO_NAMESPACE::SetOrthographic(false);

				IMGUIZMO_NAMESPACE::SetDrawlist();

				IMGUIZMO_NAMESPACE::Manipulate(reinterpret_cast<float*>(&_perspectiveCamera->_viewMatrix), reinterpret_cast<float*>(&_perspectiveCamera->_projectionMatrix)
					, _currentOperation, _currentMode,
					reinterpret_cast<float*>(&const_cast<DUOLMath::Matrix&>(_selectedGameObject->GetTransform()->GetWorldMatrix())));

				// 씬뷰 공간에 넣는다.
				IMGUIZMO_NAMESPACE::SetRect(_position.x, _position.y + ImGui::GetFrameHeight(), GetSafeSize().x, GetSafeSize().y);

				if (IMGUIZMO_NAMESPACE::IsUsing())
				{
					// 매트릭스 분해 및 업데이트 (=> 자식 개체들도 ..!)
					_selectedGameObject->GetTransform()->SetWorldTM(_selectedGameObject->GetTransform()->GetWorldMatrix());
				}
			}
#pragma endregion
		};
	}

	SceneView::~SceneView()
	{

	}

	void SceneView::RenderOutline()
	{
		uint64_t selectedID = _selectedGameObject->GetUUID();

		DUOLGameEngine::GraphicsManager::GetInstance()->_renderingPipelineLayouts.at(TEXT("IDOutline"))->_dataSize = sizeof(uint64_t);
		DUOLGameEngine::GraphicsManager::GetInstance()->_renderingPipelineLayouts.at(TEXT("IDOutline"))->_perObjectBufferData = &selectedID;

		DUOLGameEngine::GraphicsManager::GetInstance()->Execute(TEXT("IDOutline"), true, false);
	}

	void SceneView::ObjectPicking_SceneView(const DUOLMath::Vector2& currentTextureSize, const DUOLMath::Vector2& mousePosition)
	{
		auto&& pickedID = ControllableViewBase::ObjectPicking(_image->_size, mousePosition);

#pragma region GAMEOBJECT_PICKING
		DUOLGameEngine::Scene* scene = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene();

		if (scene != nullptr)
		{
			//// Root Object의 선택만 가능합니다.
			//auto&& rootObjects = scene->GetRootObjects();

			//for (auto& rootObject : rootObjects)
			//{
			//	// 선택이 되었습니다.
			//	if (pickedID == rootObject->GetUUID())
			//	{
			//		DUOLEditor::EditorEventManager::GetInstance()->SelectGameObject(rootObject);

			//		return;
			//	}
			//}

			auto&& gameObjects =  scene->GetAllGameObjects();
			
			for (auto& gameObject : gameObjects)
			{
				// 선택이 되었습니다.
				if (pickedID == gameObject->GetUUID())
				{
					DUOLEditor::EditorEventManager::GetInstance()->SelectGameObject(gameObject);

					return;
				}
			}
		}

		// 만약 선택된 게임 오브젝트도 없고 Gizmo도 없다면 Unselect + Selected Axis 도 없도록 지정합니다.
		DUOLEditor::EditorEventManager::GetInstance()->UnselectGameObject();
#pragma endregion
	}

	void SceneView::Update(float deltaTime)
	{
		ControllableViewBase::Update(deltaTime);

		// 0. 이전에 Game View (== Game) 가 그려졌다. 복사해놓자
		DUOLGameEngine::GraphicsManager::GetInstance()->CopyTexture(TEXT("CopyGameView"), TEXT("GameView"));

		// 1. SceneView의 이미지가 호출해야할 텍스처를 업데이트합니다.
		_image->_size = GetSafeSize();

		DUOLGameEngine::GraphicsManager::GetInstance()->UpdateRenderScreenSize(_image->_size);

		// 3. Perspective Camera Info
		if (_perspectiveCamera != nullptr)
		{
			// Scene View 화면 사이즈에 맞게 카메라 세팅 변경
			_perspectiveCamera->OnResize(&_image->_size);

			DUOLGameEngine::GraphicsManager::GetInstance()->UpdateCameraInfo(&_perspectiveCamera->GetCameraInfo());
		}

		// 4. Execute - Perspective
		DUOLGameEngine::GraphicsManager::GetInstance()->Execute(TEXT("Scene"), true);

		if (GetIsHovered())
		{
			// 마우스 위치도 스크린 좌표로 합시다.
			DUOLMath::Vector2 mousePosition = DUOLGameEngine::InputManager::GetInstance()->GetMousePositionInScreen();

			// 현재 View 기준으로 마우스의 위치를 옮겨 어느 위치의 픽셀에 마우스가 올라갔는지 검사합니다.
			mousePosition.x -= _position.x;

			mousePosition.y = mousePosition.y - _position.y - ImGui::GetFrameHeight();

			// 기즈모에 Over하지 않고 있으며 왼쪽 버튼이 다운되었습니다. Object ID Picking
			if (!IMGUIZMO_NAMESPACE::IsOver() && DUOLGameEngine::InputManager::GetInstance()->GetMouseButtonDown(DUOLGameEngine::MouseCode::Left))
			{
				ObjectPicking_SceneView(_image->_size, mousePosition);
			}
		}

		// SceneView panel은 최종적인 Object를 그립니다.
		_image->_textureID = DUOLGameEngine::GraphicsManager::GetInstance()->GetShaderResourceAddress(TEXT("SceneView"));
	}
}