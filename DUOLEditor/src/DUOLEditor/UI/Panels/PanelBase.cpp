#include "DUOLEditor/UI/Panels/PanelBase.h"

namespace DUOLEditor
{
	PanelBase::PanelBase() :
		WidgetGroupBase()
		, _isEnable(true)
	{

	}

	PanelBase::~PanelBase()
	{

	}

	bool PanelBase::GetIsEnable() const
	{
		return _isEnable;
	}

	void PanelBase::SetIsEnable(bool value)
	{
		_isEnable = value;
	}

	void PanelBase::Draw()
	{
		if (_isEnable)
		{
			Draw_Impl();
		}
	}
}