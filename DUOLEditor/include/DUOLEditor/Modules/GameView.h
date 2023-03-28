#pragma once
#include "DUOLEditor/Modules/ViewBase.h"

namespace DUOLEditor
{
	/**
	 * \brief ���� ���� �ɼǿ� ���� �������� Ȯ���� �� �ִ� UI Object.
	 */
	class GameView : public DUOLEditor::ViewBase
	{
	public:
		GameView(const DUOLCommon::tstring& title, bool isOpened, const PanelWindowSetting& windowSetting);

		virtual ~GameView() override;

	private:
		bool _isInGameMode;

	public:
		virtual void Update(float deltaTime) override;
	};
}