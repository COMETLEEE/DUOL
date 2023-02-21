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
		// Select GameObject Event ���
		DUOLEditor::EditorEventManager::GetInstance()->GetGameObjectSelectedEvent() +=
			std::bind([this](DUOLGameEngine::GameObject* gameObject) { this->_selectedGameObject = gameObject; },
				std::placeholders::_1);

		// UnSelect GameObject Event ���
		DUOLEditor::EditorEventManager::GetInstance()->GetGameObjectUnselectedEvent() +=
			std::bind([this]() { this->_selectedGameObject = nullptr; });

		// ���� �׽ý� �ø� ��� X
		IMGUIZMO_NAMESPACE::AllowAxisFlip(false);

		_panelWindowCallbacksAfter += [this]()
		{
#pragma region IMGUIZMO
			// �����찡 Focus�� �� ���� �� ��Ŭ�� (Flying Mode) �� �ƴ� ���¿��� ���� Ŭ���� ���� ��
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

				// ���� ������ �ִ´�.
				IMGUIZMO_NAMESPACE::SetRect(_position.x, _position.y + ImGui::GetFrameHeight(), GetSafeSize().x, GetSafeSize().y);

				if (IMGUIZMO_NAMESPACE::IsUsing())
				{
					// ��Ʈ���� ���� �� ������Ʈ (=> �ڽ� ��ü�鵵 ..!)
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
			//// Root Object�� ���ø� �����մϴ�.
			//auto&& rootObjects = scene->GetRootObjects();

			//for (auto& rootObject : rootObjects)
			//{
			//	// ������ �Ǿ����ϴ�.
			//	if (pickedID == rootObject->GetUUID())
			//	{
			//		DUOLEditor::EditorEventManager::GetInstance()->SelectGameObject(rootObject);

			//		return;
			//	}
			//}

			auto&& gameObjects =  scene->GetAllGameObjects();
			
			for (auto& gameObject : gameObjects)
			{
				// ������ �Ǿ����ϴ�.
				if (pickedID == gameObject->GetUUID())
				{
					DUOLEditor::EditorEventManager::GetInstance()->SelectGameObject(gameObject);

					return;
				}
			}
		}

		// ���� ���õ� ���� ������Ʈ�� ���� Gizmo�� ���ٸ� Unselect + Selected Axis �� ������ �����մϴ�.
		DUOLEditor::EditorEventManager::GetInstance()->UnselectGameObject();
#pragma endregion
	}

	void SceneView::Update(float deltaTime)
	{
		ControllableViewBase::Update(deltaTime);

		// 0. ������ Game View (== Game) �� �׷�����. �����س���
		DUOLGameEngine::GraphicsManager::GetInstance()->CopyTexture(TEXT("CopyGameView"), TEXT("GameView"));

		// 1. SceneView�� �̹����� ȣ���ؾ��� �ؽ�ó�� ������Ʈ�մϴ�.
		_image->_size = GetSafeSize();

		DUOLGameEngine::GraphicsManager::GetInstance()->UpdateRenderScreenSize(_image->_size);

		// 3. Perspective Camera Info
		if (_perspectiveCamera != nullptr)
		{
			// Scene View ȭ�� ����� �°� ī�޶� ���� ����
			_perspectiveCamera->OnResize(&_image->_size);

			DUOLGameEngine::GraphicsManager::GetInstance()->UpdateCameraInfo(&_perspectiveCamera->GetCameraInfo());
		}

		// 4. Execute - Perspective
		DUOLGameEngine::GraphicsManager::GetInstance()->Execute(TEXT("Scene"), true);

		if (GetIsHovered())
		{
			// ���콺 ��ġ�� ��ũ�� ��ǥ�� �սô�.
			DUOLMath::Vector2 mousePosition = DUOLGameEngine::InputManager::GetInstance()->GetMousePositionInScreen();

			// ���� View �������� ���콺�� ��ġ�� �Ű� ��� ��ġ�� �ȼ��� ���콺�� �ö󰬴��� �˻��մϴ�.
			mousePosition.x -= _position.x;

			mousePosition.y = mousePosition.y - _position.y - ImGui::GetFrameHeight();

			// ����� Over���� �ʰ� ������ ���� ��ư�� �ٿ�Ǿ����ϴ�. Object ID Picking
			if (!IMGUIZMO_NAMESPACE::IsOver() && DUOLGameEngine::InputManager::GetInstance()->GetMouseButtonDown(DUOLGameEngine::MouseCode::Left))
			{
				ObjectPicking_SceneView(_image->_size, mousePosition);
			}
		}

		// SceneView panel�� �������� Object�� �׸��ϴ�.
		_image->_textureID = DUOLGameEngine::GraphicsManager::GetInstance()->GetShaderResourceAddress(TEXT("SceneView"));
	}
}