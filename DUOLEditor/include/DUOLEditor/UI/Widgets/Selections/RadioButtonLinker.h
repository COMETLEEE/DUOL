#pragma once
#include "DUOLEditor/UI/Widgets/Selections/RadioButton.h"

namespace DUOLEditor
{
	/**
	 * \brief ���� ��ư�� ��ŷ�ϴ� ������ �����մϴ�. ���� ��ư ����Ʈ �߿����� ��� ���踦
	 * �̿��� ��Ҹ� ��Ʈ���� �� ����մϴ�.
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