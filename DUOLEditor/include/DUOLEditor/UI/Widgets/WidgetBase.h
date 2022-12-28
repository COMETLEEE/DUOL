/**

    @file      WidgetBase.h
    @brief     모든 위젯들의 베이스 클래스.
    @details   ~
    @author    COMETLEE
    @date      15.12.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include "DUOLCommon/ImGui/imgui.h"
#include "DUOLEditor/UI/GUIBase.h"
#include "DUOLEditor/UI/AddOns/AddOnable.h"

namespace DUOLEditor
{
	class WidgetGroupBase;
}

namespace DUOLEditor
{
	/**
	 * \brief base class of all widgets. all GUI is constructed by widgets.
	 */
	class WidgetBase : public DUOLEditor::GUIBase, public AddOnable
	{
	public:
		WidgetBase();

		virtual void Draw();

		virtual void Draw_Impl() = 0;

		[[nodiscard]]
		DUOLEditor::WidgetGroupBase* GetParent() const;

		void SetParent(DUOLEditor::WidgetGroupBase* parent);

		bool HasParent() const;

		bool GetIsEnable() const;

		void SetIsEnable(bool value);

		void Destroy();

		bool GetIsReservedDestroy() const;

	protected:
		DUOLEditor::WidgetGroupBase* _parent;
		
		bool _isReservedDestroy;

		bool _isEnable;

		bool _isLineBreak = true;

		bool _autoExecuteAddOns = true;

#pragma region FRIEND_CLASS
		friend class WidgetGroupBase;
#pragma endregion
	};
}