#pragma once
#include "DUOLGameEngine/ECS/Component/BehaviourBase.h"

#include "DUOLGraphicsEngine/ResourceManager/Resource/RenderContantBuffer.h"

#include "DUOLCommon/Event/Event.h"

namespace DUOLGameEngine
{
	class Camera final : public DUOLGameEngine::BehaviourBase,
		public DUOLGameEngine::enable_shared_from_base<Camera, BehaviourBase>
	{
	public:
		Camera(const std::weak_ptr<DUOLGameEngine::GameObject>& owner, const DUOLCommon::tstring& name = TEXT("Camera"));

		virtual ~Camera();

	private:
		/**
		 * \brief Main camera of the scene. use this camera's property when rendering.
		 */
		static std::shared_ptr<DUOLGameEngine::Camera> _mainCamera;

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

	private:
		static std::shared_ptr<DUOLGameEngine::Camera> GetMainCamera() { return _mainCamera; }

		DUOLCommon::EventListenerID _onResizeListenerIDForGraphics;

		void OnResize(const uint32_t& screenWidth, const uint32_t& screenHeight);

		void UpdateProjectionMatrix();

	public:
		virtual void OnEnable() override;

		virtual void OnDisable() override;

		const DUOLGraphicsEngine::Camera& GetCameraInfo();

#pragma region FRIEND_CLASS
		friend class GraphicsManager;
#pragma endregion
	};
}