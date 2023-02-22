#pragma once
#include "DUOLEditor/Modules/ControllableViewBase.h"
#include "DUOLEditor/UI/Gizmo/ImGuizmo.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/RenderObject.h"

namespace DUOLEditor
{
	/**
	 * \brief 레벨을 구성할 때 참조할 수 있는 게임 공간 화면을 나타내는 UI Object.
	 */
	class SceneView : public DUOLEditor::ControllableViewBase
	{
	public:
		SceneView(const DUOLCommon::tstring& title, bool isOpened, const PanelWindowSetting& windowSetting);

		virtual ~SceneView() override;

	private:
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
