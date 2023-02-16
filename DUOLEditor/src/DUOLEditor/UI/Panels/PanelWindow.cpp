#include "DUOLEditor/UI/Panels/PanelWindow.h"

#include "DUOLCommon/ImGui/imgui_internal.h"

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
		auto window = ImGui::FindWindowByName(DUOLCommon::StringHelper::ToString(_panelName + _tstringID).c_str());

		if (window != nullptr)
			return window->Appearing;
		else
			return false;
	}

	void PanelWindow::ScrollToBottom()
	{
		_mustScrollToBottom = true;
	}

	void PanelWindow::ScrollToTop()
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
		// ������ == �г��� ���� ���� ��쿡�� �����մϴ�.
		if (_isOpened)
		{
			int windowFlags = ImGuiWindowFlags_None;

			if (!_panelWindowSetting.resizable)					windowFlags |= ImGuiWindowFlags_NoResize;
			if (!_panelWindowSetting.movable)					windowFlags |= ImGuiWindowFlags_NoMove;
			if (!_panelWindowSetting.dockable)					windowFlags |= ImGuiWindowFlags_NoDocking;
			if (_panelWindowSetting.hideBackground)				windowFlags |= ImGuiWindowFlags_NoBackground;
			if (_panelWindowSetting.forceHorizontalScrollbar)	windowFlags |= ImGuiWindowFlags_AlwaysHorizontalScrollbar;
			if (_panelWindowSetting.forceVerticalScrollbar)		windowFlags |= ImGuiWindowFlags_AlwaysVerticalScrollbar;
			if (_panelWindowSetting.allowHorizontalScrollbar)	windowFlags |= ImGuiWindowFlags_HorizontalScrollbar;
			if (!_panelWindowSetting.bringToFrontOnFocus)		windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
			if (!_panelWindowSetting.collapsable)				windowFlags |= ImGuiWindowFlags_NoCollapse;
			if (!_panelWindowSetting.allowInputs)				windowFlags |= ImGuiWindowFlags_NoInputs;
			if (!_panelWindowSetting.scrollable)                windowFlags |= ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar;
			if (!_panelWindowSetting.titleBar)					windowFlags |= ImGuiWindowFlags_NoTitleBar;

			ImVec2 minSizeConstraint = ImGuiHelper::ToImVec2(_minSize);
			ImVec2 maxSizeConstraint = ImGuiHelper::ToImVec2(_maxSize);

			// ������ ������ Ŭ����
			if (minSizeConstraint.x <= 0.f || minSizeConstraint.y <= 0.f)
				minSizeConstraint = { 0.f, 0.f };

			// ������ ������ Ŭ����
			if (maxSizeConstraint.x <= 0.f || maxSizeConstraint.y <= 0.f)
				maxSizeConstraint = { 10000.f ,10000.f};

			// �ּ�, �ִ� ������ ���� Setting
			ImGui::SetNextWindowSizeConstraints(minSizeConstraint, maxSizeConstraint);

			// ������ �����Դϴ�.
			if (ImGui::Begin(DUOLCommon::StringHelper::ToString(_panelName + _tstringID).c_str(), _panelWindowSetting.closable ? &_isOpened : nullptr, windowFlags))
			{
				_isHovered = ImGui::IsWindowHovered();

				_isFocused = ImGui::IsWindowFocused();

				float scrollY = ImGui::GetScrollY();

				_scrolledToBottom = scrollY == ImGui::GetScrollMaxY();

				_scrolledToTop = scrollY == 0.f;

				// �� �������� �����ٸ� ..
				if (!_isOpened)
					_closeEvent.Invoke();

				Update();

				if (_mustScrollToBottom)
				{
					ImGui::SetScrollY(ImGui::GetScrollMaxY());
					_mustScrollToBottom = false;
				}

				if (_mustScrollToTop)
				{
					ImGui::SetScrollY(0.0f);
					_mustScrollToTop = false;
				}

				DrawWidgets();

				_panelWindowCallbacks.Invoke();
			}

			ImGui::End();
		}
	}
}