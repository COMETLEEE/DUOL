#include "DUOLGameEngine/ECS/Component/Camera.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/Manager/GraphicsManager.h"

#include "DUOLGameEngine/Engine.h"

#include <rttr/policy.h>
#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"

using namespace rttr;

RTTR_PLUGIN_REGISTRATION
{
	rttr::registration::enumeration<DUOLGameEngine::CameraProjection>("CameraProjection")
	(
		value("Perspective", DUOLGameEngine::CameraProjection::Perspective)
		, value("Orthographic", DUOLGameEngine::CameraProjection::Orthographic)
	);

	rttr::registration::class_<DUOLGameEngine::Camera>("Camera")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		// 조금 위험하긴 한데 .. 사용해보자
		rttr::policy::ctor::as_raw_ptr
	)
	.property("Near", &DUOLGameEngine::Camera::GetNear, &DUOLGameEngine::Camera::SetNear)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
	)
	.property("Far", &DUOLGameEngine::Camera::GetFar, &DUOLGameEngine::Camera::SetFar)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
	)
	.property("VFOV", &DUOLGameEngine::Camera::GetFOV, &DUOLGameEngine::Camera::SetFOV)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
	)
	.property("Projection", &DUOLGameEngine::Camera::GetCameraProjection, &DUOLGameEngine::Camera::SetCameraProjection)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Enumeration)
		)
	.property("OcclusionCulling", &DUOLGameEngine::Camera::GetUseOcclusionCulling, &DUOLGameEngine::Camera::SetUseOcclusionCulling)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		, metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Bool)
	);
}

namespace DUOLGameEngine
{
	DUOLGameEngine::Camera* Camera::_mainCamera = nullptr;

	Camera::Camera() :
		BehaviourBase(nullptr, TEXT("Camera"))
		, enable_shared_from_base<DUOLGameEngine::Camera, DUOLGameEngine::BehaviourBase>()
		, _cameraInfo(DUOLGraphicsEngine::Camera())
		, _nearClipPlane(0.5f)
		, _farClipPlane(500.f)
		, _fieldOfView(45.0f)
		, _isOrthographics(false)
		, _cameraProjection(CameraProjection::Perspective)
		, _orthographicSize(0.f)
		, _useOcclusionCulling(false)
		, _aspectRatio(1.f)
	{
		// size check and init cam properties.
		const DUOLMath::Vector2& screenSize = GraphicsManager::GetInstance()->GetScreenSize();

		if (screenSize.y != 0)
			_aspectRatio = static_cast<float>(screenSize.x) / static_cast<float>(screenSize.y);

		// Projection Matrix
		UpdateProjectionMatrix();
	}

	Camera::Camera(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		BehaviourBase(owner, name)
		, enable_shared_from_base<DUOLGameEngine::Camera, DUOLGameEngine::BehaviourBase>()
		, _cameraInfo(DUOLGraphicsEngine::Camera())
		, _nearClipPlane(0.5f)
		, _farClipPlane(500.f)
		, _fieldOfView(70.f)
		, _isOrthographics(false)
		, _cameraProjection(CameraProjection::Perspective)
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
		BehaviourBase::OnDisable();

		EventManager::GetInstance()->RemoveEventFunction<std::any>(TEXT("Resize"), _onResizeListenerIDForGraphics);

		if ((_mainCamera != nullptr) && (_mainCamera == this))
		{
			_mainCamera = nullptr;
		}
	}

	float Camera::GetNear() const
	{
		return _nearClipPlane;
	}

	void Camera::SetNear(float value)
	{
		// 최소 Near : 0.5f
		_nearClipPlane = value > 0.5f ? value : 0.5f;

		if (_nearClipPlane >= _farClipPlane)
			_farClipPlane = _nearClipPlane + 1.f;

		UpdateProjectionMatrix();
	}

	float Camera::GetFar() const
	{
		return _farClipPlane;
	}

	void Camera::SetFar(float value)
	{
		_farClipPlane = value;

		if (_farClipPlane <= _nearClipPlane)
			_nearClipPlane = _farClipPlane - 1.f;

		UpdateProjectionMatrix();
	}

	float Camera::GetFOV() const
	{
		return _fieldOfView;
	}

	void Camera::SetFOV(float value)
	{
		_fieldOfView = value;

		if (_fieldOfView < 0.001f)
			_fieldOfView = 0.001f;
		else if (_fieldOfView > 179.9f)
			_fieldOfView = 179.9f;

		UpdateProjectionMatrix();
	}

	bool Camera::GetIsOrthographic() const
	{
		return _isOrthographics;
	}

	void Camera::SetIsOrthographic(bool isOrthographics)
	{
		_isOrthographics = isOrthographics;

		if (_isOrthographics)
		{
			_cameraProjection = CameraProjection::Orthographic;
		}
		else
		{
			_cameraProjection = CameraProjection::Perspective;
		}

		UpdateProjectionMatrix();
	}

	bool Camera::GetUseOcclusionCulling() const
	{
		return _useOcclusionCulling;
	}

	void Camera::SetUseOcclusionCulling(bool value)
	{
		if (_useOcclusionCulling == value)
			return;

		_useOcclusionCulling = value;
	}

	void Camera::SetMainCamera(DUOLGameEngine::Camera* camera)
	{
		Camera::_mainCamera = camera;
	}

	CameraProjection Camera::GetCameraProjection() const
	{
		return _cameraProjection;
	}

	void Camera::SetCameraProjection(CameraProjection projection)
	{
		if (_cameraProjection == projection)
			return;

		_cameraProjection = projection;

		if (_cameraProjection == CameraProjection::Orthographic)
		{
			_isOrthographics = true;
		}
		else
		{
			_isOrthographics = false;
		}

		UpdateProjectionMatrix();
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
				Matrix::CreatePerspectiveFieldOfView(DUOLMath::MathHelper::DegreeToRadian(_fieldOfView), _aspectRatio, _nearClipPlane, _farClipPlane);
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
			_mainCamera = this;
		}
	}

	void Camera::OnDisable()
	{
		BehaviourBase::OnDisable();

		EventManager::GetInstance()->RemoveEventFunction<std::any>(TEXT("Resize"), _onResizeListenerIDForGraphics);
		
		if ((_mainCamera != nullptr) && (_mainCamera == this))
		{
			_mainCamera = nullptr;
		}
	}
	
	const DUOLGraphicsEngine::Camera& Camera::GetCameraInfo()
	{
		// TODO : Transform의 변동이 있는 경우에만 ViewMatrix 관련 Update 할 수 있도록 한다 ..!
		auto transform = GetTransform();
		auto& worldPos =transform->GetWorldPosition();
		auto& up = transform->GetUp();
		auto& look = transform->GetLook();

		// 이전 정보
		_cameraInfo._prevViewMatrix = _cameraInfo._viewMatrix;

		_cameraInfo._prevProjectionMatrix = _cameraInfo._projectionMatrix;

		// 현재 정보
		_cameraInfo._projectionMatrix = _projectionMatrix;

		// 뷰 매트릭스를 다시 갱신합니다.
		_viewMatrix = Matrix::CreateLookAt(worldPos, worldPos + look, up);

		_cameraInfo._viewMatrix = _viewMatrix;

		_cameraInfo._inverseProjectionMatrix = _projectionMatrix.Invert();

		_cameraInfo._viewProjectionMatrix = _cameraInfo._viewMatrix * _cameraInfo._projectionMatrix;

		_cameraInfo._viewProjectionInverseTransposeMatrix = ((_cameraInfo._viewProjectionMatrix).Invert()).Transpose();

		_cameraInfo._cameraPosition = GetTransform()->GetWorldPosition();

		_cameraInfo._cameraFar = _farClipPlane;

		_cameraInfo._cameraNear = _nearClipPlane;

		// 라디안으로 보낸다 ..!
		_cameraInfo._cameraVerticalFOV = DUOLMath::MathHelper::DegreeToRadian(_fieldOfView);

		_cameraInfo._aspectRatio = _aspectRatio;

		return _cameraInfo;
	}
}