/**

    @file      WidgetGroupBase.h
    @brief     위젯을 모아 놓을 수 있는 객체의 베이스 클래스
    @details   ~
    @author    COMETLEE
    @date      15.12.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include <list>

#include "WidgetBase.h"

namespace DUOLEditor
{
	/**
	 * \brief base class of any class that can group widgets.
	 */
	class WidgetGroupBase
	{
	public:
		/**
		 * \brief remove widget from the group.
		 * \param widget 지울 widget의 포인터.
		 */
		void RemoveWidget(DUOLEditor::WidgetBase* widget);

		/**
		 * \brief remove all widgets from the group.
		 */
		void RemoveAllWidgets();

		template <typename TWidget, typename ... Args>
		TWidget* AddWidget(Args&&... args) requires std::derived_from<TWidget, WidgetBase>;

		// 아니 근데 이거 같은 위젯도 여러개 들어가야하는뎅 ..
		template <typename TWidget>
		TWidget* GetWidget() requires std::derived_from<TWidget, WidgetBase>;

		const std::list<DUOLEditor::WidgetBase*>& GetWidgets();

		void ConsiderWidget(DUOLEditor::WidgetBase* widget);

		void UnConsiderWidget(DUOLEditor::WidgetBase* widget);

		/**
		 * \brief destroy reserved widgets.
		 */
		void DestroyReservedWidgets();

		/**
		 * \brief draw all widgets.
		 */
		void DrawWidgets();

	protected:
		std::list<DUOLEditor::WidgetBase*> _widgets;
	};

	template <typename TWidget, typename ... Args>
	TWidget* WidgetGroupBase::AddWidget(Args&&... args)
		requires std::derived_from<TWidget, WidgetBase>
	{
		_widgets.emplace_back(new TWidget(std::forward<Args>(args)...));

		_widgets.back()->SetParent(this);

		return static_cast<TWidget*>(_widgets.back());
	}

	template <typename TWidget>
	TWidget* WidgetGroupBase::GetWidget()
		requires std::derived_from<TWidget, WidgetBase>
	{
		for (auto& widget : _widgets)
		{
			if (dynamic_cast<TWidget*>(widget) != nullptr)
				return static_cast<TWidget*>(widget);
		}

		return nullptr;
	}
}