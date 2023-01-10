#pragma once
#include "DUOLEditor/Modules/ViewBase.h"

namespace DUOLEditor
{
	/**
	 * \brief ������ ������ �� ������ �� �ִ� ���� ���� ȭ���� ��Ÿ���� UI Object.
	 */
	class SceneView : public DUOLEditor::ViewBase
	{
	public:
		SceneView(const DUOLCommon::tstring& title, bool isOpened, const PanelWindowSetting& windowSetting);

		virtual ~SceneView() override;

	public:
		// ���� ���۷��̼ǿ� ���� ����� ������
		// �׸��� ������ ���� .. ���� ���� �������� �˾Ƽ� �Ѵ� .. �� ���ô� �ϴ�.
		void OnSceneTextureChanged(void* textureID);
 	};
}
