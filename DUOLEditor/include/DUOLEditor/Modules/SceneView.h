#pragma once
#include "DUOLEditor/Modules/ControllableViewBase.h"
#include "DUOLEditor/UI/Gizmo/ImGuizmo.h"
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
		/**
		 * \brief 선택된 게임 오브젝트가 있다면 아웃라인을 그립니다.
		 */
		void RenderOutline();

		void ObjectPicking_SceneView(const DUOLMath::Vector2& currentTextureSize,
			const DUOLMath::Vector2& mousePosition);

	private:
		IMGUIZMO_NAMESPACE::OPERATION _currentOperation;

		IMGUIZMO_NAMESPACE::MODE _currentMode;

	public:
		/**
		 * \brief 해당 View를 그리기 전에 Update를 수행합니다.
		 * \param deltaTime 프레임 시간 간격입니다.
		 */
		virtual void Update(float deltaTime) override;
	};
}
