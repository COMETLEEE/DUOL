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

		// ù ��°�� ������ ������ �ϴ� �����ۿ� ���ؼ� �����ݴϴ�.
		if (_isFirstOpen)
		{
			ImGui::SetNextItemOpen(true);

			_isFirstOpen = false;
		}

		if (ImGui::CollapsingHeader(DUOLCommon::StringHelper::ToString(_name + _tstringID).c_str(), _closable ? &_isOpened : nullptr))
			Container::Draw_Impl();

		// ���� ��Ȳ�� �ٲ���ٸ� �̺�Ʈ �Լ��� ȣ���մϴ�.
		if (_isOpened != prevIsOpened)
		{
			if (_isOpened)
				_openEvent.Invoke();
			else
				_closeEvent.Invoke();
		}
	}
}