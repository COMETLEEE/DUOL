#include "DUOLEditor/UI/Panels/PanelWindow.h"

namespace DUOLEditor
{
	PanelWindow::PanelWindow(const DUOLCommon::tstring& panelName, bool isOpened,
		const PanelWindowSetting& panelSetting) :
		_panelName(panelName)
		, _panelWindowSetting(panelSetting)
		, _isOpened(isOpened)
	{
		// ���� ���� �����ϱ� �־��ش�.
		_autoSize = _panelWindowSetting.autoSize;
	}

	void PanelWindow::Open()
	{
		// �������� ���� ���� �մϴ�.
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
		// ������Ƽ�� �ٸ� ��쿡�� �����մϴ�.
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