#include "DUOLEditor/Util/MessageBox/MessageBox.h"

#include <windows.h>

namespace DUOLEditor
{
	MessageBox_::MessageBox_(const DUOLCommon::tstring& name, const DUOLCommon::tstring& message, MessageType messageType,
		ButtonLayout buttonLayout, bool autoShowMessageBox) :
		_name(name)
		, _message(message)
		, _messageType(messageType)
		, _buttonLayout(buttonLayout)
	{
		if (autoShowMessageBox)
			ShowMessageBox();
	}

	void MessageBox_::ShowMessageBox()
	{
		int button = MessageBox
		(
			nullptr,
			static_cast<LPCWSTR>(_message.c_str()),
			static_cast<LPCWSTR>(_name.c_str()),
			static_cast<UINT>(_messageType) | static_cast<UINT>(_buttonLayout) | MB_DEFBUTTON2
		);

		_userChoice = static_cast<UserChoice>(button);
	}

	const MessageBox_::UserChoice& MessageBox_::GetUserChoice() const
	{
		return _userChoice;
	}
}