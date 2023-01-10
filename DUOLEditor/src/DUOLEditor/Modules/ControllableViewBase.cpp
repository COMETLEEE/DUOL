#include "DUOLEditor/Modules/ControllableViewBase.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/Camera.h"
#include "DUOLGameEngine/ECS/Component/TPFController.h"

namespace DUOLEditor
{
	ControllableViewBase::ControllableViewBase(const DUOLCommon::tstring& title, bool isOpened,
		const PanelWindowSetting& windowSettings) :
		ViewBase(title, isOpened, windowSettings)
	{
		// 씬에 속하지 않는 특별한 오브젝트를 생성합니다. 카메라 컨트롤링을 위해 사용합니다.
		_cameraObject = std::make_shared<DUOLGameEngine::GameObject>(TEXT("ControllableViewObject"));

		_cameraObject->AddComponent<DUOLGameEngine::Transform>();

		_cameraObject->AddComponent<DUOLGameEngine::Camera>();

		_cameraObject->AddComponent<DUOLGameEngine::TPFController>();

		_cameraObject->OnAwake();

		_cameraObject->OnStart();
	}

	ControllableViewBase::~ControllableViewBase()
	{
		_cameraObject.reset();
	}

	void ControllableViewBase::Update(float deltaTime)
	{
		// Hover 중일 때 업데이트를 실시합니다.
		if (GetIsHovered())
		{
			_cameraObject->OnUpdate(deltaTime);

			_cameraObject->OnLateUpdate(deltaTime);
		}
	}
}