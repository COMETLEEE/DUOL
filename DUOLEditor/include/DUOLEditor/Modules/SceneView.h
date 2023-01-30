#pragma once
#include "DUOLEditor/Modules/ControllableViewBase.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/RenderObject.h"

namespace DUOLEditor
{
	constexpr uint32_t MAX_CAMERA_GIZMO = 10;

	constexpr uint32_t MAX_DIRECTIONAL_LIGHT_GIZMO = 10;

	constexpr uint32_t MAX_POINT_LIGHT_GIZMO = 20;

	constexpr uint32_t MAX_SPOT_LIGHT_GIZMO = 20;

	/**
	 * \brief 게임 오브젝트의 현재 Operator state.
	 */
	enum class TransformGizmoState
	{
		Scale = 0
		, Rotate = 3
		, Translate = 6
	};

	/**
	 * \brief 트랜스폼 기즈모 중 선택된 축을 나타냅니다.
	 */
	enum class TransformGizmoSelectedAxis
	{
		Look = 1
		, Right = 2
		, Up = 3
		, None = 0
	};

	/**
	 * \brief 레벨을 구성할 때 참조할 수 있는 게임 공간 화면을 나타내는 UI Object.
	 */
	class SceneView : public DUOLEditor::ControllableViewBase
	{
	public:
		SceneView(const DUOLCommon::tstring& title, bool isOpened, const PanelWindowSetting& windowSetting);

		virtual ~SceneView() override;

	private:

		// TODO : 다 하나의 Behavior 로 나눌 수 있을 것 같습니다 !
#pragma region TRANSFORM_GIZMO
		/**
		 * \brief 현재 오퍼레이션 모드
		 */
		DUOLEditor::TransformGizmoState	_currentTransformGizmoState;

		/**
		 * \brief 현재 트랜스폼 오퍼레이션 기준이 글로벌인가요 ?
		 */
		bool _isTransformOperationGlobal;

		// 0 : X , 1 : Y , 2 : Z => 3개씩 Scale, Rotate, Translate
		DUOLGraphicsEngine::RenderObject	_transformGizmos[9];

		DUOLGraphicsEngine::RenderObject	_transformCenterGizmo;

		// 0 : X , 1 : Y , 2 : Z => 3개씩 Scale, Rotate, Translate
		DUOLGraphicsEngine::MeshInfo		_transformGizmoMeshInfos[9];

		DUOLGraphicsEngine::MeshInfo		_transformCenterGizmoMeshInfo;

		// 0 : X , 1 : Y , 2 : Z => 3개씩 Scale, Rotate, Translate
		DUOLGraphicsEngine::Transform		_transformGizmoTransform[9];

		DUOLGraphicsEngine::Transform		_transformCenterGizmoTransform;

		std::vector<DUOLGraphicsEngine::Material*> _transformRedMaterials;

		std::vector<DUOLGraphicsEngine::Material*> _transformGreenMaterials;

		std::vector<DUOLGraphicsEngine::Material*> _transformBlueMaterials;

		std::vector<DUOLGraphicsEngine::Material*> _transformYellowMaterials;

		std::vector<DUOLGraphicsEngine::Material*> _transformGrayMaterials;

		std::vector<DUOLGraphicsEngine::Material*> _transformCenterMaterials;
#pragma endregion

#pragma region CAMERA_GIZMO
		DUOLGraphicsEngine::RenderObject	_cameraGizmos[MAX_CAMERA_GIZMO];

		DUOLGraphicsEngine::MeshInfo		_cameraGizmoMeshInfos[MAX_CAMERA_GIZMO];

		DUOLGraphicsEngine::Transform		_cameraGizmoTransform[MAX_CAMERA_GIZMO];

		std::vector<DUOLGraphicsEngine::Material*> _cameraGizmoMaterials;
#pragma endregion

#pragma region LIGHT_GIZMO
		DUOLGraphicsEngine::RenderObject	_dLightGizmos[MAX_DIRECTIONAL_LIGHT_GIZMO];

		DUOLGraphicsEngine::MeshInfo		_dLightGizmoMeshInfos[MAX_DIRECTIONAL_LIGHT_GIZMO];

		DUOLGraphicsEngine::RenderObject	_sLightGizmos[MAX_SPOT_LIGHT_GIZMO];

		DUOLGraphicsEngine::MeshInfo		_sLightGizmoMeshInfos[MAX_SPOT_LIGHT_GIZMO];

		DUOLGraphicsEngine::RenderObject	_pLightGizmos[MAX_POINT_LIGHT_GIZMO];

		DUOLGraphicsEngine::MeshInfo		_pLightGizmoMeshInfos[MAX_POINT_LIGHT_GIZMO];
#pragma endregion

		/**
		 * \brief 선택된 게임 오브젝트가 있다면 아웃라인을 그립니다.
		 */
		void RenderOutline();

		/**
		 * \brief GraphicsManager 렌더 큐에 Gizmo 를 그릴 수 있는 명령을 추가합니다.
		 */
		void RenderAllPerspectiveGizmo();

		void RenderTransformGizmo();

		// 1 : Look , 2 : Right , 3 : Up
		DUOLEditor::TransformGizmoSelectedAxis _selectedAxis;

		void TransformGizmoUpdate();

		void ObjectPicking_SceneView(const DUOLMath::Vector2& currentTextureSize,
			const DUOLMath::Vector2& mousePosition);

	public:
		/**
		 * \brief 해당 View를 그리기 전에 Update를 수행합니다.
		 * \param deltaTime 프레임 시간 간격입니다.
		 */
		virtual void Update(float deltaTime) override;
	};
}
