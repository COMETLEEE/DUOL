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

	const std::list<std::shared_ptr<DUOLEditor::WidgetBase>>& WidgetGroupBase::GetWidgets()
	{
		return _widgets;
	}

	void WidgetGroupBase::DestroyReservedWidgets()
	{
		std::erase_if(_widgets, [](auto& widget)
			{
				// ���� ����Ǿ� �ִ� �����鿡 ���Ͽ� �����ϰ� ����Ʈ���� ����.
				if (widget->_isReservedDestroy)
				{
					widget.reset();

					return true;
				}
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