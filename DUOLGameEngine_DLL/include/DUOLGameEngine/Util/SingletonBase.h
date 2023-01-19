/**

    @file      SingletonBase.h
    @brief     Template C++ Singleton Base class
    @details   ~
    @author    COMETLEE
    @date      18.10.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include "DUOLGameEngine/Export_Engine.h"

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
		static std::shared_ptr<TClass> _instance;

		static std::once_flag _flag;

	protected:
		SingletonBase() = default;

		virtual ~SingletonBase() = default;

	public:
		inline static const std::shared_ptr<TClass>& GetInstance()
		{
			std::call_once(_flag, []()
			{
				TClass* prim = new TClass();

				_instance = std::shared_ptr<TClass>(prim, Deleter());
			});

			return _instance;
		}
	};

	template <typename TClass>
	std::shared_ptr<TClass>
		SingletonBase<TClass>::_instance = std::shared_ptr<TClass>();

	template <typename TClass>
	std::once_flag SingletonBase<TClass>::_flag;
}