#include "DUOLEditor/Modules/ContentsBrowser.h"

namespace DUOLEditor
{
	ContentsBrowser::ContentsBrowser(const DUOLCommon::tstring panelName, bool isOpened,
		PanelWindowSetting panelSettings) :
		PanelWindow(panelName, isOpened, panelSettings)
	{
	}

	ContentsBrowser::~ContentsBrowser()
	{
	}
}
