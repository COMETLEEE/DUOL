/**

    @file      DragAndDropSource.h
    @brief     드래그 앤 드랍을 시작할 수 있는 애드인
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
	class DragAndDropSource : public AddOnBase
	{
	public:
        DragAndDropSource(const DUOLCommon::tstring& identifier, const DUOLCommon::tstring& tooltip, TData data) :
			_identifier(identifier)
			, _tooltip(tooltip)
			, _data(data)
			, _hasTooltip(true)
        {}

        virtual void Execute() override
        {
            ImGuiDragDropFlags flags = 0;
            flags |= ImGuiDragDropFlags_SourceNoDisableHover;       // keep the source displayed as hovered;
            flags |= ImGuiDragDropFlags_SourceNoHoldToOpenOthers;   // disable the feature of opening foreign treenodes / tabs while dragging.

            if (!_hasTooltip)
                flags |= ImGuiDragDropFlags_AcceptNoPreviewTooltip; // hide the tooltip.

            if (ImGui::BeginDragDropSource(flags))
            {
                if (_isDragged)
                    _dragStartEvent.Invoke();

                _isDragged = true;

                // 툴팁이 있습니다.
                if (!(flags & ImGuiDragDropFlags_SourceNoPreviewTooltip))
                {
                    ImGui::Text(DUOLCommon::StringHelper::ToString(_tooltip).c_str());
                }

                ImGui::SetDragDropPayload(DUOLCommon::StringHelper::ToString(_identifier).c_str(), &_data, sizeof(_data));
                ImGui::EndDragDropSource();
            }
            else
            {
                if (_isDragged)
                    _dragStopEvent.Invoke();

                _isDragged = false;
            }
        }

	public:
        DUOLCommon::tstring _identifier;

        DUOLCommon::tstring _tooltip;

        TData _data;

        DUOLCommon::Event<void> _dragStartEvent;

        DUOLCommon::Event<void> _dragStopEvent;

        bool _hasTooltip;

	private:
        bool _isDragged;
	};
}