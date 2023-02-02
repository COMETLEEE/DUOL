#include "DUOLEditor/UI/Widgets/WidgetGroupBase.h"

#include <algorithm>

namespace DUOLEditor
{
	WidgetGroupBase::~WidgetGroupBase()
	{
		// 나머지 새끼 영역을도 딜리트 
		for (auto widget : _widgets)
			delete widget;
	}

	void WidgetGroupBase::RemoveWidget(DUOLEditor::WidgetBase* widget)
	{
		std::for_each(_widgets.begin(), _widgets.end(), [&widget](auto& widgetIn)
			{
				// 지우고자 하는 위젯 발견. 삭제를 예약합니다.
				if (widget == widgetIn)
					widget->Destroy();
			});

		std::for_each(_addReservedWidgets.begin(), _addReservedWidgets.end(), [&widget](auto& widgetIn)
			{
				// 지우고자 하는 위젯 발견. 삭제를 예약합니다.
				if (widget == widgetIn)
					widget->Destroy();
			});
	}

	void WidgetGroupBase::RemoveAllWidgets()
	{
		// 모든 위젯에 대해서 삭제를 예약합니다.
		for (auto& widget : _widgets)
		{
			widget->Destroy();
		}

		// 모든 위젯에 대해서 삭제를 예약합니다.
		for (auto& widget : _addReservedWidgets)
		{
			widget->Destroy();
		}
	}

	const std::list<DUOLEditor::WidgetBase*>& WidgetGroupBase::GetWidgets()
	{
		return _widgets;
	}

	void WidgetGroupBase::ConsiderWidget(DUOLEditor::WidgetBase* widget)
	{
		// 넣어주고
		_widgets.push_back(widget);

		// 부모로 설정해줍니다
		widget->SetParent(this);
	}

	void WidgetGroupBase::UnConsiderWidget(DUOLEditor::WidgetBase* widget)
	{
		auto iter = std::find_if(_widgets.begin(), _widgets.end(), [&widget](DUOLEditor::WidgetBase* widgetIn)
			{
				return widget == widgetIn;
			});

		auto iter2 = std::find_if(_addReservedWidgets.begin(), _addReservedWidgets.end(), [&widget](DUOLEditor::WidgetBase* widgetIn)
			{
				return widget == widgetIn;
			});

		if (iter != _widgets.end())
		{
			// 부모 제외해주고
			widget->SetParent(nullptr);

			_widgets.erase(iter);
		}

		if (iter2 != _addReservedWidgets.end())
		{
			// 부모 제외해주고
			widget->SetParent(nullptr);

			_addReservedWidgets.erase(iter2);
		}
	}

	void WidgetGroupBase::DestroyReservedWidgets()
	{
		std::erase_if(_widgets, [](DUOLEditor::WidgetBase* widget)
			{
				// 삭제 예약되어 있는 위젯들에 대하여 리셋하고 리스트에서 삭제.
				if (widget->_isReservedDestroy)
				{
					// 만약 해당 widget에 Container 라면 ..?

					delete widget;

					return true;
				}
				else
					return false;
			});

		std::erase_if(_addReservedWidgets, [](DUOLEditor::WidgetBase* widget)
			{
				// 삭제 예약되어 있는 위젯들에 대하여 리셋하고 리스트에서 삭제.
				if (widget->_isReservedDestroy)
				{
					// 만약 해당 widget에 Container 라면 ..?
					delete widget;

					return true;
				}
				else
					return false;
			});
	}

	void WidgetGroupBase::AddReservedWidgets()
	{
		for (auto widget: _addReservedWidgets)
		{
			_widgets.push_back(widget);

			// 만약, 해당 위젯이 위젯 그룹 베이스이면 재귀적으로 뚫어줍시다 !
			DUOLEditor::WidgetGroupBase* widgetGroupBase = dynamic_cast<DUOLEditor::WidgetGroupBase*>(widget);

			if (widgetGroupBase != nullptr)
				widgetGroupBase->AddReservedWidgets();
		}

		if (!_addReservedWidgets.empty())
			_addReservedWidgets.clear();
	}

	void WidgetGroupBase::DrawWidgets()
	{
		// 이번 프레임을 그리기 전 삭제 예약되어 있는 녀석을 삭제합니다.
		DestroyReservedWidgets();

		for (const auto& widget : _widgets)
			widget->Draw();

		// 이번 프레임 마지막에 추가된 녀석들을 그려줍니다.
		AddReservedWidgets();
	}
}