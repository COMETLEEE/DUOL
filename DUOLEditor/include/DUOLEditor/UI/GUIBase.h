/**

    @file      GUIBase.h
    @brief     GUI component's interface class.
    @details   ~
    @author    COMETLEE
    @date      15.12.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include <memory>

#include "DUOLCommon/StringHelper.h"
#include "DUOLEditor/Util/ImGuiHelper.h"

namespace DUOLEditor
{
	/**
	 * \brief GUI component's interface class.
	 */
	class GUIBase
	{
	public:
		virtual void Draw() = 0;

	protected:
		GUIBase();

		virtual ~GUIBase() = default;

	protected:
		uint64_t _id;

		DUOLCommon::tstring _tstringID;

		static uint64_t _nextID;

	public:
		[[nodiscard]]
		const uint64_t& GetID() const;

		[[nodiscard]]
		const DUOLCommon::tstring& GetTStringID() const;
	};
}
