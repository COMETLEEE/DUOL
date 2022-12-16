#include "DUOLEditor/UI/AddOns/AddOnable.h"

namespace DUOLEditor
{
	AddOnable::~AddOnable()
	{
		RemoveAllAddOns();
	}

	void AddOnable::ExecuteAllAddOns()
	{
		for (auto& addIn : _addOns)
			addIn->Execute();
	}

	void AddOnable::RemoveAddOn(AddOnBase* addOn)
	{
		std::erase_if(_addOns, [addOn](auto& addOnIn)
			{
				// 주소가 같다면 리스트에서 지웁니다.
				if (addOn == addOnIn.get())
				{
					addOnIn.reset();

					return true;
				}
				else
					return false;
			});
	}

	void AddOnable::RemoveAllAddOns()
	{
		for (auto& addOn : _addOns)
			addOn.reset();

		_addOns.clear();
	}
}