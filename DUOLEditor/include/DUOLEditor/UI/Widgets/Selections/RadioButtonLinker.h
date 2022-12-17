#pragma once
#include "DUOLEditor/UI/Widgets/Selections/RadioButton.h"

namespace DUOLEditor
{
	/**
	 * \brief 라디오 버튼을 링킹하는 역할을 수행합니다. 라디오 버튼 리스트 중에서의 상관 관계를
	 * 이용해 요소를 컨트롤할 때 사용합니다.
	 */
	class RadioButtonLinker : public DataWidgetBase<int>
	{
	public:
		RadioButtonLinker();

		void Link(RadioButton* radioButton);

		void UnLink(RadioButton* radioButton);

		int GetSelectedRadioButton() const;

	protected:
		virtual void Draw_Impl() override;

	private:
		void OnRadioButtonClicked(int radioID);

	public:
		DUOLCommon::Event<void, int> _selectChangedEvent;

	private:
		int _availableRadioID;

		int _selectedRadioID;

		std::list<std::pair<DUOLCommon::EventHandlerID, RadioButton*>> _radioButtons;
	};
}