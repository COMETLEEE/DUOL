#include "DUOLEditor/Modules/ControllableViewBase.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Camera.h"
#include "DUOLGameEngine/ECS/Component/TPFController.h"
#include "DUOLGameEngine/Manager/InputManager.h"

#include "DUOLGameEngine/Manager/GraphicsManager.h"

namespace DUOLEditor
{
	ControllableViewBase::ControllableViewBase(const DUOLCommon::tstring& title, bool isOpened,
		const PanelWindowSetting& windowSettings) :
		ViewBase(title, isOpened, windowSettings)
	{
		// ���� ������ �ʴ� Ư���� ������Ʈ�� �����մϴ�. ī�޶� ��Ʈ�Ѹ��� ���� ����մϴ�.
		_cameraObject = std::make_shared<DUOLGameEngine::GameObject>(TEXT("ControllableViewObject"));

		_cameraObject->AddComponent<DUOLGameEngine::Transform>();

		_camera = _cameraObject->AddComponent<DUOLGameEngine::Camera>();

		_cameraObject->AddComponent<DUOLGameEngine::TPFController>();

		_cameraObject->OnAwake();

		_cameraObject->OnStart();
	}

	ControllableViewBase::~ControllableViewBase()
	{
		_cameraObject.reset();
	}

	void ControllableViewBase::ObjectPicking(const DUOLMath::Vector2& mousePosition)
	{
		// ���� �����غ��� ..
		uint64_t objectID =	DUOLGameEngine::GraphicsManager::GetInstance()->FastPicking(mousePosition);
	}

	void ControllableViewBase::Update(float deltaTime)
	{
		// Hover ���� �� ������Ʈ�� �ǽ��մϴ�.
		if (GetIsHovered())
		{
			_cameraObject->OnUpdate(deltaTime);

			_cameraObject->OnLateUpdate(deltaTime);
		}
	}
}