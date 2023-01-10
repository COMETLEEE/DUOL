#pragma once
#include "DUOLEditor/Modules/ControllableViewBase.h"

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

	public:
		/**
		 * \brief �ش� View�� �׸��� ���� Update�� �����մϴ�.
		 * \param deltaTime ������ �ð� �����Դϴ�.
		 */
		virtual void Update(float deltaTime) override;
 	};
}