/**

    @file      AddInable.h
    @brief     애드인을 붙일 수 있는 클래스로 만듭니다.
    @details   ~
    @author    COMETLEE
    @date      16.12.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include <list>
#include <memory>

#include "DUOLEditor/UI/AddOns/AddOnBase.h"

namespace DUOLEditor
{
	/**
	 * \brief make your class able to have add-ins.
	 */
	class AddOnable
	{
	public:
		~AddOnable();

		template <typename TAddIn, typename ...Args>
		TAddIn* AddAddOn(Args&& ... args) requires std::derived_from<TAddIn, AddOnBase>
		{
			_addOns.push_back(std::make_shared<TAddIn>(std::forward<Args>(args)...));

			return _addOns.back().get();
		}

		template <typename TAddIn>
		TAddIn* GetAddIn() requires std::derived_from<TAddIn, AddOnBase>
		{
			for (auto iter = _addOns.begin(); iter != _addOns.end(); ++iter)
			{
				TAddIn* ret = std::dynamic_pointer_cast<TAddIn>(ret).get();

				if (ret != nullptr)
					return ret;
			}

			return nullptr;
		}

		/**
		 * \brief 모든 애드온을 실행합니다.
		 */
		void ExecuteAllAddOns();

		/**
		 * \brief 특정 애드온을 지웁니다.
		 * \param addIn 지우고자 하는 애드온
		 */
		void RemoveAddOn(AddOnBase* addOn);

		/**
		 * \brief 모든 애드온을 지웁니다.
		 */
		void RemoveAllAddOns();

	private:
		/**
		 * \brief 애드온 리스트입니다.
		 */
		std::list<std::shared_ptr<DUOLEditor::AddOnBase>> _addOns;
	};
}
