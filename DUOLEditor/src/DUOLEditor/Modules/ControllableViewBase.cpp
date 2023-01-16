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
		// 씬에 속하지 않는 특별한 오브젝트를 생성합니다. 카메라 컨트롤링을 위해 사용합니다.
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

	void ControllableViewBase::ObjectPicking(const DUOLMath::Vector2& currentTextureSize, const DUOLMath::Vector2& mousePosition)
	{
		uint64_t objectID =	DUOLGameEngine::GraphicsManager::GetInstance()->FastPicking(currentTextureSize, mousePosition);

		DUOLGameEngine::Scene* scene = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene();

		if (scene != nullptr)
		{
			// Root Object의 선택만 가능합니다.
			auto&& rootObjects =  scene->GetRootObjects();

			for (auto& rootObject : rootObjects)
			{
				// 선택이 되었습니다.
				if (objectID == rootObject->GetUUID())
				{
					DUOLEditor::EditorEventManager::GetInstance()->SelectGameObject(rootObject);

					_selectedGameObject = rootObject;

					return;
				}
			}

			// 만약 선택된 게임 오브젝트가 없다면 Unselect !
			DUOLEditor::EditorEventManager::GetInstance()->UnselectGameObject();

			_selectedGameObject = nullptr;
		}
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
