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
	 * \brief ���� ������Ʈ�� ���� Operator state.
	 */
	enum class TransformGizmoState
	{
		Scale = 0
		, Rotate = 3
		, Translate = 6
	};

	/**
	 * \brief Ʈ������ ����� �� ���õ� ���� ��Ÿ���ϴ�.
	 */
	enum class TransformGizmoSelectedAxis
	{
		Look = 1
		, Right = 2
		, Up = 3
		, None = 0
	};

	/**
	 * \brief ������ ������ �� ������ �� �ִ� ���� ���� ȭ���� ��Ÿ���� UI Object.
	 */
	class SceneView : public DUOLEditor::ControllableViewBase
	{
	public:
		SceneView(const DUOLCommon::tstring& title, bool isOpened, const PanelWindowSetting& windowSetting);

		virtual ~SceneView() override;

	private:

		// TODO : �� �ϳ��� Behavior �� ���� �� ���� �� �����ϴ� !
#pragma region TRANSFORM_GIZMO
		/**
		 * \brief ���� ���۷��̼� ���
		 */
		DUOLEditor::TransformGizmoState	_currentTransformGizmoState;

		/**
		 * \brief ���� Ʈ������ ���۷��̼� ������ �۷ι��ΰ��� ?
		 */
		bool _isTransformOperationGlobal;

		// 0 : X , 1 : Y , 2 : Z => 3���� Scale, Rotate, Translate
		DUOLGraphicsEngine::RenderObject	_transformGizmos[9];

		DUOLGraphicsEngine::RenderObject	_transformCenterGizmo;

		// 0 : X , 1 : Y , 2 : Z => 3���� Scale, Rotate, Translate
		DUOLGraphicsEngine::MeshInfo		_transformGizmoMeshInfos[9];

		DUOLGraphicsEngine::MeshInfo		_transformCenterGizmoMeshInfo;

		// 0 : X , 1 : Y , 2 : Z => 3���� Scale, Rotate, Translate
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
		 * \brief ���õ� ���� ������Ʈ�� �ִٸ� �ƿ������� �׸��ϴ�.
		 */
		void RenderOutline();

		/**
		 * \brief GraphicsManager ���� ť�� Gizmo �� �׸� �� �ִ� ����� �߰��մϴ�.
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
		 * \brief �ش� View�� �׸��� ���� Update�� �����մϴ�.
		 * \param deltaTime ������ �ð� �����Դϴ�.
		 */
		virtual void Update(float deltaTime) override;
	};
}
