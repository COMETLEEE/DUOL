#include "DUOLEditor/UI/Widgets/WidgetGroupBase.h"

#include <algorithm>

namespace DUOLEditor
{
	void WidgetGroupBase::RemoveWidget(DUOLEditor::WidgetBase* widget)
	{
		std::for_each(_widgets.begin(), _widgets.end(), [&widget](auto& widgetIn)
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
			widget->Destroy();
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

		if (iter != _widgets.end())
		{
			// 부모 제외해주고
			widget->SetParent(nullptr);

			_widgets.erase(iter);
		}
	}

	void WidgetGroupBase::DestroyReservedWidgets()
	{
		std::erase_if(_widgets, [](auto& widget)
			{
				// 삭제 예약되어 있는 위젯들에 대하여 리셋하고 리스트에서 삭제.
				if (widget->_isReservedDestroy)
				{
					delete widget;

					return true;
				}
				else
					return false;
			});
	}

	void WidgetGroupBase::DrawWidgets()
	{
		// 이번 프레임을 그리기 전 삭제 예약되어 있는 녀석을 삭제합니다.
		DestroyReservedWidgets();

		for (const auto& widget : _widgets)
			widget->Draw();
	}
}