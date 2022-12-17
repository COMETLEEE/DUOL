#pragma once
#include "DUOLCommon/Event/Event.h"
#include "DUOLEditor/UI/Widgets/DataWidgetBase.h"

namespace DUOLEditor
{
	/**
	 * \brief display a input field on panel.
	 * \tparam TData data type to input.
	 * \tparam Size count of data.
	 */
	template <typename TData, size_t Size>
	class InputScalar : public DataWidgetBase<std::array<TData, Size>>
	{
		static_assert(Size > 0, "Invalid InputScalar size. (2 or more required)");
		static_assert(std::is_scalar_v<TData>, "Invalid InputScalar TData. (Scalar expected)");

	public:
		InputScalar(TData defaultValue, TData step, TData fastStep,
			const DUOLCommon::tstring& label, const DUOLCommon::tstring& format, bool selectAllOnClick) :
		DataWidgetBase<std::array<TData, Size>>(&_values)
		, _step(step)
		, _fastStep(fastStep)
		, _label(label)
		, _format(format)
		, _selectAllOnClick(selectAllOnClick)
		{
			// 템플릿 특수화를 잘 사용해보자. 어차피 내부에서 사용할 예정이니까 .. 이렇게만 해도 무방할듯 ?
			if constexpr (std::is_same_v<TData, int>)
				_dataType = ImGuiDataType_S32;
			else if constexpr (std::is_same_v<TData, float>)
				_dataType = ImGuiDataType_Float;
			else if constexpr (std::is_same_v<TData, double>)
				_dataType = ImGuiDataType_Double;
			else
				_dataType = ImGuiDataType_S8;

			_values.fill(defaultValue);
		}

	protected:
		virtual void Draw_Impl() override
		{
			std::array<TData, Size> prevValues = _values;

			ImGuiInputTextFlags flags = ImGuiInputTextFlags_EnterReturnsTrue;

			if (_selectAllOnClick)
				flags |= ImGuiInputTextFlags_AutoSelectAll;

			bool enterPressed = ImGui::InputScalarN(DUOLCommon::StringHelper::ToString(_label + this->_tstringID).c_str(), _dataType, _values.data(),
				Size, _step != 0.0f ? &_step : nullptr, _fastStep != 0.0f ? &_fastStep : nullptr,
				DUOLCommon::StringHelper::ToString(_format).c_str(), flags);

			bool hasChanged = false;

			for (size_t i = 0; i < Size; ++i)
				if (prevValues[i] != _values[i])
				{
					hasChanged = true;

					break;
				}

			if (hasChanged)
			{
				_valueChangedEvent.Invoke(_values);

				this->Notify();
			}

			if (enterPressed)
				_enterPressedEvent.Invoke(_values);
		}

	public:
		std::array<TData, Size> _values;

		TData _step;

		TData _fastStep;

		DUOLCommon::tstring _label;

		DUOLCommon::tstring _format;

		bool _selectAllOnClick;

		DUOLCommon::Event<void, std::array<TData, Size>&> _valueChangedEvent;

		DUOLCommon::Event<void, std::array<TData, Size>&> _enterPressedEvent;

	private:
		ImGuiDataType _dataType;
	};
}