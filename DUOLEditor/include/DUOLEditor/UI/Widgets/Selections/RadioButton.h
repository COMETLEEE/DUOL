#pragma once
#include "DUOLCommon/Event/Event.h"
#include "DUOLEditor/UI/Widgets/DataWidgetBase.h"

namespace DUOLEditor
{
	class RadioButtonLinker;
}

namespace DUOLEditor
{
	/**
	 * \brief Widget that represent a button that is selectable. Only one radio button can be selected
	* in the same RadioButtonLinker
	 */
	class RadioButton : public DataWidgetBase<bool>
	{
		friend class RadioButtonLinker;

	public:
		RadioButton(bool selected = false, const DUOLCommon::tstring& label = TEXT(""));

	protected:
		virtual void Draw_Impl() override;

	public:
		DUOLCommon::tstring _label;

		DUOLCommon::Event<void, int> _clickedEvent;

	private:
		bool _isSelected;

		int _radioID;
	};
}