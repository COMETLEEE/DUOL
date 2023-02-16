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
		/**
		 * \brief ���õ� ���� ������Ʈ�� �ִٸ� �ƿ������� �׸��ϴ�.
		 */
		void RenderOutline();

		void ObjectPicking_SceneView(const DUOLMath::Vector2& currentTextureSize,
			const DUOLMath::Vector2& mousePosition);

	private:
		IMGUIZMO_NAMESPACE::OPERATION _currentOperation;

		IMGUIZMO_NAMESPACE::MODE _currentMode;

	public:
		/**
		 * \brief �ش� View�� �׸��� ���� Update�� �����մϴ�.
		 * \param deltaTime ������ �ð� �����Դϴ�.
		 */
		virtual void Update(float deltaTime) override;
	};
}
