#include "DUOLEditor/UI/Widgets/WidgetGroupBase.h"

#include <algorithm>

namespace DUOLEditor
{
	WidgetGroupBase::~WidgetGroupBase()
	{
		// ������ ���� �������� ����Ʈ 
		for (auto widget : _widgets)
			delete widget;
	}

	void WidgetGroupBase::RemoveWidget(DUOLEditor::WidgetBase* widget)
	{
		std::for_each(_widgets.begin(), _widgets.end(), [&widget](auto& widgetIn)
			{
				// ������� �ϴ� ���� �߰�. ������ �����մϴ�.
				if (widget == widgetIn)
					widget->Destroy();
			});

		std::for_each(_addReservedWidgets.begin(), _addReservedWidgets.end(), [&widget](auto& widgetIn)
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
		{
			widget->Destroy();
		}

		// ��� ������ ���ؼ� ������ �����մϴ�.
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

		auto iter2 = std::find_if(_addReservedWidgets.begin(), _addReservedWidgets.end(), [&widget](DUOLEditor::WidgetBase* widgetIn)
			{
				return widget == widgetIn;
			});

		if (iter != _widgets.end())
		{
			// �θ� �������ְ�
			widget->SetParent(nullptr);

			_widgets.erase(iter);
		}

		if (iter2 != _addReservedWidgets.end())
		{
			// �θ� �������ְ�
			widget->SetParent(nullptr);

			_addReservedWidgets.erase(iter2);
		}
	}

	void WidgetGroupBase::DestroyReservedWidgets()
	{
		std::erase_if(_widgets, [](DUOLEditor::WidgetBase* widget)
			{
				// ���� ����Ǿ� �ִ� �����鿡 ���Ͽ� �����ϰ� ����Ʈ���� ����.
				if (widget->_isReservedDestroy)
				{
					// ���� �ش� widget�� Container ��� ..?

					delete widget;

					return true;
				}
				else
					return false;
			});

		std::erase_if(_addReservedWidgets, [](DUOLEditor::WidgetBase* widget)
			{
				// ���� ����Ǿ� �ִ� �����鿡 ���Ͽ� �����ϰ� ����Ʈ���� ����.
				if (widget->_isReservedDestroy)
				{
					// ���� �ش� widget�� Container ��� ..?
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

			// ����, �ش� ������ ���� �׷� ���̽��̸� ��������� �վ��ݽô� !
			DUOLEditor::WidgetGroupBase* widgetGroupBase = dynamic_cast<DUOLEditor::WidgetGroupBase*>(widget);

			if (widgetGroupBase != nullptr)
				widgetGroupBase->AddReservedWidgets();
		}

		if (!_addReservedWidgets.empty())
			_addReservedWidgets.clear();
	}

	void WidgetGroupBase::DrawWidgets()
	{
		// �̹� �������� �׸��� �� ���� ����Ǿ� �ִ� �༮�� �����մϴ�.
		DestroyReservedWidgets();

		for (const auto& widget : _widgets)
			widget->Draw();

		// �̹� ������ �������� �߰��� �༮���� �׷��ݴϴ�.
		AddReservedWidgets();
	}
}