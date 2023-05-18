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
		 // TODO : 배열도 지원하는데 일차원 배열만 지원합니다 ..!
		, SerializeByUUID

		/**
		 * \brief UUID 를 통한 Serialize 를 어떻게 진행할 것인가에 대한 서술입니다.
		 */
		, MappingType

		/**
		 * \brief true => serialize by string. / false => serialize by common data.
		 * TODO : 일차원 배열만 지원합니다.
		 */
		, SerializeByString

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
		, Color4
		, Int

		// Special
		, AnimatorController
		, Mesh
		, SkinnedMesh
		, UIFileName
		, ButtonEvent
		, TextEvent
		, AudioClip
		, Material
		, Layer
		, Structure
	};

	/**
	 * \brief UUID 로 시리얼라이즈 합니다. 다만, 어떻게 연결을 지어줄 지에 대한 문제입니다. (사후에 연결 또는 현재 디시리얼라이즈 과정에서 연결)
	 */
	enum class MappingType
	{
		// 현재 디시리얼라이즈와 동시에 연결, 해당 아이디의 객체를 찾을 수 있는 경우
		FileUUID

		// 디시리얼라이즈가 완료되고 게임 엔진을 참조해서 연결하는 녀석들 식별용
		, Resource
	};
}