#include "DUOLEditor/Modules/ControllableViewBase.h"

#include "DUOLEditor/Modules/EditorEventManager.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Camera.h"
#include "DUOLGameEngine/ECS/Component/TPFController.h"
#include "DUOLGameEngine/Manager/InputManager.h"

#include "DUOLGameEngine/Manager/GraphicsManager.h"
#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"

namespace DUOLEditor
{
	ControllableViewBase::ControllableViewBase(const DUOLCommon::tstring& title, bool isOpened,
		const PanelWindowSetting& windowSettings) :
		ViewBase(title, isOpened, windowSettings)
		, _selectedGameObject(nullptr)
	{
#pragma region CAMERA
		// 씬에 속하지 않는 특별한 오브젝트를 생성합니다. 카메라 컨트롤링을 위해 사용합니다.
		_perspCameraObject = std::make_shared<DUOLGameEngine::GameObject>(TEXT("ControllableViewObject_Perspective"));

		_perspCameraObject->AddComponent<DUOLGameEngine::Transform>();

		_perspCameraObject->GetTransform()->SetPosition(DUOLMath::Vector3(0.f, 0.f, -10.f));

		_perspectiveCamera = _perspCameraObject->AddComponent<DUOLGameEngine::Camera>();

		_perspCameraObject->AddComponent<DUOLGameEngine::TPFController>();

		_perspCameraObject->OnAwake();

		_perspCameraObject->OnStart();
#pragma endregion
	}

	ControllableViewBase::~ControllableViewBase()
	{
		_perspCameraObject.reset();
	}

	uint64_t ControllableViewBase::ObjectPicking(const DUOLMath::Vector2& currentTextureSize, const DUOLMath::Vector2& mousePosition)
	{
		uint64_t objectID = DUOLGameEngine::GraphicsManager::GetInstance()->FastPicking(currentTextureSize, mousePosition);

		return objectID;
	}

	void ControllableViewBase::Update(float deltaTime)
	{
		// Focused + Mouse 우클릭 시 카메라의 업데이트를 진행합니다
		if (GetIsFocused())
		{
			_perspCameraObject->OnUpdate(deltaTime);

			if (DUOLGameEngine::InputManager::GetInstance()->GetMouseButtonPressed(DUOLGameEngine::MouseCode::Right))
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_None);

				_perspCameraObject->OnLateUpdate(deltaTime);
			}
		}
	}
}