#include "DUOLEditor/UI/Widgets/Selections/ListBox.h"

namespace DUOLEditor
{
	ListBox::ListBox(int currentChoice) :
		DataWidgetBase<int>(&_currentChoices)
		, _currentChoices(currentChoice)
		, _nextChoice(0)
	{
	}
	
	void ListBox::Draw_Impl()
	{
		// ���� ���� ���̽��� ���ؼ� ���� ��Ͽ� ���ٸ� �׳� ���� ��Ͽ� �°� ���̽��� �ǵ�����.
		if (!_viewChoices.contains(_currentChoices))
			_currentChoices = _viewChoices.begin()->first;

		if (ImGui::BeginListBox(DUOLCommon::StringHelper::ToString(_tstringID).c_str(), ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
		{
			for (const auto& [key, value] : _viewChoices)
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
						_choiceChangedEvent.Invoke(_viewChoices.at(_currentChoices));
						Notify();
					}
				}
			}

			ImGui::EndListBox();
		}
	}

	void ListBox::AddChoice(const DUOLCommon::tstring& choice)
	{
		_viewChoices.insert({ _nextChoice, choice });

		_choices.insert({ _nextChoice++, choice });
	}
}