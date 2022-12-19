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
		DataWidgetBase(TData* dataHolder) : _data(dataHolder), _dataExternal(nullptr) {}

		virtual void Draw() override;

		/**
		 * \brief 데이터의 변경이 확인되면 데이터 디스패칭 준비.
		 */
		void Notify();

		/**
		 * \brief 외부 데이터 참조를 등록합니다.
		 * \param dataPointer 위젯 외부의 데이터를 참조하기 위한 주소. 위젯의 표시를 업데이트 및 조작된 사항을 외부로 전달합니다.
		 */
		void RegisterExternalDataPointer(TData* dataPointer) { _dataExternal = dataPointer; }

		/**
		 * \brief 외부로 데이터를 던질 때 사용하는 함수를 등록합니다.
		 * \param provider 외부로 데이터를 참조시킬 때 사용하는 함수 객체
		 */
		void RegisterProvider(std::function<void(TData*)> provider) { _dataProvider = provider; }

		/**
		 * \brief 외부에서의 데이터를 받을 때 사용하는 함수를 등록합니다.
		 * \param gatherer 외부의 데이터를 참조할 때 사용하는 함수 객체
		 */
		void RegisterGatherer(std::function<TData(void)> gatherer) { _dataGatherer = gatherer; }

	private:
		TData* _dataExternal = nullptr;

		std::function<void(TData*)> _dataProvider = nullptr;

		std::function<TData(void)> _dataGatherer = nullptr;

		bool _isValueChanged;

		TData* _data;
	};

	template <typename TData>
	void DataWidgetBase<TData>::Draw()
	{
		if (_isEnable)
		{
			// 내부 루프에서 바뀐게 있다면 바뀐 데이터를 받고 ..
			if (_dataExternal != nullptr)
				*_data = *_dataExternal;
			else if (_dataGatherer != nullptr)
				*_data = _dataGatherer();

			// 그리면서 업데이트 하고
			WidgetBase::Draw();

			// 업데이트 중 바뀐 내용을 외부에 전달합니다.
			if (_isValueChanged)
			{
				if (_dataExternal != nullptr)
					*_dataExternal = *_data;
				// _data의 주소를 받아서 안에서 값을 받아서 행동합니다.
				else if (_dataGatherer != nullptr)
					_dataProvider(_data);

				_isValueChanged = false;
			}
		}
	}

	template <typename TData>
	void DataWidgetBase<TData>::Notify()
	{
		// 데이터의 값이 변경되었습니다. => Provide 할 필요가 있습니다.
		_isValueChanged = true;
	}
}
