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
	enum class GameObjectGizmoState
	{
		Scale
		, Rotation
		, Translation
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
		 * \brief 선택된 게임 오브젝트가 있다면 아웃라인을 그립니다.
		 */
		void RenderOutline();

		/**
		 * \brief GraphicsManager 렌더 큐에 Gizmo 를 그릴 수 있는 명령을 추가합니다.
		 */
		void RenderAllGizmo();

	public:
		/**
		 * \brief 해당 View를 그리기 전에 Update를 수행합니다.
		 * \param deltaTime 프레임 시간 간격입니다.
		 */
		virtual void Update(float deltaTime) override;
 	};
}
