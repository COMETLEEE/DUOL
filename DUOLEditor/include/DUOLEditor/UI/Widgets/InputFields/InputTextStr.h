#pragma once
#include "DUOLCommon/Event/Event.h"
#include "DUOLEditor/UI/Widgets/DataWidgetBase.h"

namespace DUOLEditor
{
	/**
	 * \brief display a input text field.
	 */
	class InputTextStr : public DataWidgetBase<std::string>
	{
	public:
		InputTextStr(const DUOLCommon::tstring& text = TEXT(""), const DUOLCommon::tstring& label = TEXT(""));

	protected:
		virtual void Draw_Impl() override;

	public:
		std::string _imguiStr;

		DUOLCommon::tstring _text;

		DUOLCommon::tstring _label;

		bool _selectAllOnClick;

		bool _isFullWidth;

		DUOLCommon::Event<void, DUOLCommon::tstring> _textChangedEvent;

		DUOLCommon::Event<void, DUOLCommon::tstring> _enterPressedEvent;

	public:
		bool GetIsFullWidth() const;

		void SetIsFullWidth(bool value);
	};
}