#pragma once
#include "DUOLCommon/StringHelper.h"



namespace DUOLEditor
{
	/**
	 * \brief User message box.
	 */
	class MessageBox_
	{
	public:
		enum class MessageType
		{
			QUESTION = 0x00000020L,
			INFORMATION = 0x00000040L,
			WARNING = 0x00000030L,
			ERROR_ = 0x00000010L
		};

		enum class ButtonLayout
		{
			OK = 0x00000000L,
			OK_CANCEL = 0x00000001L,
			YES_NO = 0x00000004L,
			YES_NO_CANCEL = 0x00000003L,
			RETRY_CANCEL = 0x00000005L,
			ABORT_RETRY_IGNORE = 0x00000002L,
			CANCEL_TRYAGAIN_CONTINUE = 0x00000006L,
			HELP = 0x00004000L
		};

		enum class UserChoice
		{
			OK = 1,
			CANCEL = 2,
			YES = 6,
			NO = 7,
			CONTINUE = 11,
			IGNORE_ = 5,
			RETRY = 4,
			TRYAGAIN = 10
		};

		MessageBox_(const DUOLCommon::tstring& name, const DUOLCommon::tstring& message, MessageType messageType = MessageType::INFORMATION, ButtonLayout buttonLayout = ButtonLayout::OK, bool autoShowMessageBox = true);

		void ShowMessageBox();

		const UserChoice& GetUserChoice() const;

	private:
		DUOLCommon::tstring _name;

		DUOLCommon::tstring _message;

		MessageType _messageType;

		ButtonLayout _buttonLayout;

		UserChoice _userChoice;
	};
}