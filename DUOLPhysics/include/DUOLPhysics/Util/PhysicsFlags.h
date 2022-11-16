/**

	@file    PhysicsFlags.h
	@brief	 중복된 Flag를 변하게 사용하기 위한 클래스
	@details -
	@author  JKim
	@date    16.11.2022

**/
#pragma once

namespace DUOLPhysics
{
	/**

		@class   PhysicsFlags
		@brief	 중복된 Flag를 변하게 사용하기 위한 클래스
		@details -
		@tparam  EnumFlag    - EnumFlag
		@tparam  BitMaskType - Flag를 저장할 Type

	**/
	template<typename EnumFlag, typename BitMaskType = int>
	class PhysicsFlags
	{
		using Flags = PhysicsFlags<EnumFlag, BitMaskType>;

	public:
		PhysicsFlags();

		PhysicsFlags(EnumFlag enumType);

		PhysicsFlags(const BitMaskType& bitMask);

		PhysicsFlags(const PhysicsFlags<EnumFlag, BitMaskType>& other);

		~PhysicsFlags() = default;

	private:
		BitMaskType _bitMask;

	public:
		const BitMaskType& GetBitMask() const { return _bitMask; };

		Flags& operator =(EnumFlag enumType);

		Flags& operator = (const Flags& other);

		Flags& operator |= (EnumFlag enumType);

		Flags& operator |= (const Flags& other);

		Flags& operator &= (EnumFlag enumType);

		Flags& operator &= (const Flags& other);

		Flags& operator ^= (EnumFlag enumType);

		Flags& operator ^= (const Flags& other);

		bool operator == (EnumFlag enumType);

		bool operator == (const Flags& other);

		bool operator != (EnumFlag enumType);

		bool operator != (const Flags& other);

		Flags operator | (EnumFlag enumType);

		Flags operator | (const Flags& other);

		Flags operator & (EnumFlag enumType);

		Flags operator & (const Flags& other);

		Flags operator ^ (EnumFlag enumType);

		Flags operator ^ (const Flags& other);

		Flags operator ~ ();
	};

	template<typename EnumFlag, typename BitMaskType>
	inline PhysicsFlags<EnumFlag, BitMaskType>::PhysicsFlags() :
		_bitMask(0)
	{

	}

	template<typename EnumFlag, typename BitMaskType>
	inline PhysicsFlags<EnumFlag, BitMaskType>::PhysicsFlags(EnumFlag enumType) :
		_bitMask(static_cast<BitMaskType>(enumType))
	{

	}

	template<typename EnumFlag, typename BitMaskType>
	inline PhysicsFlags<EnumFlag, BitMaskType>::PhysicsFlags(const BitMaskType& bitMask) :
		_bitMask(bitMask)
	{

	}

	template<typename EnumFlag, typename BitMaskType>
	inline PhysicsFlags<EnumFlag, BitMaskType>::PhysicsFlags(const PhysicsFlags<EnumFlag, BitMaskType>& other) :
		_bitMask(other._bitMask)
	{

	}

	template<typename EnumFlag, typename BitMaskType>
	inline PhysicsFlags<EnumFlag, BitMaskType>::Flags& PhysicsFlags<EnumFlag, BitMaskType>::operator=(EnumFlag enumType)
	{
		this->_bitMask = static_cast<BitMaskType>(enumType);

		return *this;
	}

	template<typename EnumFlag, typename BitMaskType>
	inline PhysicsFlags<EnumFlag, BitMaskType>::Flags& PhysicsFlags<EnumFlag, BitMaskType>::operator=(const Flags& other)
	{
		this->_bitMask = other._bitMask;

		return *this;
	}

	template<typename EnumFlag, typename BitMaskType>
	inline PhysicsFlags<EnumFlag, BitMaskType>::Flags& PhysicsFlags<EnumFlag, BitMaskType>::operator|=(EnumFlag enumType)
	{
		this->_bitMask |= static_cast<BitMaskType>(enumType);

		return *this;
	}

	template<typename EnumFlag, typename BitMaskType>
	inline PhysicsFlags<EnumFlag, BitMaskType>::Flags& PhysicsFlags<EnumFlag, BitMaskType>::operator|=(const Flags& other)
	{
		this->_bitMask |= other._bitMask;

		return *this;
	}

	template<typename EnumFlag, typename BitMaskType>
	inline PhysicsFlags<EnumFlag, BitMaskType>::Flags& PhysicsFlags<EnumFlag, BitMaskType>::operator&=(EnumFlag enumType)
	{
		this->_bitMask &= static_cast<BitMaskType>(enumType);

		return *this;
	}

	template<typename EnumFlag, typename BitMaskType>
	inline PhysicsFlags<EnumFlag, BitMaskType>::Flags& PhysicsFlags<EnumFlag, BitMaskType>::operator&=(const Flags& other)
	{
		this->_bitMask &= other._bitMask;

		return *this;
	}

	template<typename EnumFlag, typename BitMaskType>
	inline PhysicsFlags<EnumFlag, BitMaskType>::Flags& PhysicsFlags<EnumFlag, BitMaskType>::operator^=(EnumFlag enumType)
	{
		this->_bitMask ^= static_cast<BitMaskType>(enumType);

		return *this;
	}

	template<typename EnumFlag, typename BitMaskType>
	inline PhysicsFlags<EnumFlag, BitMaskType>::Flags& PhysicsFlags<EnumFlag, BitMaskType>::operator^=(const Flags& other)
	{
		this->_bitMask ^= other._bitMask;

		return *this;
	}

	template<typename EnumFlag, typename BitMaskType>
	inline bool PhysicsFlags<EnumFlag, BitMaskType>::operator==(EnumFlag enumType)
	{
		return this->_bitMask == static_cast<BitMaskType>(enumType);
	}

	template<typename EnumFlag, typename BitMaskType>
	inline bool PhysicsFlags<EnumFlag, BitMaskType>::operator==(const Flags& other)
	{
		return this->_bitMask == other._bitMask;
	}

	template<typename EnumFlag, typename BitMaskType>
	inline bool PhysicsFlags<EnumFlag, BitMaskType>::operator!=(EnumFlag enumType)
	{
		return this->_bitMask != static_cast<BitMaskType>(enumType);
	}

	template<typename EnumFlag, typename BitMaskType>
	inline bool PhysicsFlags<EnumFlag, BitMaskType>::operator!=(const Flags& other)
	{
		return this->_bitMask != other._bitMask;
	}

	template<typename EnumFlag, typename BitMaskType>
	inline PhysicsFlags<EnumFlag, BitMaskType>::Flags PhysicsFlags<EnumFlag, BitMaskType>::operator|(EnumFlag enumType)
	{
		return Flags(this->_bitMask | static_cast<BitMaskType>(enumType));
	}

	template<typename EnumFlag, typename BitMaskType>
	inline PhysicsFlags<EnumFlag, BitMaskType>::Flags PhysicsFlags<EnumFlag, BitMaskType>::operator|(const Flags& other)
	{
		return Flags(this->_bitMask | other._bitMask);
	}

	template<typename EnumFlag, typename BitMaskType>
	inline PhysicsFlags<EnumFlag, BitMaskType>::Flags PhysicsFlags<EnumFlag, BitMaskType>::operator&(EnumFlag enumType)
	{
		return Flags(this->_bitMask & static_cast<BitMaskType>(enumType));
	}

	template<typename EnumFlag, typename BitMaskType>
	inline PhysicsFlags<EnumFlag, BitMaskType>::Flags PhysicsFlags<EnumFlag, BitMaskType>::operator&(const Flags& other)
	{
		return Flags(this->_bitMask & other._bitMask);
	}

	template<typename EnumFlag, typename BitMaskType>
	inline PhysicsFlags<EnumFlag, BitMaskType>::Flags PhysicsFlags<EnumFlag, BitMaskType>::operator^(EnumFlag enumType)
	{
		return Flags(this->_bitMask ^ static_cast<BitMaskType>(enumType));
	}

	template<typename EnumFlag, typename BitMaskType>
	inline PhysicsFlags<EnumFlag, BitMaskType>::Flags PhysicsFlags<EnumFlag, BitMaskType>::operator^(const Flags& other)
	{
		return Flags(this->_bitMask ^ other._bitMask);
	}

	template<typename EnumFlag, typename BitMaskType>
	inline PhysicsFlags<EnumFlag, BitMaskType>::Flags PhysicsFlags<EnumFlag, BitMaskType>::operator~()
	{
		return Flag(~this->_bitMask);
	}
}