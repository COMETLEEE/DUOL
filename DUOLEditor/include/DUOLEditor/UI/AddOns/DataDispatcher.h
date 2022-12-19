/**

    @file      DataDispatcher.h
    @brief     data
    @details   ~
    @author    COMETLEE
    @date      16.12.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include <functional>
#include "DUOLEditor/UI/AddOns/AddOnBase.h"

namespace DUOLEditor
{
	/**
	 * \brief auto Data dispatching of any data widget.
	 * \tparam TData 
	 */
	template <typename TData>
	class DataDispatcher : public AddOnBase
	{



		/**
		 * \brief Data dispatcher는 DataWidget에 귀속되어 사용됩니다.
		 */
		virtual void Execute() override { }

	private:
		bool _isValueChanged = false;

		TData* _data;

		std::function<void(TData)> _provide;

		std::function<TData(void)> _gather;
	};
}