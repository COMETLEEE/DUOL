#pragma once
#include "DUOLCommon/Event/Event.h"
#include "DUOLEditor/UI/Widgets/DataWidgetBase.h"

namespace DUOLEditor
{
	template <typename TData, size_t Size>
	class DragScalar : public DataWidgetBase<std::array<TData, Size>>
	{
		static_assert(Size > 0, "Invalid DragScalar size. (2 or more required)");
		static_assert(std::is_scalar_v<TData>, "Invalid DragScalar TData. (Scalar expected)");

	public:
		DragScalar(TData min, TData max, TData value, TData speed,
			const DUOLCommon::tstring& label, const DUOLCommon::tstring format) :
			DataWidgetBase<std::array<TData, Size>>(&_values)
			, _min(min)
			, _max(max)
			, _speed(speed)
			, _label(label)
			, _format(format)
		{
			// 템플릿 특수화를 잘 이용해봅시다.
			if constexpr (std::is_same_v<TData, int>)
				_dataType = ImGuiDataType_S32;
			else if constexpr (std::is_same_v<TData, float>)
				_dataType = ImGuiDataType_Float;
			else if constexpr (std::is_same_v<TData, double>)
				_dataType = ImGuiDataType_Double;
			else
				_dataType = ImGuiDataType_S8;

			_values.fill(value);
		}

	protected:
		virtual void Draw_Impl() override
		{
			if (_max < _min)
				_max = _min;

			for (size_t i = 0 ; i < Size ; i++)
			{
				if (_values[i] < _min)
					_values[i] = _min;
				else if (_values[i] > _max)
					_values[i] = _max;
			}

			if (ImGui::DragScalarN(DUOLCommon::StringHelper::ToString(_label + this->_tstringID).c_str(), _dataType, _values.data(), Size, 
				_speed, &_min, &_max, DUOLCommon::StringHelper::ToString(_format).c_str()))
			{
				_valueChangedEvent.Invoke(_values);

				this->Notify();
			}
		}

	public:
		TData _min;

		TData _max;

		TData _speed;

		std::array<TData, Size> _values;

		DUOLCommon::tstring _label;

		DUOLCommon::tstring _format;

		DUOLCommon::Event<void, std::array<TData, Size>&> _valueChangedEvent;

	protected:
		ImGuiDataType _dataType;
	};
}