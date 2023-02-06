#pragma once

namespace DUOLCommon
{
	enum class MetaDataType
	{
		/**
		 * \brief true => serialize. / false => No serialize.
		 */
		Serializable

		/**
		 * \brief true => serialize by UUID. / false => serialize by common data.
		 */
		, SerializeByUUID

		/**
		 * \brief true => inspect. / false => No inspect.
		 */
		, Inspectable

		// 어떻게 Inspect 할 것인가에 대한 서술이 들어간 'class InspectType' 매개변수를 받는 메타 데이터 타입입니다.
		/*
		 * Float, Float2, Float3, Float4	-	ImGui::DragScalarN<> 로 나타냄
		 * String							-	ImGui::InputText
		 * Enumeration						-	ImGui::ComboBox 로 나타냄
		 * Bool								-	ImGui::CheckBox 로 나타냄
		 * Color							-	ImGui::ColorPicker
		 */
		, InspectType
	};

	/**
	 * \brief Inspect 를 어떻게 할 것인가 서술하는 값
	 */
	enum class InspectType
	{
		Float, Float2, Float3, Float4
		, String
		, Enumeration
		, Bool
		, Color
	};
}