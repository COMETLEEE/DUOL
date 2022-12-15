#include "DUOLEditor/UI/Widgets/WidgetBase.h"

namespace DUOLEditor
{
	WidgetBase::WidgetBase() :
		_parent(nullptr)
		, _isReservedDestroy(false)
		, _isEnable(true)
	{

	}

	void WidgetBase::Draw()
	{
		// 사용 가능하면 !
		if (_isEnable)
		{
			Draw_Impl();
		}
	}

	DUOLEditor::WidgetGroupBase* WidgetBase::GetParent() const
	{
		return _parent;
	}

	void WidgetBase::SetParent(DUOLEditor::WidgetGroupBase* parent)
	{
		_parent = parent;
	}

	bool WidgetBase::GetIsEnable() const
	{
		return _isEnable;
	}

	void WidgetBase::SetIsEnable(bool value)
	{
		_isEnable = value;
	}

	void WidgetBase::Destroy()
	{
		_isReservedDestroy = true;
	}

	bool WidgetBase::GetIsReservedDestroy() const
	{
		return _isReservedDestroy;
	}
}