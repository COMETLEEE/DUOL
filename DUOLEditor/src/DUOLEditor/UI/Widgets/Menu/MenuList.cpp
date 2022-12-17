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
			// ������ �ʾҴµ� ����. ������ ����.
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