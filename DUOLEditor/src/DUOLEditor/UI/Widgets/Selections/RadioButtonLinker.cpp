#include "DUOLEditor/UI/Widgets/Selections//RadioButtonLinker.h"

namespace DUOLEditor
{
	RadioButtonLinker::RadioButtonLinker() :
		DataWidgetBase<int>(&_selectedRadioID)
		, _availableRadioID(0)
		, _selectedRadioID(-1)
	{
	}

	void RadioButtonLinker::Link(RadioButton* radioButton)
	{
		radioButton->_radioID = _availableRadioID++;
		auto handlerID = (radioButton->_clickedEvent += std::bind(&RadioButtonLinker::OnRadioButtonClicked, this, std::placeholders::_1));

		// ���� ��Ŀ ���� ���õ� ���� ��ư�� ���ٸ�
		if (radioButton->_isSelected && _selectedRadioID == -1)
		{
			_selectedRadioID = radioButton->_radioID;
		}

		_radioButtons.push_back({ handlerID, radioButton });
	}

	void RadioButtonLinker::UnLink(RadioButton* radioButton)
	{
		auto iter = std::ranges::find_if(_radioButtons.begin(), _radioButtons.end(), [&radioButton](std::pair<DUOLCommon::EventListenerID, RadioButton*>& pair)
			{
				return pair.second == radioButton;
			});

		if (iter != _radioButtons.end())
		{
			// ���� ��ư���� �̺�Ʈ �ڵ鷯 �����.
			iter->second->_clickedEvent.RemoveListener(iter->first);

			_radioButtons.erase(iter);
		}
	}

	int RadioButtonLinker::GetSelectedRadioButton() const
	{
		return _selectedRadioID;
	}

	void RadioButtonLinker::Draw_Impl()
	{
		// ���� ��ư ��Ŀ�� ���� �׸� ���� �����ϴ�.
		// �׳� ��ŷ�� ���� �̺�Ʈ ������ �׷��� �����ϴ� ���Ҹ� �����մϴ�.
	}

	void RadioButtonLinker::OnRadioButtonClicked(int radioID)
	{
		// ������ �ٲ� ��쿡�� �ǽ��մϴ�.
		if (radioID != _selectedRadioID)
		{
			_selectedRadioID = radioID;

			_selectChangedEvent.Invoke(_selectedRadioID);
			
			Notify();

			for (const auto& [listener, radioButton] : _radioButtons)
			{
				// ���õ� �༮�� �������� �ٲ��ݴϴ�.
				radioButton->_isSelected = (radioButton->_radioID == _selectedRadioID);
			}
		}
	}
}