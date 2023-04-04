#pragma once

namespace DUOLSound
{
	/**
	 * \brief �ߺ��Ǵ� ���� ��� �̿� �÷��׸� �������θ� ó���ϱ� ���� ���ø� Ŭ����
	 * \tparam EnumFlag �÷��� ���꿡 ����ϴ� enum class.
	 * \tparam BitMaskType �÷��� ��ü�� ����� ������ Ÿ��
	 */
	template<typename EnumFlag, typename BitMaskType = int>
	class Flags
	{
	public:
		Flags();

		Flags(EnumFlag enumType);

		Flags(const BitMaskType& bitMask);

		Flags(const Flags<EnumFlag, BitMaskType>& other);

		~Flags() = default;

	private:
		BitMaskType _bitMask;

	public:
		const BitMaskType& GetBitMask() const { return _bitMask; };

		Flags<EnumFlag, BitMaskType>& operator =(EnumFlag enumType);

		Flags<EnumFlag, BitMaskType>& operator = (const Flags<EnumFlag, BitMaskType>& other);

		Flags<EnumFlag, BitMaskType>& operator |= (EnumFlag enumType);

		Flags<EnumFlag, BitMaskType>& operator |= (const Flags<EnumFlag, BitMaskType>& other);

		Flags<EnumFlag, BitMaskType>& operator &= (EnumFlag enumType);

		Flags<EnumFlag, BitMaskType>& operator &= (const Flags<EnumFlag, BitMaskType>& other);

		Flags<EnumFlag, BitMaskType>& operator ^= (EnumFlag enumType);

		Flags<EnumFlag, BitMaskType>& operator ^= (const Flags<EnumFlag, BitMaskType>& other);

		bool operator == (EnumFlag enumType);

		bool operator == (const Flags<EnumFlag, BitMaskType>& other);

		bool operator != (EnumFlag enumType);

		bool operator != (const Flags<EnumFlag, BitMaskType>& other);

		Flags<EnumFlag, BitMaskType> operator | (EnumFlag enumType);

		Flags<EnumFlag, BitMaskType> operator | (const Flags<EnumFlag, BitMaskType>& other);

		Flags<EnumFlag, BitMaskType> operator & (EnumFlag enumType);

		Flags<EnumFlag, BitMaskType> operator & (const Flags<EnumFlag, BitMaskType>& other);

		Flags<EnumFlag, BitMaskType> operator ^ (EnumFlag enumType);

		Flags<EnumFlag, BitMaskType> operator ^ (const Flags<EnumFlag, BitMaskType>& other);

		Flags<EnumFlag, BitMaskType> operator ~ ();
	};

	template<typename EnumFlag, typename BitMaskType>
	inline Flags<EnumFlag, BitMaskType>::Flags() :
		_bitMask(0)
	{

	}

	template<typename EnumFlag, typename BitMaskType>
	inline Flags<EnumFlag, BitMaskType>::Flags(EnumFlag enumType) :
		_bitMask(static_cast<BitMaskType>(enumType))
	{

	}

	template<typename EnumFlag, typename BitMaskType>
	inline Flags<EnumFlag, BitMaskType>::Flags(const BitMaskType& bitMask) :
		_bitMask(bitMask)
	{

	}

	template<typename EnumFlag, typename BitMaskType>
	inline Flags<EnumFlag, BitMaskType>::Flags(const Flags<EnumFlag, BitMaskType>& other) :
		_bitMask(other._bitMask)
	{

	}

	template<typename EnumFlag, typename BitMaskType>
	inline Flags<EnumFlag, BitMaskType>& Flags<EnumFlag, BitMaskType>::operator=(EnumFlag enumType)
	{
		this->_bitMask = static_cast<BitMaskType>(enumType);

		return *this;
	}

	template<typename EnumFlag, typename BitMaskType>
	inline Flags<EnumFlag, BitMaskType>& Flags<EnumFlag, BitMaskType>::operator=(const Flags<EnumFlag, BitMaskType>& other)
	{
		this->_bitMask = other._bitMask;

		return *this;
	}

	template<typename EnumFlag, typename BitMaskType>
	inline Flags<EnumFlag, BitMaskType>& Flags<EnumFlag, BitMaskType>::operator|=(EnumFlag enumType)
	{
		this->_bitMask |= static_cast<BitMaskType>(enumType);

		return *this;
	}

	template<typename EnumFlag, typename BitMaskType>
	inline Flags<EnumFlag, BitMaskType>& Flags<EnumFlag, BitMaskType>::operator|=(const Flags<EnumFlag, BitMaskType>& other)
	{
		this->_bitMask |= other._bitMask;

		return *this;
	}

	template<typename EnumFlag, typename BitMaskType>
	inline Flags<EnumFlag, BitMaskType>& Flags<EnumFlag, BitMaskType>::operator&=(EnumFlag enumType)
	{
		this->_bitMask &= static_cast<BitMaskType>(enumType);

		return *this;
	}

	template<typename EnumFlag, typename BitMaskType>
	inline Flags<EnumFlag, BitMaskType>& Flags<EnumFlag, BitMaskType>::operator&=(const Flags<EnumFlag, BitMaskType>& other)
	{
		this->_bitMask &= other._bitMask;

		return *this;
	}

	template<typename EnumFlag, typename BitMaskType>
	inline Flags<EnumFlag, BitMaskType>& Flags<EnumFlag, BitMaskType>::operator^=(EnumFlag enumType)
	{
		this->_bitMask ^= static_cast<BitMaskType>(enumType);

		return *this;
	}

	template<typename EnumFlag, typename BitMaskType>
	inline Flags<EnumFlag, BitMaskType>& Flags<EnumFlag, BitMaskType>::operator^=(const Flags<EnumFlag, BitMaskType>& other)
	{
		this->_bitMask ^= other._bitMask;

		return *this;
	}

	template<typename EnumFlag, typename BitMaskType>
	inline bool Flags<EnumFlag, BitMaskType>::operator==(EnumFlag enumType)
	{
		return this->_bitMask == static_cast<BitMaskType>(enumType);
	}

	template<typename EnumFlag, typename BitMaskType>
	inline bool Flags<EnumFlag, BitMaskType>::operator==(const Flags<EnumFlag, BitMaskType>& other)
	{
		return this->_bitMask == other._bitMask;
	}

	template<typename EnumFlag, typename BitMaskType>
	inline bool Flags<EnumFlag, BitMaskType>::operator!=(EnumFlag enumType)
	{
		return this->_bitMask != static_cast<BitMaskType>(enumType);
	}

	template<typename EnumFlag, typename BitMaskType>
	inline bool Flags<EnumFlag, BitMaskType>::operator!=(const Flags<EnumFlag, BitMaskType>& other)
	{
		return this->_bitMask != other._bitMask;
	}

	template<typename EnumFlag, typename BitMaskType>
	inline Flags<EnumFlag, BitMaskType> Flags<EnumFlag, BitMaskType>::operator|(EnumFlag enumType)
	{
		return Flags(this->_bitMask | static_cast<BitMaskType>(enumType));
	}

	template<typename EnumFlag, typename BitMaskType>
	inline Flags<EnumFlag, BitMaskType> Flags<EnumFlag, BitMaskType>::operator|(const Flags<EnumFlag, BitMaskType>& other)
	{
		return Flags(this->_bitMask | other._bitMask);
	}

	template<typename EnumFlag, typename BitMaskType>
	inline Flags<EnumFlag, BitMaskType> Flags<EnumFlag, BitMaskType>::operator&(EnumFlag enumType)
	{
		return Flags(this->_bitMask & static_cast<BitMaskType>(enumType));
	}

	template<typename EnumFlag, typename BitMaskType>
	inline Flags<EnumFlag, BitMaskType> Flags<EnumFlag, BitMaskType>::operator&(const Flags<EnumFlag, BitMaskType>& other)
	{
		return Flags(this->_bitMask & other._bitMask);
	}

	template<typename EnumFlag, typename BitMaskType>
	inline Flags<EnumFlag, BitMaskType> Flags<EnumFlag, BitMaskType>::operator^(EnumFlag enumType)
	{
		return Flags(this->_bitMask ^ static_cast<BitMaskType>(enumType));
	}

	template<typename EnumFlag, typename BitMaskType>
	inline Flags<EnumFlag, BitMaskType> Flags<EnumFlag, BitMaskType>::operator^(const Flags<EnumFlag, BitMaskType>& other)
	{
		return Flags(this->_bitMask ^ other._bitMask);
	}

	template<typename EnumFlag, typename BitMaskType>
	inline Flags<EnumFlag, BitMaskType> Flags<EnumFlag, BitMaskType>::operator~()
	{
		return Flag(~this->_bitMask);
	}
}