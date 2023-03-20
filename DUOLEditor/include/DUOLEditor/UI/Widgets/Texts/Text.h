#pragma once
#include "DUOLEditor/UI/Widgets/DataWidgetBase.h"

namespace DUOLEditor
{
	/**
	 * \brief widget to display text.
	 */
	class Text : public DataWidgetBase<DUOLCommon::tstring>
	{
	public:
		Text(const DUOLCommon::tstring& text = TEXT(""), std::function<void()> callbackAfter = nullptr);

		std::function<void()> _callbackAfter;

	protected:
		virtual void Draw_Impl() override;

	protected:
		DUOLCommon::tstring _text;
	};
}