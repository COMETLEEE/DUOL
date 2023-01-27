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
		// ���� ������ �ʴ� Ư���� ������Ʈ�� �����մϴ�. ī�޶� ��Ʈ�Ѹ��� ���� ����մϴ�.
		_perspCameraObject = std::make_shared<DUOLGameEngine::GameObject>(TEXT("ControllableViewObject_Perspective"));

		_perspCameraObject->AddComponent<DUOLGameEngine::Transform>();

		_perspCameraObject->GetTransform()->SetPosition(DUOLMath::Vector3(0.f, 0.f, -10.f));

		_perspectiveCamera = _perspCameraObject->AddComponent<DUOLGameEngine::Camera>();

		_perspCameraObject->AddComponent<DUOLGameEngine::TPFController>();

		_perspCameraObject->OnAwake();

		_perspCameraObject->OnStart();

		// ���� ������ �ʴ� Ư���� ������Ʈ�� �����մϴ�. ī�޶� ��Ʈ�Ѹ��� ���� ����մϴ�.
		_orthoCameraObject = std::make_shared<DUOLGameEngine::GameObject>(TEXT("ControllableViewObject_Orthographic"));

		_orthoCameraObject->AddComponent<DUOLGameEngine::Transform>();

		_orthographicCamera = _orthoCameraObject->AddComponent<DUOLGameEngine::Camera>();

		// Orthographic Camera
		_orthographicCamera->SetIsOrthographic(true);

		_orthoCameraObject->AddComponent<DUOLGameEngine::TPFController>();

		_orthoCameraObject->OnAwake();

		_orthoCameraObject->OnStart();
#pragma endregion
	}

	ControllableViewBase::~ControllableViewBase()
	{
		_perspCameraObject.reset();

		_orthoCameraObject.reset();
	}

	uint64_t ControllableViewBase::ObjectPicking(const DUOLMath::Vector2& currentTextureSize, const DUOLMath::Vector2& mousePosition)
	{
		uint64_t objectID = DUOLGameEngine::GraphicsManager::GetInstance()->FastPicking(currentTextureSize, mousePosition);

		return objectID;
	}

	void ControllableViewBase::Update(float deltaTime)
	{
		// Hover ���� �� ������Ʈ�� �ǽ��մϴ�.
		if (GetIsHovered())
		{
			_perspCameraObject->OnUpdate(deltaTime);

			_perspCameraObject->OnLateUpdate(deltaTime);

			_orthoCameraObject->OnUpdate(deltaTime);

			_orthoCameraObject->OnLateUpdate(deltaTime);
		}
	}
}