#include "DUOLEditor/UI/Widgets/Menu/MenuList.h"

namespace DUOLEditor
{
	MenuList::MenuList(const DUOLCommon::tstring& name, bool isEnable) :
		_name(name)
		, _isEnable(isEnable)
	{
	}

	void MenuList::Draw_Impl()
	{
		if (ImGui::BeginMenu(DUOLCommon::StringHelper::ToString(_name).c_str(), !_isEnable))
		{
			// 열리지 않았는데 열림. 눌려서 열림.
			if (!_isOpened)
			{
				_clickedEvent.Invoke();

				_isOpened = true;
			}

			DrawWidgets();

			ImGui::EndMenu();
		}
		else
		{
			_isOpened = false;
		}
	}
}