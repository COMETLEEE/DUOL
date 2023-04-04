#pragma once

namespace DUOLSound
{
	/**
	 * \brief 중복되는 사운드 모듈 이용 플래그를 선언으로만 처리하기 위한 템플릿 클래스
	 * \tparam EnumFlag 플래그 연산에 사용하는 enum class.
	 * \tparam BitMaskType 플래그 전체에 사용할 데이터 타입
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