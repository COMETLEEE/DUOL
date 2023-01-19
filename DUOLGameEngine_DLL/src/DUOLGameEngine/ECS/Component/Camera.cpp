#include "DUOLGameEngine/ECS/Component/Camera.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/Manager/GraphicsManager.h"

#include "DUOLGameEngine/Engine.h"

namespace DUOLGameEngine
{
	std::shared_ptr<DUOLGameEngine::Camera> Camera::_mainCamera = nullptr;

	Camera::Camera(const std::weak_ptr<DUOLGameEngine::GameObject>& owner, const DUOLCommon::tstring& name) :
		BehaviourBase(owner, name)
		, enable_shared_from_base<DUOLGameEngine::Camera, DUOLGameEngine::BehaviourBase>()
		, _cameraInfo(DUOLGraphicsEngine::Camera())
		, _nearClipPlane(1.f)
		, _farClipPlane(500.f)
		, _fieldOfView(45.0f)
		, _isOrthographics(false)
		, _orthographicSize(0.f)
		, _useOcclusionCulling(false)
		, _aspectRatio(1.f)
	{
		// Tag
		GetGameObject()->SetTag(TEXT("MainCamera"));

		// size check and init cam properties.
		const DUOLMath::Vector2& screenSize = GraphicsManager::GetInstance()->GetScreenSize();

		if (screenSize.y != 0)
			_aspectRatio = static_cast<float>(screenSize.x) / static_cast<float>(screenSize.y);

		// Projection Matrix
		UpdateProjectionMatrix();
	}

	Camera::~Camera()
	{
		
	}

	void Camera::OnResize(std::any screenSize)
	{
		DUOLMath::Vector2* screen = std::any_cast<DUOLMath::Vector2*>(screenSize);

		if (screen->x == 0.f || screen->y == 0.f)
			return;

		_aspectRatio = screen->x / screen->y;

		_orthographicSize = screen->y / 2.f;

		UpdateProjectionMatrix();
	}

	void Camera::UpdateProjectionMatrix()
	{
		if (!_isOrthographics)
		{
			_projectionMatrix = 
				Matrix::CreatePerspectiveFieldOfView(_fieldOfView, _aspectRatio, _nearClipPlane, _farClipPlane);
		}
		else
		{
			_projectionMatrix =
				Matrix::CreateOrthographic((_orthographicSize * _aspectRatio) * 2.f, _orthographicSize * 2.f, _nearClipPlane, _farClipPlane);
		}
	}

	void Camera::OnEnable()
	{
		BehaviourBase::OnEnable();

		// Graphics Engine에 OnResize Event Handler register.
		const std::function<void(std::any)> functor =
			std::bind(&Camera::OnResize, this, std::placeholders::_1);

		_onResizeListenerIDForGraphics = EventManager::GetInstance()->AddEventFunction(TEXT("Resize"), functor);
		
		if (_mainCamera == nullptr)
		{
			// Main Camera cache.
			_mainCamera = this->shared_from_base();
		}
	}

	void Camera::OnDisable()
	{
		BehaviourBase::OnDisable();

		EventManager::GetInstance()->RemoveEventFunction<std::any>(TEXT("Resize"), _onResizeListenerIDForGraphics);
		
		if ((_mainCamera != nullptr) && (_mainCamera.get() == this))
		{
			_mainCamera.reset();

			_mainCamera = nullptr;
		}
	}

	const DUOLGraphicsEngine::Camera& Camera::GetCameraInfo()
	{
		// TODO : Transform의 변동이 있는 경우에만 ViewMatrix 관련 Update 할 수 있도록 한다 ..!
		_viewMatrix = GetTransform()->GetWorldMatrix().Invert();

		_cameraInfo._projectionMatrix = _projectionMatrix;

		_cameraInfo._viewMatrix = _viewMatrix;

		_cameraInfo._viewProjectionMatrix = _cameraInfo._viewMatrix * _cameraInfo._projectionMatrix;

		_cameraInfo._viewProjectionInverseTransposeMatrix = ((_cameraInfo._viewProjectionMatrix).Invert()).Transpose();

		_cameraInfo._cameraPosition = GetTransform()->GetWorldPosition();

		_cameraInfo._cameraFar = _farClipPlane;

		_cameraInfo._cameraNear = _nearClipPlane;

		_cameraInfo._cameraVerticalFOV = _fieldOfView;

		_cameraInfo._aspectRatio = _aspectRatio;

		return _cameraInfo;
	}
}