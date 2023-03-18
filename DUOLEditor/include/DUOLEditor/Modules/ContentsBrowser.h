#pragma once
#include "DUOLEditor/UI/Panels/PanelWindow.h"

namespace DUOLEditor
{
	/**
	 * \brief Content's Browsing object.
	 */
	class ContentsBrowser : public DUOLEditor::PanelWindow
	{
	public:
		ContentsBrowser(const DUOLCommon::tstring panelName, bool isOpened, PanelWindowSetting panelSettings);

		virtual ~ContentsBrowser() override;

	private:
		
	};
}
