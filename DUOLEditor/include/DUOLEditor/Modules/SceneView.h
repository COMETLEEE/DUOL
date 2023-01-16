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
	enum class GameObjectGizmoState
	{
		Scale
		, Rotation
		, Translation
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
		DUOLEditor::GameObjectGizmoState	_gizmoState;

		DUOLGraphicsEngine::RenderObject	_cameraGizmos[MAX_CAMERA_GIZMO];

		DUOLGraphicsEngine::MeshInfo		_cameraGizmoMeshInfos[MAX_CAMERA_GIZMO];

		DUOLGraphicsEngine::Transform		_cameraGizmoTransform[MAX_CAMERA_GIZMO];

		std::vector<DUOLGraphicsEngine::Material*> _cameraGizmoMaterials;

		DUOLGraphicsEngine::RenderObject	_dLightGizmos[MAX_DIRECTIONAL_LIGHT_GIZMO];

		DUOLGraphicsEngine::MeshInfo		_dLightGizmoMeshInfos[MAX_DIRECTIONAL_LIGHT_GIZMO];

		DUOLGraphicsEngine::RenderObject	_sLightGizmos[MAX_SPOT_LIGHT_GIZMO];

		DUOLGraphicsEngine::MeshInfo		_sLightGizmoMeshInfos[MAX_SPOT_LIGHT_GIZMO];

		DUOLGraphicsEngine::RenderObject	_pLightGizmos[MAX_POINT_LIGHT_GIZMO];

		DUOLGraphicsEngine::MeshInfo		_pLightGizmoMeshInfos[MAX_POINT_LIGHT_GIZMO];


		/**
		 * \brief ���õ� ���� ������Ʈ�� �ִٸ� �ƿ������� �׸��ϴ�.
		 */
		void RenderOutline();

		/**
		 * \brief GraphicsManager ���� ť�� Gizmo �� �׸� �� �ִ� ����� �߰��մϴ�.
		 */
		void RenderAllGizmo();

	public:
		/**
		 * \brief �ش� View�� �׸��� ���� Update�� �����մϴ�.
		 * \param deltaTime ������ �ð� �����Դϴ�.
		 */
		virtual void Update(float deltaTime) override;
 	};
}
