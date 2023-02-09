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
		 * \brief UUID �� ���� Serialize �� ��� ������ ���ΰ��� ���� �����Դϴ�.
		 */
		, UUIDSerializeType

		/**
		 * \brief Current
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
	};

	/**
	 * \brief UUID �� �ø�������� �մϴ�. �ٸ�, ��� ������ ������ ���� ���� �����Դϴ�. (���Ŀ� ���� �Ǵ� ���� ��ø�������� �������� ����)
	 */
	enum class UUIDSerializeType
	{
		// ���� ��ø��������� ���ÿ� ����, �ش� ���̵��� ��ü�� ã�� �� �ִ� ���
		FileUUID

		// ��ø������� �Ϸ�ǰ� ���� ������ �����ؼ� �����ϴ� �༮�� �ĺ�
		, AnimatorController
		, AnimationClip
		, Mesh
		, PhysicsMaterial
		, Resource
	};
}