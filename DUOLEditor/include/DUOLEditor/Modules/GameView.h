#pragma once
#include "DUOLEditor/Modules/ViewBase.h"

namespace DUOLEditor
{
	/**
	 * \brief 현재 게임 옵션에 따른 렌더링을 확인할 수 있는 UI Object.
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