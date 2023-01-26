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

namespace DUOLEditor
{
	SceneView::SceneView(const DUOLCommon::tstring& title, bool isOpened, const PanelWindowSetting& windowSetting) :
		ControllableViewBase(title, isOpened, windowSetting)
		, _currentTransformGizmoState(TransformGizmoState::Translate)
		, _isTransformOperationGlobal(true)
	{
		_cameraGizmoMaterials.push_back(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(TEXT("CamMat"))->GetPrimitiveMaterial());

		for (int i = 0; i < MAX_CAMERA_GIZMO; i++)
		{
			_cameraGizmoMeshInfos[i].SetTransformPointer(&_cameraGizmoTransform[i]);

			_cameraGizmos[i]._renderInfo = &_cameraGizmoMeshInfos[i];

			_cameraGizmos[i]._mesh = DUOLGameEngine::ResourceManager::GetInstance()->GetMesh(TEXT("Camera"))->GetPrimitiveMesh();

			_cameraGizmos[i]._materials = &_cameraGizmoMaterials;
		}

		_transformRedMaterials.push_back(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(TEXT("TransformRed"))->GetPrimitiveMaterial());

		// ���������� ���� �ٲ��ݴϴ�.
		DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(TEXT("TransformRed"))->GetPrimitiveMaterial()->
			SetPipelineState(DUOLGameEngine::GraphicsManager::GetInstance()->GetPipelineState(TEXT("Gizmo")));

		_transformGreenMaterials.push_back(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(TEXT("TransformGreen"))->GetPrimitiveMaterial());

		DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(TEXT("TransformGreen"))->GetPrimitiveMaterial()->
			SetPipelineState(DUOLGameEngine::GraphicsManager::GetInstance()->GetPipelineState(TEXT("Gizmo")));

		_transformBlueMaterials.push_back(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(TEXT("TransformBlue"))->GetPrimitiveMaterial());

		DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(TEXT("TransformRed"))->GetPrimitiveMaterial()->
			SetPipelineState(DUOLGameEngine::GraphicsManager::GetInstance()->GetPipelineState(TEXT("Gizmo")));

		_transformCenterMaterials.push_back(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(TEXT("TransformWhite"))->GetPrimitiveMaterial());

		DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(TEXT("TransformWhite"))->GetPrimitiveMaterial()->
			SetPipelineState(DUOLGameEngine::GraphicsManager::GetInstance()->GetPipelineState(TEXT("Gizmo")));

		for (int i = 0 ; i < 9 ; i++)
		{
			_transformGizmoMeshInfos[i].SetTransformPointer(&_transformGizmoTransform[i]);

			_transformGizmos[i]._renderInfo = &_transformGizmoMeshInfos[i];

			if (i < 3)
				_transformGizmos[i]._mesh = DUOLGameEngine::ResourceManager::GetInstance()->GetMesh(TEXT("Arrow_Scale"))->GetPrimitiveMesh();
			else if (i < 6)
				_transformGizmos[i]._mesh = DUOLGameEngine::ResourceManager::GetInstance()->GetMesh(TEXT("Arrow_Rotate"))->GetPrimitiveMesh();
			else
				_transformGizmos[i]._mesh = DUOLGameEngine::ResourceManager::GetInstance()->GetMesh(TEXT("Arrow_Translate"))->GetPrimitiveMesh();

			// Look Right Up
			if (i % 3 == 0)
				_transformGizmos[i]._materials = &_transformBlueMaterials;
			else if (i % 3 == 1)
				_transformGizmos[i]._materials = &_transformRedMaterials;
			else if (i % 3 == 2)
				_transformGizmos[i]._materials = &_transformGreenMaterials;
		}

		_transformCenterGizmoMeshInfo.SetTransformPointer(&_transformCenterGizmoTransform);

		_transformCenterGizmo._renderInfo = &_transformCenterGizmoMeshInfo;

		_transformCenterGizmo._mesh = DUOLGameEngine::ResourceManager::GetInstance()->GetMesh(TEXT("Transform_Center"))->GetPrimitiveMesh();

		_transformCenterGizmo._materials = &_transformCenterMaterials;

		// Select GameObject Event ���
		DUOLEditor::EditorEventManager::GetInstance()->GetGameObjectSelectedEvent() +=
			std::bind([this](DUOLGameEngine::GameObject* gameObject) { this->_selectedGameObject = gameObject; },
				std::placeholders::_1);

		// UnSelect GameObject Event ���
		DUOLEditor::EditorEventManager::GetInstance()->GetGameObjectUnselectedEvent() +=
			std::bind([this]() { this->_selectedGameObject = nullptr; });
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

	void SceneView::RenderAllPerspectiveGizmo()
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
		}
	}

	void SceneView::RenderTransformGizmo()
	{
#pragma region TRANSFORM_GIZMO
		if (_selectedGameObject != nullptr)
		{
			if (_isTransformOperationGlobal)
			{
				const DUOLMath::Vector3& selectedTranslation = _selectedGameObject->GetComponent<DUOLGameEngine::Transform>()->GetWorldPosition();

				// Perspective Transform�� �ǽ��ؼ� ���� �׷����� ȭ�鿡�� ��� ��ġ�ϴ��� Ȯ���Ѵ�.
				DUOLMath::Vector4 calcSelc = DUOLMath::Vector4{ selectedTranslation.x, selectedTranslation.y ,selectedTranslation.z , 1.f };

				calcSelc = DUOLMath::Vector4::Transform(calcSelc, _perspectiveCamera->_viewMatrix * _perspectiveCamera->_projectionMatrix);

				calcSelc.x = (calcSelc.x / calcSelc.w) * DUOLGameEngine::GraphicsManager::GetInstance()->GetScreenSize().x / 2;
				calcSelc.y = (calcSelc.y / calcSelc.w) * DUOLGameEngine::GraphicsManager::GetInstance()->GetScreenSize().y / 2;
				calcSelc.z = (calcSelc.z / calcSelc.w);

				// NDC ��ǥ + ���� ��ũ�� �ػ󵵿��� ��� ���� ��ġ�ϴ��� Ȯ���մϴ�.
				DUOLMath::Vector3 calcSel = DUOLMath::Vector3{calcSelc.x, calcSelc.y, 100.f};

				DUOLMath::Vector3 calcSelSphe = DUOLMath::Vector3{calcSelc.x, calcSelc.y, 50.f};

				DUOLMath::Matrix camWorldRot = DUOLMath::Matrix::CreateFromQuaternion(_orthographicCamera->GetTransform()->GetWorldRotation()).Invert();

				DUOLMath::Matrix lookMat = DUOLMath::Matrix::CreateScale(DUOLMath::Vector3{ 100.f, 100.f ,100.f }) * camWorldRot * DUOLMath::Matrix::CreateTranslation(calcSel)
					* _orthographicCamera->GetTransform()->GetWorldMatrix();
					
				DUOLMath::Matrix rightMat = DUOLMath::Matrix::CreateScale(DUOLMath::Vector3{ 100.f, 100.f ,100.f }) * DUOLMath::Matrix::CreateRotationY(DUOLMath::PI / 2) * camWorldRot * DUOLMath::Matrix::CreateTranslation(calcSel)
					*_orthographicCamera->GetTransform()->GetWorldMatrix();
					
				DUOLMath::Matrix upMat = DUOLMath::Matrix::CreateScale(DUOLMath::Vector3{ 100.f, 100.f ,100.f }) * DUOLMath::Matrix::CreateRotationX(-DUOLMath::PI / 2) * camWorldRot * DUOLMath::Matrix::CreateTranslation(calcSel)
					* _orthographicCamera->GetTransform()->GetWorldMatrix();

				DUOLMath::Matrix centerMat = DUOLMath::Matrix::CreateScale(DUOLMath::Vector3{ 10.f, 10.f,10.f}) * DUOLMath::Matrix::CreateTranslation(calcSelSphe)
				* _orthographicCamera->GetTransform()->GetWorldMatrix();

				_transformGizmoTransform[static_cast<int>(_currentTransformGizmoState) + 0]._world = lookMat;

				_transformGizmoTransform[static_cast<int>(_currentTransformGizmoState) + 0]._worldInvTranspose = lookMat.Invert().Transpose();

				_transformGizmoMeshInfos[static_cast<int>(_currentTransformGizmoState) + 0].SetObjectID(_selectedGameObject->GetUUID() + 1);
				
				_transformGizmoTransform[static_cast<int>(_currentTransformGizmoState) + 1]._world = rightMat;

				_transformGizmoTransform[static_cast<int>(_currentTransformGizmoState) + 1]._worldInvTranspose = rightMat.Invert().Transpose();

				_transformGizmoMeshInfos[static_cast<int>(_currentTransformGizmoState) + 1].SetObjectID(_selectedGameObject->GetUUID() + 2);

				_transformGizmoTransform[static_cast<int>(_currentTransformGizmoState) + 2]._world = upMat;

				_transformGizmoTransform[static_cast<int>(_currentTransformGizmoState) + 2]._worldInvTranspose = upMat.Invert().Transpose();

				_transformGizmoMeshInfos[static_cast<int>(_currentTransformGizmoState) + 2].SetObjectID(_selectedGameObject->GetUUID() + 3);

				_transformCenterGizmoTransform._world = centerMat;

				_transformCenterGizmoTransform._worldInvTranspose = centerMat.Invert().Transpose();

				_transformCenterGizmoMeshInfo.SetObjectID(_selectedGameObject->GetUUID());

				DUOLGameEngine::GraphicsManager::GetInstance()->ReserveRenderObject(&_transformGizmos[static_cast<int>(_currentTransformGizmoState) + 0]);

				DUOLGameEngine::GraphicsManager::GetInstance()->ReserveRenderObject(&_transformGizmos[static_cast<int>(_currentTransformGizmoState) + 1]);

				DUOLGameEngine::GraphicsManager::GetInstance()->ReserveRenderObject(&_transformGizmos[static_cast<int>(_currentTransformGizmoState) + 2]);

				DUOLGameEngine::GraphicsManager::GetInstance()->ReserveRenderObject(&_transformCenterGizmo);
			}
		}
#pragma endregion
	}

	void SceneView::TransformGizmoPicking(const DUOLMath::Vector2& currentTextureSize,
		const DUOLMath::Vector2& mousePosition)
	{
		uint64_t objectID = DUOLGameEngine::GraphicsManager::GetInstance()->FastPicking(currentTextureSize, mousePosition);

		// ������Ʈ �̸��� �ؽ� �Լ��� �����ϴ� ���� �������� .. �� ����� �ظ��ϸ� ������ ���̵�� üũ�� �� ���Ƽ� �����մϴ�.
		// Picked Look
		if (objectID == _selectedGameObject->GetUUID() + 1)
		{
			_selectedDirection = 1;
		}
		// Picked Right
		else if (objectID == _selectedGameObject->GetUUID() + 2)
		{
			_selectedDirection = 2;
		}
		// Pickied Up
		else if (objectID == _selectedGameObject->GetUUID() + 3)
		{
			_selectedDirection = 3;
		}
	}

	void SceneView::TransformGizmoUpdate(const DUOLMath::Vector2& currentMousePosition)
	{
		static DUOLMath::Vector2 prevMousePosition;

		DUOLMath::Vector2 prevToCur = prevMousePosition - currentMousePosition;

		// ���� ���� ���⿡ ���ؼ� Ʈ������ ������Ʈ


		// Look
		if (_selectedDirection == 1)
		{
			// Global Operation
			if (_isTransformOperationGlobal)
			{
				// ��ũ���� ����
				DUOLMath::Vector3 worldLook = DUOLMath::Vector3::Forward;

				DUOLMath::Vector3::TransformNormal(worldLook, _orthographicCamera->_viewMatrix * _orthographicCamera->_projectionMatrix);
			}
		}
		// Right
		else if (_selectedDirection == 2)
		{
			
		}
		// Up
		else if (_selectedDirection == 3)
		{
			
		}

		prevMousePosition = currentMousePosition;
	}

	void SceneView::Update(float deltaTime)
	{
		ControllableViewBase::Update(deltaTime);

		// 0. ������ Game View (== Game) �� �׷�����. �����س���
		DUOLGameEngine::GraphicsManager::GetInstance()->CopyTexture(TEXT("CopyGameView"), TEXT("GameView"));

		// 1. SceneView�� �̹����� ȣ���ؾ��� �ؽ�ó�� ������Ʈ�մϴ�.
		_image->_size = GetSafeSize();

#pragma region GameView�� �׸� �� Context�� �ʱ�ȭ���� �ʾұ� ������ Rendering, Lighting ���� �̺�Ʈ�� �� �� �� ����ų �ʿ䰡 �����ϴ� ..!
		// 1. SceneRender Event Invoke
		// DUOLGameEngine::EventManager::GetInstance()->InvokeEvent(TEXT("SceneRendering"));

		// 2. SceneLighting Event Invoke
		// DUOLGameEngine::EventManager::GetInstance()->InvokeEvent(TEXT("SceneLighting"));
#pragma endregion

		DUOLGameEngine::GraphicsManager::GetInstance()->UpdateRenderScreenSize(_image->_size);

		// 1-1. Perspective Gizmo Render Event
		RenderAllPerspectiveGizmo();

		// 3. Perspective Camera Info
		if (_perspectiveCamera != nullptr)
		{
			// Game View ȭ�� ����� �°� ī�޶� ���� ����
			_perspectiveCamera->OnResize(&_image->_size);

			DUOLGameEngine::GraphicsManager::GetInstance()->UpdateCameraInfo(&_perspectiveCamera->GetCameraInfo());
		}

		// 4. Execute - Perspective
		DUOLGameEngine::GraphicsManager::GetInstance()->Execute(TEXT("Scene"), true);

		if (GetIsHovered())
		{
			// Object ID Picking
			if (DUOLGameEngine::InputManager::GetInstance()->GetMouseButtonDown(DUOLGameEngine::MouseCode::Left))
			{
				// ���콺 ��ġ�� ��ũ�� ��ǥ�� �սô�.
				DUOLMath::Vector2 mousePosition = DUOLGameEngine::InputManager::GetInstance()->GetMousePositionInScreen();

				// ���� View �������� ���콺�� ��ġ�� �Ű� ��� ��ġ�� �ȼ��� ���콺�� �ö󰬴��� �˻��մϴ�.
				mousePosition.x -= _position.x;

				mousePosition.y = mousePosition.y - _position.y - ImGui::GetFrameHeight();

				ObjectPicking(_image->_size, mousePosition);
			}
		}

		// 5 - 0. Orthographic Camera Info
		//if (_orthographicCamera != nullptr)
		//{
		//	DUOLMath::Vector2 screenSize = DUOLGameEngine::GraphicsManager::GetInstance()->GetScreenSize();

		//	// Game View ȭ�� ����� �°� ī�޶� ���� ����
		//	_orthographicCamera->OnResize(&screenSize);

		//	DUOLGameEngine::GraphicsManager::GetInstance()->UpdateCameraInfo(&_orthographicCamera->GetCameraInfo());
		//}

		// ����, ���� ���õ� ���� ������Ʈ�� �ִٸ� �ƿ� ���ΰ�
		// ���� ���۷��̼ǿ� �´� ����� �׷��ݴϴ�.
		if (_selectedGameObject != nullptr)
		{
			RenderOutline();

			// RenderTransformGizmo();

			if (GetIsHovered())
			{
				// ���콺 ��ġ�� ��ũ�� ��ǥ�� �սô�.
				DUOLMath::Vector2 mousePosition = DUOLGameEngine::InputManager::GetInstance()->GetMousePositionInScreen();

				// ���� View �������� ���콺�� ��ġ�� �Ű� ��� ��ġ�� �ȼ��� ���콺�� �ö󰬴��� �˻��մϴ�.
				mousePosition.x -= _position.x;

				mousePosition.y = mousePosition.y - _position.y - ImGui::GetFrameHeight();

				// Transform Gizmo Picking (�����ų�)
				if (DUOLGameEngine::InputManager::GetInstance()->GetMouseButtonDown(DUOLGameEngine::MouseCode::Left))
				{
					TransformGizmoPicking(_image->_size, mousePosition);

					TransformGizmoUpdate(mousePosition);
				}

				if (DUOLGameEngine::InputManager::GetInstance()->GetMouseButtonPressed(DUOLGameEngine::MouseCode::Left))
				{
					TransformGizmoUpdate(mousePosition);
				}

				// ���� ���콺�� �ö���� ���õ� ������ �����ݴϴ�.
				if (DUOLGameEngine::InputManager::GetInstance()->GetMouseButtonUp(DUOLGameEngine::MouseCode::Left))
				{
					_selectedDirection = 0;
				}
			}
			else
				_selectedDirection = 0;
		}

		// Albedo, Depth, ... �� ����ϱ� ���ؼ� Default Pipeline�� ���Ǵ� GBuffer �� Depth�� Clear �մϴ�.
		// DUOLGameEngine::GraphicsManager::GetInstance()->ClearRenderingPipelineSetups(TEXT("Default"));

		// Gizmo Execute
		// DUOLGameEngine::GraphicsManager::GetInstance()->Execute(TEXT("SceneView_Gizmo"), true, false);

		// SceneView panel�� �������� Object�� �׸��ϴ�.
		_image->_textureID = DUOLGameEngine::GraphicsManager::GetInstance()->GetShaderResourceAddress(TEXT("SceneView"));
	}
}