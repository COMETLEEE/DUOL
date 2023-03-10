#pragma once
#include "DUOLEditor/UI/Panels/PanelMenuBar.h"

namespace DUOLEditor
{
	class MenuItem;
	class PanelWindow;
}

namespace DUOLEditor
{
	class PanelWindow;

	class MenuBar : public DUOLEditor::PanelMenuBar
	{
	public:
		MenuBar();

		void RegisterPanel(const DUOLCommon::tstring& name, PanelWindow* panel);

	private:
		void CreateSceneMenu();

	private:
		std::unordered_map<DUOLCommon::tstring, std::pair<PanelWindow*, MenuItem*>> _panels;
	};
}
