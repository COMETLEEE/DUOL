#include "DUOLEditor/UI/Panels/PanelWindow.h"

namespace DUOLEditor
{
	PanelWindow::PanelWindow(const DUOLCommon::tstring& panelName, bool isOpened,
		const PanelWindowSetting& panelSetting) :
		_panelName(panelName)
		, _panelWindowSetting(panelSetting)
		, _isOpened(isOpened)
	{
		// 없을 수도 있으니까 넣어준다.
		_autoSize = _panelWindowSetting.autoSize;
	}

	void PanelWindow::Open()
	{
		// 열려있지 않을 때만 합니다.
		if (!_isOpened)
		{
			_isOpened = true;

			_openEvent.Invoke();
		}
	}

	void PanelWindow::Close()
	{
		if (_isOpened)
		{
			_isOpened = false;

			_closeEvent.Invoke();
		}
	}

	void PanelWindow::Focus()
	{
		ImGui::SetWindowFocus(DUOLCommon::StringHelper::ToString(_panelName + _tstringID).c_str());
	}

	void PanelWindow::SetIsOpened(bool value)
	{
		// 프로퍼티가 다른 경우에만 시작합니다.
		if (value != _isOpened)
		{
			_isOpened = value;

			if (_isOpened)
				_openEvent.Invoke();
		}
	}

	bool PanelWindow::GetIsOpened() const
	{
		return _isOpened;
	}

	bool PanelWindow::GetIsHovered() const
	{
		return _isHovered;
	}

	bool PanelWindow::GetIsFocused() const
	{
		return _isFocused;
	}

	bool PanelWindow::GetIsAppearing() const
	{
		//
		// ImGui::FindWindowByName()
	}

	bool PanelWindow::ScrollToBottom()
	{
		_mustScrollToBottom = true;
	}

	bool PanelWindow::ScrollToTop()
	{
		_mustScrollToTop = true;
	}

	bool PanelWindow::GetIsScrolledToBottom() const
	{
		return _scrolledToBottom;
	}

	bool PanelWindow::GetIsScrolledToTop() const
	{
		return _scrolledToTop;
	}

	void PanelWindow::Draw_Impl()
	{

	}
}