#pragma once
#include "DUOLCommon/Event/Event.h"
#include "DUOLEditor/UI/Panels/PanelTransformableBase.h"

namespace DUOLEditor
{
	/**
	 * \brief display panel that is localized and behave like a window (Movable, resizable...)
	 */
	class PanelWindow : public PanelTransformableBase
	{
	public:
		PanelWindow(const DUOLCommon::tstring& panelName = TEXT(""), bool isOpened = true,
			const PanelWindowSetting& panelSetting = PanelWindowSetting());

		void Open();

		void Close();

		void Focus();

		void SetIsOpened(bool value);

		bool GetIsOpened() const;

		bool GetIsHovered() const;

		bool GetIsFocused() const;

		bool GetIsAppearing() const;

		bool ScrollToBottom();

		bool ScrollToTop();

		bool GetIsScrolledToBottom() const;

		bool GetIsScrolledToTop() const;

	protected:
		virtual void Draw_Impl() override;

	public:
		DUOLCommon::tstring _panelName;

		DUOLMath::Vector2 _minSize = { 0.f, 0.f };

		DUOLMath::Vector2 _maxSize = { 0.f, 0.f };

		PanelWindowSetting _panelWindowSetting;

		DUOLCommon::Event<void> _openEvent;

		DUOLCommon::Event<void> _closeEvent;

	private:
		bool _isOpened;

		bool _isHovered;

		bool _isFocused;

		bool _mustScrollToBottom = false;

		bool _mustScrollToTop = false;

		bool _scrolledToBottom = false;

		bool _scrolledToTop = false;
	};
}