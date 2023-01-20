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
		// ���� ���� ���̽��� ���ؼ� ���� ��Ͽ� ���ٸ� �׳� ���� ��Ͽ� �°� ���̽��� �ǵ�����.
		if (!_choices.contains(_currentChoices))
			_currentChoices = _choices.begin()->first;

		if (ImGui::BeginCombo(DUOLCommon::StringHelper::ToString(_tstringID).c_str(), DUOLCommon::StringHelper::ToString(_choices[_currentChoices]).c_str()))
		{
			for (const auto& [key, value] : _choices)
			{
				bool selected = (key == _currentChoices);
				
				if (ImGui::Selectable(DUOLCommon::StringHelper::ToString(value).c_str(), selected))
				{
					// ���� ���ð� �ٸ�, ���� Ű�� ���ؼ� ������ �Ǿ����ϴ�. ��, ������ ������ �ٲ�����ϴ�.
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