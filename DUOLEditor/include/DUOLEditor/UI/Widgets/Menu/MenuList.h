#pragma once
#include "DUOLCommon/Event/Event.h"
#include "DUOLEditor/UI/Widgets/Layout/Container.h"

namespace DUOLEditor
{
	/**
	 * \brief Widget that group with a menu display.
	 */
	class MenuList : public Container
	{
	public:
		MenuList(const DUOLCommon::tstring& name = TEXT(""), bool isEnable = false);

	protected:
		virtual void Draw_Impl() override;

	public:
		DUOLCommon::tstring _name;

		bool _isEnable;

		DUOLCommon::Event<void> _clickedEvent;

	private:
		bool _isOpened;
	};
}