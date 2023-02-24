#include "DUOLEditor/UI/Widgets/Layout/ContainerCollapsable.h"

namespace DUOLEditor
{
	ContainerCollapsable::ContainerCollapsable(const DUOLCommon::tstring& name, bool isFirstOpen) :
		_name(name)
		, _closable(false)
		, _isOpened(true)
		, _isFirstOpen(isFirstOpen)
	{

	}

	void ContainerCollapsable::Draw_Impl()
	{
		bool prevIsOpened = _isOpened;

		// 첫 번째로 무조건 열려야 하는 아이템에 대해서 열어줍니다.
		if (_isFirstOpen)
		{
			ImGui::SetNextItemOpen(true);

			_isFirstOpen = false;
		}

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