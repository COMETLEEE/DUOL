#pragma once
#include "DUOLCommon/Event/Event.h"
#include "DUOLEditor/UI/Widgets/DataWidgetBase.h"

namespace DUOLEditor
{
	/**
	 * \brief 근데 이게 싱글에서만 적용되어서 .. 싱글 버티컬 필요하면 그 때 만들자.
	 */
	enum class SliderOrientation
	{
		Horizontal
		, Vertical
	};

	/**
	 * \brief 
	 * \tparam TData Data type used.
	 * \tparam Size count of scalars.
	 */
	template <typename TData, size_t Size>
	class SliderScalar : public DataWidgetBase<std::array<TData, Size>>
	{
		static_assert(Size > 0, "Invalid SliderScalar size. (1 or more required)");
		static_assert(std::is_scalar_v<TData>, "Invalid SliderScalar TData. (Scalar expected)");

	public:
		SliderScalar(TData min, TData max, TData value, const DUOLCommon::tstring& label, const DUOLCommon::tstring& format)
			: DataWidgetBase<std::array<TData, Size>>(&_values)
			, _min(min)
			, _max(max)
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

			for (size_t i = 0; i < Size; ++i)
			{
				if (_values[i] < _min)
					_values[i] = _min;
				else if (_values[i] > _max)
					_values[i] = _max;
			}

			bool valueChanged = ImGui::SliderScalarN(DUOLCommon::StringHelper::ToString(_label + this->_tstringID).c_str(), 
				_dataType, _values.data(), Size, &_min, &_max, DUOLCommon::StringHelper::ToString(_format).c_str());

			if (valueChanged)
			{
				_valueChangedEvent.Invoke(_values);

				this->Notify();
			}
		}

	public:
		TData _min;

		TData _max;

		std::array<TData, Size> _values;

		DUOLCommon::tstring _label;

		DUOLCommon::tstring _format;

		DUOLCommon::Event<void, std::array<TData, Size>> _valueChangedEvent;

	protected:
		ImGuiDataType _dataType;
	};
}