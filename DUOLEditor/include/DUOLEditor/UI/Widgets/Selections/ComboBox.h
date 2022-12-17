#pragma once
#include <map>

#include "DUOLCommon/Event/Event.h"
#include "DUOLEditor/UI/Widgets/DataWidgetBase.h"

namespace DUOLEditor
{
	/**
	 * \brief Widget that display a list of values that the user can select.
	 */
	class ComboBox : public DataWidgetBase<int>
	{
	public:
		ComboBox(int currentChoice = 0);

	protected:
		virtual void Draw_Impl() override;

	public:
		std::map<int, DUOLCommon::tstring> _choices;

		int _currentChoices;

	public:
		DUOLCommon::Event<void, int> _valueChangedEvent;
	};
}
