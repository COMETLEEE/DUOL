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

#pragma region ���� �׳� ����Ÿ�� ���� �������ȴ�. ������ �ʿ� ���� ..!
		// 0. ������ Game View (== Game) �� �׷�����. �����س���
		DUOLGameEngine::GraphicsManager::GetInstance()->CopyTexture(TEXT("CopyGameView"), TEXT("GameView"));
#pragma endregion

		// 1. SceneView�� �̹����� ȣ���ؾ��� �ؽ�ó�� ������Ʈ�մϴ�.
		_image->_size = GetSafeSize();

#pragma region GameView�� �׸� �� Context�� �ʱ�ȭ���� �ʾұ� ������ Rendering, Lighting ���� �̺�Ʈ�� �� �� �� ����ų �ʿ䰡 �����ϴ� ..!
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
			// Game View ȭ�� ����� �°� ī�޶� ���� ����
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

				// ���� View �������� ���콺�� ��ġ�� �Ű��ݴϴ�.
				mousePosition.x -= _position.x;
				mousePosition.y = mousePosition.y - _position.y - ImGui::GetFrameHeight();

				ObjectPicking(_image->_size, mousePosition);
			}
		}

		// ����, ���� ���õ� ���� ������Ʈ�� �ִٸ� �ƿ� ���ΰ�
		// ���� ���۷��̼ǿ� �´� ����� �׷��ݴϴ�.
		if (_selectedGameObject != nullptr)
			RenderOutline();
	}
}