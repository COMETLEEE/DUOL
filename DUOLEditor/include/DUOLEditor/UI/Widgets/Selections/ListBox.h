#pragma once
#include <map>

#include "DUOLCommon/Event/Event.h"
#include "DUOLEditor/UI/Widgets/DataWidgetBase.h"

namespace DUOLEditor
{
	class ListBox : public DataWidgetBase<int>
	{
	public:
		ListBox(int currentChoice = 0);

	protected:
		virtual void Draw_Impl() override;
	public:
		std::map<int, DUOLCommon::tstring> _choices;

		int _currentChoices;

	public:
		int _nextChoice;

		void AddChoice(const DUOLCommon::tstring& choice);

		DUOLCommon::Event<void, int> _valueChangedEvent;

		DUOLCommon::Event<void, DUOLCommon::tstring> _choiceChangedEvent;
	};
}