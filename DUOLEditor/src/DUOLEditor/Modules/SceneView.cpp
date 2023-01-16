#include "DUOLEditor/Modules/SceneView.h"
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

namespace DUOLEditor
{
	SceneView::SceneView(const DUOLCommon::tstring& title, bool isOpened, const PanelWindowSetting& windowSetting) :
		ControllableViewBase(title, isOpened, windowSetting)
	{
		_cameraGizmoMaterials.push_back(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(TEXT("CamMat"))->GetPrimitiveMaterial());

		for (int i = 0 ; i < MAX_CAMERA_GIZMO ; i++)
		{
			_cameraGizmoMeshInfos[i].SetTransformPointer(&_cameraGizmoTransform[i]);

			_cameraGizmos[i]._renderInfo = &_cameraGizmoMeshInfos[i];

			_cameraGizmos[i]._mesh = DUOLGameEngine::ResourceManager::GetInstance()->GetMesh(TEXT("Camera"))->GetPrimitiveMesh();

			_cameraGizmos[i]._materials = &_cameraGizmoMaterials;
		}
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

	void SceneView::RenderAllGizmo()
	{
		DUOLGameEngine::Scene* scene = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene();

		auto&& allGameObjects = scene->GetAllGameObjects();

		int cameraCount = 0;

		for (int i = 0; i < allGameObjects.size(); i++)
		{
			DUOLGameEngine::GameObject* gameObject = allGameObjects[i];

			DUOLGameEngine::Transform* transform = gameObject->GetTransform();

			DUOLGameEngine::GameObject* rootGameObject = transform->GetRootTransform()->GetGameObject();

			DUOLGameEngine::Camera* camera = gameObject->GetComponent<DUOLGameEngine::Camera>();

			DUOLGameEngine::Light* light = gameObject->GetComponent<DUOLGameEngine::Light>();

			if (camera != nullptr)
			{
				const DUOLMath::Matrix& worldTM = transform->GetWorldMatrix();

				_cameraGizmoTransform[cameraCount]._world = worldTM;

				_cameraGizmoTransform[cameraCount]._worldInvTranspose = worldTM.Invert().Transpose();

				_cameraGizmoMeshInfos[cameraCount].SetObjectID(rootGameObject->GetUUID());

				DUOLGameEngine::GraphicsManager::GetInstance()->ReserveRenderObject(&_cameraGizmos[cameraCount++]);
			}

			/*if (light != nullptr)
			{
				
			}*/
		}
	}

	void SceneView::Update(float deltaTime)
	{
		ControllableViewBase::Update(deltaTime);

#pragma region 이제 그냥 렌더타겟 전부 나눠버렸다. 복사할 필요 없다 ..!
		// 0. 이전에 Game View (== Game) 가 그려졌다. 복사해놓자
		DUOLGameEngine::GraphicsManager::GetInstance()->CopyTexture(TEXT("CopyGameView"), TEXT("GameView"));
#pragma endregion

		// 1. SceneView의 이미지가 호출해야할 텍스처를 업데이트합니다.
		_image->_size = GetSafeSize();

#pragma region GameView를 그릴 때 Context를 초기화하지 않았기 때문에 Rendering, Lighting 등의 이벤트를 한 번 더 일으킬 필요가 없습니다 ..!
		// 1. SceneRender Event Invoke
		// DUOLGameEngine::EventManager::GetInstance()->InvokeEvent(TEXT("SceneRendering"));

		// 2. SceneLighting Event Invoke
		// DUOLGameEngine::EventManager::GetInstance()->InvokeEvent(TEXT("SceneLighting"));
#pragma endregion

		// 1-1. Gizmo Render Event
		DUOLGameEngine::GraphicsManager::GetInstance()->UpdateRenderScreenSize(_image->_size);

		RenderAllGizmo();

		// 3. Camera Info (For game update)
		if (_camera != nullptr)
		{
			// Game View 화면 사이즈에 맞게 카메라 세팅 변경
			_camera->OnResize(&_image->_size);

			DUOLGameEngine::GraphicsManager::GetInstance()->UpdateCameraInfo(&_camera->GetCameraInfo());
		}

		// 4. Execute
		DUOLGameEngine::GraphicsManager::GetInstance()->Execute(TEXT("Scene"), true);

		_image->_textureID = DUOLGameEngine::GraphicsManager::GetInstance()->GetShaderResourceAddress(TEXT("SceneView"));

		if (GetIsHovered())
		{
			// Object ID Picking
			if (DUOLGameEngine::InputManager::GetInstance()->GetMouseButtonDown(DUOLGameEngine::MouseCode::Left))
			{
				DUOLMath::Vector2 mousePosition = DUOLGameEngine::InputManager::GetInstance()->GetMousePosition();

				// 현재 View 기준으로 마우스의 위치를 옮겨줍니다.
				mousePosition.x -= _position.x;
				mousePosition.y = mousePosition.y - _position.y - ImGui::GetFrameHeight();

				ObjectPicking(_image->_size, mousePosition);
			}
		}

		// 만약, 현재 선택된 게임 오브젝트가 있다면 아웃 라인과
		// 현재 오퍼레이션에 맞는 기즈모를 그려줍니다.
		if (_selectedGameObject != nullptr)
			RenderOutline();
	}
}