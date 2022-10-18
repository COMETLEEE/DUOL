/**

    @file      SingletonBase.h
    @brief     Template C++ Singleton Base class
    @details   ~
    @author    COMETLEE
    @date      18.10.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include "DUOLGameEngine/Util/Defines.h"
#include <mutex>

namespace DUOLGameEngine
{
	/**
	 * \brief Singleton Pattern을 적용할 수 있는 템플릿 기본 클래스
	 * \tparam TClass Singleton Pattern을 적용하고 싶은 클래스
	 */
	template <typename TClass>
	class SingletonBase
	{
		DELETE_COPY_MOVE(SingletonBase<TClass>)

	public:
		struct Deleter
		{
			void operator()(TClass* instance)
			{
				delete instance;
			}
		};

	private:
		static std::unique_ptr<TClass, Deleter> _instance;

		static std::once_flag _flag;

	protected:
		SingletonBase() = default;

		virtual ~SingletonBase() = default;

	public:
		static const std::unique_ptr<TClass, Deleter>& GetInstance()
		{
			std::call_once(_flag, []() { _instance.reset(new TClass); });

			return _instance;
		}
	};

	template <typename TClass>
	std::unique_ptr<TClass, typename SingletonBase<TClass>::Deleter>
		SingletonBase<TClass>::_instance = std::unique_ptr<TClass, typename SingletonBase<TClass>::Deleter>(nullptr, typename
			SingletonBase<TClass>::Deleter());

	template <typename TClass>
	std::once_flag SingletonBase<TClass>::_flag;
}