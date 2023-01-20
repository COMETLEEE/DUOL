#include "DUOLEditor/UI/Widgets/Selections/ComboBox.h"

namespace DUOLEditor
{
	ComboBox::ComboBox(int currentChoice) :
		DataWidgetBase<int>(&_currentChoices)
		, _currentChoices(currentChoice)
	{
	}

	void ComboBox::Draw_Impl()
	{
		// 만약 현재 초이스에 대해서 선택 목록에 없다면 그냥 선택 목록에 맞게 초이스를 되돌린다.
		if (!_choices.contains(_currentChoices))
			_currentChoices = _choices.begin()->first;

		if (ImGui::BeginCombo(DUOLCommon::StringHelper::ToString(_tstringID).c_str(), DUOLCommon::StringHelper::ToString(_choices[_currentChoices]).c_str()))
		{
			for (const auto& [key, value] : _choices)
			{
				bool selected = (key == _currentChoices);
				
				if (ImGui::Selectable(DUOLCommon::StringHelper::ToString(value).c_str(), selected))
				{
					// 현재 선택과 다른, 지금 키에 대해서 선택이 되었습니다. 즉, 유저의 선택이 바뀌었습니다.
					if (!selected)
					{
						ImGui::SetItemDefaultFocus();
						_currentChoices = key;
						_valueChangedEvent.Invoke(_currentChoices);
						Notify();
					}
				}
			}

			ImGui::EndCombo();
		}
	}
}