/**

    @file      DataWidgetBase.h
    @brief     데이터 홀더의 주소를 가지고 관련된 작업을 수행하는 위젯의 기본 클래스
    @details   ~
    @author    COMETLEE
    @date      16.12.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include "DUOLEditor/UI/Widgets/WidgetBase.h"

namespace DUOLEditor
{
	/**
	 * \brief widget with data.
	 */
	template <typename TData>
	class DataWidgetBase : public WidgetBase
	{
	public:
		DataWidgetBase(TData* dataHolder) : _data(dataHolder) {}

		virtual void Draw() override;

	private:
		TData* _data;
	};

	template <typename TData>
	void DataWidgetBase<TData>::Draw()
	{
		if (_isEnable)
		{
			// 내부에서 바뀐게 있다면 바뀐 데이터를 받고 ..

			// 그리면서 업데이트 하고
			WidgetBase::Draw();

			// 업데이트 중 바뀐 내용을 홀더에게 전달해야할텐데 ..
		}
	}
}
