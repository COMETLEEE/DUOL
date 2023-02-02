#include "DUOLEditor/UI/Widgets/Layout/ContainerCollapsable.h"

namespace DUOLEditor
{
	ContainerCollapsable::ContainerCollapsable(const DUOLCommon::tstring& name) :
		_name(name)
		, _closable(false)
		, _isOpened(true)
	{
	}

	void ContainerCollapsable::Draw_Impl()
	{
		bool prevIsOpened = _isOpened;

		if (ImGui::CollapsingHeader(DUOLCommon::StringHelper::ToString(_name + _tstringID).c_str(), _closable ? &_isOpened : nullptr))
			Container::Draw_Impl();

		// 오픈 상황이 바뀌었다면 이벤트 함수를 호출합니다.
		if (_isOpened != prevIsOpened)
		{
			if (_isOpened)
				_openEvent.Invoke();
			else
				_closeEvent.Invoke();
		}
	}
}