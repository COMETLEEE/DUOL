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
		, _farClipPlane(100.f)
		, _fieldOfView(45.0f)
		, _isOrthographics(false)
		, _orthographicSize(0.f)
		, _useOcclusionCulling(false)
	{
		// Tag
		GetGameObject()->SetTag(TEXT("MainCamera"));

		// size check and init cam properties.
		const uint32_t& screenWidth = GraphicsManager::GetInstance()->GetScreenWidth();
		const uint32_t& screenHeight = GraphicsManager::GetInstance()->GetScreenHeight();

		_aspectRatio = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);

		// Projection Matrix
		UpdateProjectionMatrix();
	}

	Camera::~Camera()
	{
		
	}

	void Camera::OnResize(const uint32_t& screenWidth, const uint32_t& screenHeight)
	{
		if (screenHeight == 0)
			return;

		_aspectRatio = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);

		_orthographicSize = static_cast<float>(screenHeight) / 2.f;

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
		const std::function<void(const uint32_t&, const uint32_t&)> functor =
			std::bind(&Camera::OnResize, this, std::placeholders::_1, std::placeholders::_2);

		_onResizeListenerIDForGraphics = GraphicsManager::GetInstance()->GetOnResizeEvent().AddListener(functor);

		if (_mainCamera == nullptr)
		{
			// Main Camera cache.
			_mainCamera = this->shared_from_base();
		}
	}

	void Camera::OnDisable()
	{
		BehaviourBase::OnDisable();

		GraphicsManager::GetInstance()->GetOnResizeEvent().RemoveListener(_onResizeListenerIDForGraphics);

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

		return _cameraInfo;
	}
}