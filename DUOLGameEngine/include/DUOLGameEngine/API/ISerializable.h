#pragma once

namespace DUOLGameEngine
{
	/**
	 * \brief ISerializable Ŭ������ Serialize, Deserialize ������ Ŭ������ �������̽��Դϴ�.
	 * ���� DUOLJson (== Serializer ..) ������ �ش� �������̽��� �̵��� �����Դϴ�.
	 */
	class ISerializable
	{
	public:
		ISerializable() = default;

		ISerializable(const ISerializable& other) = default;

		ISerializable& operator=(const ISerializable& other) = default;

		ISerializable(ISerializable&& other) = default;

		ISerializable& operator=(ISerializable&& other) = default;

		/**
		 * \brief �������� ���� ���� �Ҹ���
		 */
		virtual ~ISerializable() = default;

		/**
		 * \brief Serialize �� �� ȣ��ȴ�.
		 */
		virtual void OnSerialize() {}

		/**
		 * \brief Desirialize �� �� ȣ��ȴ�.
		 */
		virtual void OnDeserialize() {}
	};
}