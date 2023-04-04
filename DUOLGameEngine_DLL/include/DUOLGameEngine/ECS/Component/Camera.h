#pragma once
#include <any>

#include "DUOLGameEngine/ECS/Component/BehaviourBase.h"

#include "DUOLGraphicsEngine/ResourceManager/Resource/RenderConstantBuffer.h"

#include "DUOLCommon/Event/Event.h"

namespace DUOLEditor
{
	class SceneView;
	class GameView;
}

namespace DUOLGameEngine
{
	enum class CameraProjection
	{
		Perspective
		, Orthographic
	};

	class DUOL_GAMEENGINE_API Camera final : public DUOLGameEngine::BehaviourBase,
		public DUOLGameEngine::enable_shared_from_base<Camera, BehaviourBase>
	{
	public:
		Camera();

		Camera(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name = TEXT("Camera"));

		virtual ~Camera();

	private:
		/**
		 * \brief Main camera of the scene. use this camera's property when rendering.
		 */
		static DUOLGameEngine::Camera* _mainCamera;

		/**
		 * \brief Per frame information of Camera to send graphics engine.
		 */
		DUOLGraphicsEngine::Camera _cameraInfo;

		/**
		 * \brief The distance of the near clipping plane from the Camera, in world units.
		 */
		float _nearClipPlane;

		/**
		 * \brief The distance of the far clipping plane from the Camera, in world units.
		 */
		float _farClipPlane;

		/**
		 * \brief The vertical field of view of the Camera, in degrees.
		 */
		float _fieldOfView;

		/**
		 * \brief The aspect ratio. (width divided by height.)
		 */
		float _aspectRatio;

		/**
		 * \brief Is the camera orthographic (true) or perspective (false) ?
		 */
		bool _isOrthographics;

		/**
		 * \brief Camera's half size of the vertical viewing volume when in orthographic mode. 
		 */
		float _orthographicSize;

		/**
		 * \brief Whether or not the Camera will use occlusion culling during rendering.
		 */
		bool _useOcclusionCulling;

		DUOLMath::Matrix _projectionMatrix;

		DUOLMath::Matrix _viewMatrix;

		CameraProjection _cameraProjection;

	public:
		float GetNear() const;

		void SetNear(float value);

		float GetFar() const;

		void SetFar(float value);

		float GetFOV() const;

		void SetFOV(float value);

		bool GetIsOrthographic() const;

		void SetIsOrthographic(bool isOrthographics);

		bool GetUseOcclusionCulling() const;

		void SetUseOcclusionCulling(bool value);

		static void SetMainCamera(DUOLGameEngine::Camera* camera);

	private:
		CameraProjection GetCameraProjection() const;

		void SetCameraProjection(CameraProjection projection);

	private:
		DUOLCommon::EventListenerID _onResizeListenerIDForGraphics;

		static DUOLGameEngine::Camera* GetMainCamera() { return _mainCamera; }

		void OnResize(std::any screenSize);

		void UpdateProjectionMatrix();

	public:
		virtual void OnEnable() override;

		virtual void OnDisable() override;

		const DUOLGraphicsEngine::Camera& GetCameraInfo();

		RTTR_ENABLE(BehaviourBase)

		RTTR_REGISTRATION_FRIEND

#pragma region FRIEND_CLASS
		friend class GraphicsManager;

		friend class DUOLEditor::GameView;

		friend class DUOLEditor::SceneView;
#pragma endregion
	};
}
