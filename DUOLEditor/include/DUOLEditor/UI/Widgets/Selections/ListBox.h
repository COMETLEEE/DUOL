#pragma once
#include <map>

#include "DUOLCommon/Event/Event.h"
#include "DUOLEditor/UI/Widgets/DataWidgetBase.h"

namespace DUOLEditor
{
	/**
	 * \brief 검색이 가능한 List Box 객체입니다.
	 */
	class ListBox : public DataWidgetBase<int>
	{
	public:
		ListBox(int currentChoice = 0);

	protected:
		virtual void Draw_Impl() override;

	public:
		/**
		 * \brief 현재 ListBox에 등록된 모든 선택 가능 컨텐츠들
		 */
		std::map<int, DUOLCommon::tstring> _choices;

		/**
		 * \brief 현재 ListBox에 보여지는 모든 선택 가능 컨텐츠들
		 */
		std::map<int, DUOLCommon::tstring> _viewChoices;

		int _currentChoices;

	public:
		int _nextChoice;

		void AddChoice(const DUOLCommon::tstring& choice);

		DUOLCommon::Event<void, int> _valueChangedEvent;

		DUOLCommon::Event<void, DUOLCommon::tstring> _choiceChangedEvent;
	};
}