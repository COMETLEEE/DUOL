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
		 // TODO : �迭�� �����ϴµ� ������ �迭�� �����մϴ� ..!
		, SerializeByUUID

		/**
		 * \brief UUID �� ���� Serialize �� ��� ������ ���ΰ��� ���� �����Դϴ�.
		 */
		, MappingType

		/**
		 * \brief true => serialize by string. / false => serialize by common data.
		 * TODO : ������ �迭�� �����մϴ�.
		 */
		, SerializeByString

		/**
		 * \brief true => inspect. / false => No inspect.
		 */
		, Inspectable

		// ��� Inspect �� ���ΰ��� ���� ������ �� 'class InspectType' �Ű������� �޴� ��Ÿ ������ Ÿ���Դϴ�.
		/*
		 * Float, Float2, Float3, Float4	-	ImGui::DragScalarN<> �� ��Ÿ��
		 * String							-	ImGui::InputText
		 * Enumeration						-	ImGui::ComboBox �� ��Ÿ��
		 * Bool								-	ImGui::CheckBox �� ��Ÿ��
		 * Color							-	ImGui::ColorPicker
		 */
		, InspectType
	};

	/**
	 * \brief Inspect �� ��� �� ���ΰ� �����ϴ� ��
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
	 * \brief UUID �� �ø�������� �մϴ�. �ٸ�, ��� ������ ������ ���� ���� �����Դϴ�. (���Ŀ� ���� �Ǵ� ���� ��ø�������� �������� ����)
	 */
	enum class MappingType
	{
		// ���� ��ø��������� ���ÿ� ����, �ش� ���̵��� ��ü�� ã�� �� �ִ� ���
		FileUUID

		// ��ø������� �Ϸ�ǰ� ���� ������ �����ؼ� �����ϴ� �༮�� �ĺ���
		, Resource
	};
}