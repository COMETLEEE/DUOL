/**

    @file      enabled_shared_from_base.h
    @brief     이미 기본 클래스에서 std::enable_shared_from_this를 상속받았는데 파생 클래스에서도 shared_from_this() 가
    사용하고 싶은 경우가 생길 때 ..! 상속받는 클래스.
    @details   ~
    @author    COMETLEE
    @date      7.11.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include "DUOLGameEngine/Export_Engine.h"

#include <memory>

namespace DUOLGameEngine
{
    template <typename Type>
    concept NotConst = 
        !std::is_const_v<Type>;

    template <typename Type>
    concept CanMakeSharedFromThis =
        std::derived_from<Type, std::enable_shared_from_this<Type>>;

    template <typename TDerived, typename TBase>
    concept Derived =
        std::derived_from<TDerived, TBase>;

	/**
	 * \brief 상속받은 기본 클래스에서 이미 'std::enable_shared_from_this' 를 상속받아 사용하고 있는 경우
	 * \tparam TDerived TBase의 파생 클래스
	 * \tparam TBase TDerived의 기본 클래스
	 */
	template <typename TDerived, typename TBase>
		requires CanMakeSharedFromThis<TBase>
    class DUOL_GAMEENGINE_API enable_shared_from_base
	{
	public:
		enable_shared_from_base() requires Derived<TDerived, enable_shared_from_base<TDerived, TBase>> && Derived<TDerived, TBase>
		: _priBase(static_cast<TBase*>(static_cast<TDerived*>(this))) {}
		
        [[nodiscard]]
        std::shared_ptr<TDerived> shared_from_base() noexcept
		{
            std::shared_ptr<TBase> sharedBase = _priBase->shared_from_this();

            return std::static_pointer_cast<TDerived>(sharedBase);
        }

        [[nodiscard]]
        std::shared_ptr<const TDerived> shared_from_base() const noexcept
        {
            std::shared_ptr<TBase> sharedBase = _priBase->shared_from_this();

            return std::static_pointer_cast<const TDerived>(sharedBase);
        }

        [[nodiscard]]
        std::weak_ptr<TDerived> weak_from_base() noexcept
        {
            return _priBase->weak_from_this();
        }

        [[nodiscard]]
        std::weak_ptr<const TDerived> weak_from_base() const noexcept
        {
            return weak_from_base();
        }

    private:
        mutable TBase* _priBase;
	};

    // 이것은 기본 클래스에 상속 시키고 사용하는 버전이지만 마음에 들지 않아서 봉인 ..!
    /*template <typename TBase>
    class enable_shared_from_base : public std::enable_shared_from_this<TBase>
    {
    public:
        template <typename TDerived>
        std::shared_ptr<TDerived> shared_from_base()
        {
            std::shared_ptr<TBase> temp = this->shared_from_this();

            return std::static_pointer_cast<TDerived>(temp);
        }

    public:
        template <typename TDerived>
        std::shared_ptr<TDerived> weak_from_base()
        {
            return std::static_pointer_cast<TDerived>(this->shared_from_base());
        }
    };*/
}