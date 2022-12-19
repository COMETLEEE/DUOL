/**

    @file      DragAndDropDest.h
    @brief     드래그 앤 드랍의 목적지가 될 수 있는 애드인.
    @details   ~
    @author    COMETLEE
    @date      16.12.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include "DUOLCommon/Event/Event.h"
#include "DUOLEditor/UI/AddOns/AddOnBase.h"

namespace DUOLEditor
{
	template <typename TData>
	class DragAndDropDest : public AddOnBase
	{
	public:
		DragAndDropDest(const DUOLCommon::tstring& identifier) : _identifier(identifier)
		{}

		virtual void Execute() override
		{
			if (ImGui::BeginDragDropTarget())
			{
				if (!_isHovered)
					_hoverStartEvent.Invoke();

				_isHovered = true;

				ImGuiDragDropFlags targetFlags = 0;

				if (!_showRect)
					targetFlags |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect;

				if (const ImGuiPayload* payload = 
					ImGui::AcceptDragDropPayload(DUOLCommon::StringHelper::ToString(_identifier).c_str(), targetFlags))
				{
					TData data = *(TData*)payload->Data;

					_dataReceivedEvent.Invoke(data);
				}

				ImGui::EndDragDropTarget();
			}
			else
			{
				if (_isHovered)
					_hoverEndEvent.Invoke();

				_isHovered = false;
			}
		}

		inline bool GetIsHoverd() const { return _isHovered; }

	public:
		DUOLCommon::tstring _identifier;

		DUOLCommon::Event<TData> _dataReceivedEvent;

		DUOLCommon::Event<void> _hoverStartEvent;

		DUOLCommon::Event<void> _hoverEndEvent;

		bool _showRect = true;

	private:
		bool _isHovered;
	};
}