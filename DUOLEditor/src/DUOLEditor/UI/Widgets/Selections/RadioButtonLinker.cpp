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

		// 아직 링커 내에 선택된 라디오 버튼이 없다면
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
			// 라디오 버튼에서 이벤트 핸들러 지우고.
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
		// 라디오 버튼 링커는 딱히 그릴 일은 없습니다.
		// 그냥 링킹에 대한 이벤트 관리와 그룹을 관리하는 역할만 수행합니다.
	}

	void RadioButtonLinker::OnRadioButtonClicked(int radioID)
	{
		// 선택이 바뀐 경우에만 실시합니다.
		if (radioID != _selectedRadioID)
		{
			_selectedRadioID = radioID;

			_selectChangedEvent.Invoke(_selectedRadioID);
			
			Notify();

			for (const auto& [listener, radioButton] : _radioButtons)
			{
				// 선택된 녀석을 선택으로 바꿔줍니다.
				radioButton->_isSelected = (radioButton->_radioID == _selectedRadioID);
			}
		}
	}
}