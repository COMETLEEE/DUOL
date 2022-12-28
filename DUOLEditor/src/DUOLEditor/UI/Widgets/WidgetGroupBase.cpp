#include "DUOLEditor/UI/Widgets/WidgetGroupBase.h"

#include <algorithm>

namespace DUOLEditor
{
	void WidgetGroupBase::RemoveWidget(DUOLEditor::WidgetBase* widget)
	{
		std::for_each(_widgets.begin(), _widgets.end(), [&widget](auto& widgetIn)
			{
				// ������� �ϴ� ���� �߰�. ������ �����մϴ�.
				if (widget == widgetIn)
					widget->Destroy();
			});
	}

	void WidgetGroupBase::RemoveAllWidgets()
	{
		// ��� ������ ���ؼ� ������ �����մϴ�.
		for (auto& widget : _widgets)
			widget->Destroy();
	}

	const std::list<DUOLEditor::WidgetBase*>& WidgetGroupBase::GetWidgets()
	{
		return _widgets;
	}

	void WidgetGroupBase::ConsiderWidget(DUOLEditor::WidgetBase* widget)
	{
		// �־��ְ�
		_widgets.push_back(widget);

		// �θ�� �������ݴϴ�
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
			// �θ� �������ְ�
			widget->SetParent(nullptr);

			_widgets.erase(iter);
		}
	}

	void WidgetGroupBase::DestroyReservedWidgets()
	{
		std::erase_if(_widgets, [](auto& widget)
			{
				// ���� ����Ǿ� �ִ� �����鿡 ���Ͽ� �����ϰ� ����Ʈ���� ����.
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
		// �̹� �������� �׸��� �� ���� ����Ǿ� �ִ� �༮�� �����մϴ�.
		DestroyReservedWidgets();

		for (const auto& widget : _widgets)
			widget->Draw();
	}
}