#include "DUOLEditor/Modules/ViewBase.h"

namespace DUOLEditor
{
	ViewBase::ViewBase(const DUOLCommon::tstring& title, bool isOpened, const PanelWindowSetting& windowSettings) :
		PanelWindow(title, isOpened, windowSettings)
	{
		// _image = this->AddWidget<DUOLEditor::Image>(nullptr, )
	}

	void ViewBase::Draw_Impl()
	{
		PanelWindow::Draw_Impl();
	}

	void ViewBase::Render()
	{
	}
}