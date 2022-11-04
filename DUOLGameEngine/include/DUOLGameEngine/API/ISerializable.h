#pragma once

namespace DUOLGameEngine
{
	/**
	 * \brief ISerializable 클래스는 Serialize, Deserialize 가능한 클래스의 인터페이스입니다.
	 * 추후 DUOLJson (== Serializer ..) 쪽으로 해당 인터페이스가 이동될 예정입니다.
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
		 * \brief 다형성을 위한 가상 소멸자
		 */
		virtual ~ISerializable() = default;

		/**
		 * \brief Serialize 할 때 호출된다.
		 */
		virtual void OnSerialize() {}

		/**
		 * \brief Desirialize 할 때 호출된다.
		 */
		virtual void OnDeserialize() {}
	};
}