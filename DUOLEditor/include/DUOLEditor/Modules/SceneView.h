#pragma once
#include "DUOLEditor/Modules/ControllableViewBase.h"
#include "DUOLEditor/UI/Gizmo/ImGuizmo.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/RenderObject.h"

namespace DUOLEditor
{
	/**
	 * \brief ������ ������ �� ������ �� �ִ� ���� ���� ȭ���� ��Ÿ���� UI Object.
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
		 * \brief �ش� View�� �׸��� ���� Update�� �����մϴ�.
		 * \param deltaTime ������ �ð� �����Դϴ�.
		 */
		virtual void Update(float deltaTime) override;
	};
}
