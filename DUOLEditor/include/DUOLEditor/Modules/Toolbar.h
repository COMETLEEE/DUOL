#pragma once
#include "DUOLEditor/UI/Panels/PanelWindow.h"

namespace DUOLEditor
{
	class ButtonImage;
}

namespace DUOLEditor
{
	class Toolbar : public DUOLEditor::PanelWindow
	{
	public:
		Toolbar(const DUOLCommon::tstring& title, bool isOpened, const DUOLEditor::PanelWindowSetting& windowSetting);

		virtual ~Toolbar();

		virtual void Draw_Impl() override;

	private:
		void SetAlignmentButtons();

	private:
		// ¹öÆ°µé
		DUOLEditor::ButtonImage* _playButton;

		DUOLEditor::ButtonImage* _pauseButton;

		DUOLEditor::ButtonImage* _stopButton;

		DUOLEditor::ButtonImage* _nextButton;
	};
}